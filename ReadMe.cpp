/* Using these filters is easy:

{{{
    // Create a Chebyshev type I Band Stop filter of order 3
    // with state for processing 2 channels of audio.
    Dsp::SimpleFilter <Dsp::ChebyshevI::BandStop <3>, 2> f;
    f.setup (3,    // order
             44100,// sample rate
             4000, // center frequency
             880,  // band width
             1);   // ripple dB
    f.process (numSamples, arrayOfChannels);
}}}

DOCUMENTATION

All symbols are in the Dsp namespace.

class Filter

  This is an abstract polymorphic interface that supports any filter. The
  parameters to the filter are passed in the Params structure, which is
  essentially an array of floating point numbers with a hard coded size
  limit (maxParameters). Each filter makes use of the Params as it sees fit.

  Filter::getKind ()
  Filter::getName ()
  Filter::getNumParams ()
  Filter::getParamInfo ()

  Through the use of these functions, the caller can determine the meaning
  of each indexed filter parameter at run-time. The ParamInfo structure
  contains methods that describe information about an individual parameter,
  including convenience functions to map a filter parameter to a "control
  value" in the range 0...1, suitable for presentation by a GUI element such
  as a knob or scrollbar.

  Filter::getDefaultParams ()
  Filter::getParams ()
  Filter::getParam ()
  Filter::setParam ()
  Filter::findParamId ()
  Filter::setParamById ()
  Filter::setParams ()
  Filter::copyParamsFrom ()

  These methods allow the caller to inspect the values of the parameters,
  and set the filter parameters in various ways. When parameters are changed
  they take effect on the filter immediately.

  Filter::getPoleZeros ()
  Filter::response ()

  For analysis, these routines provide insight into the pole/zero arrangement
  in the z-plane, and the complex valued response at a given normalized
  frequency in the range (0..nyquist = 0.5]. From the complex number the
  magnitude and phase can be calculated.

  Filter::getNumChannels()
  Filter::reset()
  Filter::process()

  These functions are for applying the filter to channels of data. If the
  filter was not created with channel state (i.e. Channels==0 in the derived
  class template) then they will throw an exception.

  To create a Filter object, use operator new on a subclass template with
  appropriate parameters based on the type of filter you want. Here are the
  subclasses.



template <class DesignClass, int Channels = 0, class StateType = DirectFormII>
class FilterDesign : public Filter

  This subclass of Filter takes a DesignClass (explained below) representing
  a filter, an optional parameter for the number of channels of data to
  process, and an optional customizable choice of which state realization
  to use for processing samples. Channels may be zero, in which case the
  object can only be used for analysis.

  Because the DesignClass is a member and not inherited, it is in general
  not possible to call members of the DesignClass directly. You must go
  through the Filter interface.



template <class DesignClass, int Channels, class StateType = DirectFormII>
class SmoothedFilterDesign : public Filter

  This subclass of FilterDesign implements a filter of the given DesignClass,
  and also performs smoothing of parameters over time. Specifically, when
  one or more filter parameters (such as cutoff frequency) are changed, the
  class creates a transition over a given number of samples from the original
  values to the new values. This process is invisible and seamless to the
  caller, except that the constructor takes an additional parameter that
  indicates the duration of transitions when parameters change.



template <class FilterClass, int Channels = 0, class StateType = DirectFormII>
class SimpleFilter : public FilterClass

  This is a simple wrapper around a given raw FilterClass (explained below).
  It uses inheritance so all of the members of the FilterClass are available
  to instances of this object. The simple wrapper provides state information
  for processing channels in the given form.

  The wrapper does not support introspection, parameter smoothing, or the
  Params style of applying filter settings. Instead, it uses the interface
  of the given FilterClass, which is typically a function called setup()
  that takes a list of arguments representing the parameters.

  The use of this class bypasses the virtual function overhead of going
  through a Filter object. It is not practical to change filter parameters
  of a SimpleFilter, unless you are re-using the filter for a brand new
  stream of data in which case reset() should be called immediately before
  or after changing parameters, to clear the state and prevent audible
  artifacts.



Filter family namespaces

  Each family of filters is given its own namespace. Currently these namespaces
  include:

  RBJ:          Filters from the RBJ Cookbook
  Butterworth:  Filters with Butterworth response
  ChebyshevI:   Filters using Chebyshev polynomials (ripple in the passband)
  ChebyshevII:  "Inverse Chebyshev" filters (ripple in the stopband)
  Elliptic:     Filters with ripple in both the passband and stopband
  Bessel:       Uses Bessel polynomials, theoretically with linear phase
  Legendre:     "Optimum-L" filters with steepest transition and monotonic passband.
  Custom:       Simple filters that allow poles and zeros to be specified directly

<class FilterClass>

  Within each namespace we have a set of "raw filters" (each one is an example
  of a FilterClass). For example, the raw filters in the Butterworth namespace are:

  Butterworth::LowPass
  Butterworth::HighPass
  Butterworth::BandPass
  Butterworth::BandStop
  Butterworth::LowShelf
  Butterworth::HighShelf
  Butterworth::BandShelf

  When a class template (such as SimpleFilter) requires a FilterClass, it is
  expecting an identifier of a raw filter. For example, Legendre::LowPass. The
  raw filters do not have any support for introspection or the Params style of
  changing filter settings. All they offer is a setup() function for updating
  the IIR coefficients to a given set of parameters.

<class DesignClass>

  Each filter family namespace also has the nested namespace "Design". Inside
  this namespace we have all of the raw filter names repeated, except that
  these classes additional provide the Design interface, which adds
  introspection, polymorphism, the Params style of changing filter settings,
  and in general all of the features necessary to interoperate with the Filter
  virtual base class and its derived classes. For example, the design filters
  from the Butterworth namespace are:

  Butterworth::Design::LowPass
  Butterworth::Design::HighPass
  Butterworth::Design::BandPass
  Butterworth::Design::BandStop
  Butterworth::Design::LowShelf
  Butterworth::Design::HighShelf
  Butterworth::Design::BandShelf

  For any class template that expects a DesignClass, you must pass a suitable
  object from the Design namespace of the desired filter family. For example,
  ChebyshevI::Design::BandPass.
  
// Usage Examples
// This is the only include you need
// This shows you how to operate the filters */

#include "DSP.h"
#include <sstream>
#include <iostream>
#include <iomanip>

namespace {

    void UsageExamples()
    {
        // create a two channel audio buffer
        int numSamples = 2000;
        float* audioData[2];
        audioData[0] = new float[numSamples];
        audioData[1] = new float[numSamples];

        // create a 2-channel RBJ Low Pass with parameter smoothing
        // and apply it to the audio data
        {
            // "1024" is the number of samples over which to fade parameter changes
            Dsp::Filter* f = new Dsp::SmoothedFilterDesign
                <Dsp::RBJ::Design::LowPass, 2>(1024);
            Dsp::Params params;
            params[0] = 44100; // sample rate
            params[1] = 4000; // cutoff frequency
            params[2] = 1.25; // Q
            f->setParams(params);
            f->process(numSamples, audioData);
        }

        // set up a 2-channel RBJ High Pass with parameter smoothing,
        // but bypass virtual function overhead
        {
            // the difference here is that we don't go through a pointer.
            Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2> f(1024);
            Dsp::Params params;
            params[0] = 44100; // sample rate
            params[1] = 4000; // cutoff frequency
            params[2] = 1.25; // Q
            f.setParams(params);
            f.process(numSamples, audioData);
        }

        // create a 2-channel Butterworth Band Pass of order 4,
        // with parameter smoothing and apply it to the audio data.
        // Output samples are generated using Direct Form II realization.
        {
            Dsp::Filter* f = new Dsp::SmoothedFilterDesign
                <Dsp::Butterworth::Design::BandPass <4>, 2, Dsp::DirectFormII>(1024);
            Dsp::Params params;
            params[0] = 44100; // sample rate
            params[1] = 4; // order
            params[2] = 4000; // center frequency
            params[3] = 880; // band width
            f->setParams(params);
            f->process(numSamples, audioData);
        }

        // create a 2-channel Inverse Chebyshev Low Shelf of order 5
        // and passband ripple 0.1dB, without parameter smoothing and apply it.
        {
            Dsp::Filter* f = new Dsp::FilterDesign
                <Dsp::ChebyshevII::Design::LowShelf <5>, 2>;
            Dsp::Params params;
            params[0] = 44100; // sample rate
            params[1] = 5; // order
            params[2] = 4000; // corner frequency
            params[3] = 6; // shelf gain
            params[4] = 0.1; // passband ripple
            f->setParams(params);
            f->process(numSamples, audioData);
        }

        // create an abstract Butterworth High Pass of order 4.
        // This one can't process channels, it can only be used for analysis
        // (i.e. extract poles and zeros).
        {
            Dsp::Filter* f = new Dsp::FilterDesign
                <Dsp::Butterworth::Design::HighPass <4> >;
            Dsp::Params params;
            params[0] = 44100; // sample rate
            params[1] = 4; // order
            params[2] = 4000; // cutoff frequency
            f->setParams(params);
            // this will cause a runtime assertion
            f->process(numSamples, audioData);
        }

        // Use the simple filter API to create a Chebyshev Band Stop of order 3
        // and 1dB ripple in the passband. The simle API has a smaller
        // footprint, but no introspection or smoothing.
        {
            // Note we use the raw filter instead of the one
            // from the Design namespace.
            Dsp::SimpleFilter <Dsp::ChebyshevI::BandStop <3>, 2> f;
            f.setup(3,    // order
                44100,// sample rate
                4000, // center frequency
                880,  // band width
                1);   // ripple dB
            f.process(numSamples, audioData);
        }

        // Set up a filter, extract the coefficients and print them to standard
        // output. Note that this filter is not capable of processing samples,
        // as it has no state. It only has coefficients.
        {
            Dsp::SimpleFilter <Dsp::RBJ::LowPass> f;
            f.setup(44100, // sample rate Hz
                440, // cutoff frequency Hz
                1); // "Q" (resonance)

            std::ostringstream os;

            os << "a0 = " << f.getA0() << "\n"
                << "a1 = " << f.getA1() << "\n"
                << "a2 = " << f.getA2() << "\n"
                << "b0 = " << f.getB0() << "\n"
                << "b1 = " << f.getB1() << "\n"
                << "b2 = " << f.getB2() << "\n";
            ;

            std::cout << os.str();
        }

        // Create an instance of a raw filter. This is as low as it gets, any
        // lower and we will just have either a Biquad or a Cascade, and you'll
        // be setting the coefficients manually.
        {
            // This is basically like eating uncooked food
            Dsp::RBJ::LowPass f;
            f.setup(44100, 440, 1);

            // calculate response at frequency 440 Hz
            Dsp::complex_t response = f.response(440. / 44100);
        }

        // Extract coefficients from a Cascade
        {
            Dsp::SimpleFilter <Dsp::Butterworth::HighPass <3> > f;
            f.setup(3, 44100, 2000);

            std::ostringstream os;

            os << "numStages = " << f.getNumStages() << "\n"
                << "a0[0] = " << f[0].getA0() << "\n"
                << "a1[0] = " << f[0].getA1() << "\n"
                << "a2[0] = " << f[0].getA2() << "\n"
                << "b0[0] = " << f[0].getB0() << "\n"
                << "b1[0] = " << f[0].getB1() << "\n"
                << "b2[0] = " << f[0].getB2() << "\n"
                << "a0[1] = " << f[1].getA0() << "\n"
                << "a1[1] = " << f[1].getA1() << "\n"
                << "a2[1] = " << f[1].getA2() << "\n"
                << "b0[1] = " << f[1].getB0() << "\n"
                << "b1[1] = " << f[1].getB1() << "\n"
                << "b2[1] = " << f[1].getB2() << "\n"
                ;

            std::cout << os.str();
        }
    }

}
