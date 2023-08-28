#ifndef DSPFILTERS_RBJ_H
#define DSPFILTERS_RBJ_H

#include "Common.h"
#include "Biquad.h"
#include "Design.h"
#include "Filter.h"

namespace Dsp {

    /*
     * Filter realizations based on Robert Bristol-Johnson formulae:
     *
     * http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
     *
     */

    namespace RBJ {

        //
        // Raw filters
        //

        struct LowPass : BiquadBase
        {
            void setup(double sampleRate,
                double cutoffFrequency,
                double q);
        };

        struct HighPass : BiquadBase
        {
            void setup(double sampleRate,
                double cutoffFrequency,
                double q);
        };

        struct BandPass1 : BiquadBase
        {
            // (constant skirt gain, peak gain = Q)
            void setup(double sampleRate,
                double centerFrequency,
                double bandWidth);
        };

        struct BandPass2 : BiquadBase
        {
            // (constant 0 dB peak gain)
            void setup(double sampleRate,
                double centerFrequency,
                double bandWidth);
        };

        struct BandStop : BiquadBase
        {
            void setup(double sampleRate,
                double centerFrequency,
                double bandWidth);
        };

        struct LowShelf : BiquadBase
        {
            void setup(double sampleRate,
                double cutoffFrequency,
                double gainDb,
                double shelfSlope);
        };

        struct HighShelf : BiquadBase
        {
            void setup(double sampleRate,
                double cutoffFrequency,
                double gainDb,
                double shelfSlope);
        };

        struct BandShelf : BiquadBase
        {
            void setup(double sampleRate,
                double centerFrequency,
                double gainDb,
                double bandWidth);
        };

        struct AllPass : BiquadBase
        {
            void setup(double sampleRate,
                double phaseFrequency,
                double q);
        };

        //------------------------------------------------------------------------------

        //
        // Gui-friendly Design layer
        //

        namespace Design {

            struct TypeIBase : DesignBase
            {
                enum
                {
                    NumParams = 3
                };

                static int getNumParams()
                {
                    return 3;
                }

                static const ParamInfo getParamInfo_1()
                {
                    return ParamInfo::defaultCutoffFrequencyParam();
                }

                static const ParamInfo getParamInfo_2()
                {
                    return ParamInfo::defaultQParam();
                }
            };

            template <class FilterClass>
            struct TypeI : TypeIBase, FilterClass
            {
                void setParams(const Params& params)
                {
                    FilterClass::setup(params[0], params[1], params[2]);
                }
            };

            struct TypeIIBase : DesignBase
            {
                enum
                {
                    NumParams = 3
                };

                static int getNumParams()
                {
                    return 3;
                }

                static const ParamInfo getParamInfo_1()
                {
                    return ParamInfo::defaultCenterFrequencyParam();
                }

                static const ParamInfo getParamInfo_2()
                {
                    return ParamInfo::defaultBandwidthParam();
                }
            };

            template <class FilterClass>
            struct TypeII : TypeIIBase, FilterClass
            {
                void setParams(const Params& params)
                {
                    FilterClass::setup(params[0], params[1], params[2]);
                }
            };

            struct TypeIIIBase : DesignBase
            {
                enum
                {
                    NumParams = 4
                };

                static int getNumParams()
                {
                    return 4;
                }

                static const ParamInfo getParamInfo_1()
                {
                    return ParamInfo::defaultCutoffFrequencyParam();
                }

                static const ParamInfo getParamInfo_2()
                {
                    return ParamInfo::defaultGainParam();
                }

                static const ParamInfo getParamInfo_3()
                {
                    return ParamInfo::defaultSlopeParam();
                }
            };

            template <class FilterClass>
            struct TypeIII : TypeIIIBase, FilterClass
            {
                void setParams(const Params& params)
                {
                    FilterClass::setup(params[0], params[1], params[2], params[3]);
                }
            };

            struct TypeIVBase : DesignBase
            {
                enum
                {
                    NumParams = 4
                };

                static int getNumParams()
                {
                    return 4;
                }

                static const ParamInfo getParamInfo_1()
                {
                    return ParamInfo::defaultCenterFrequencyParam();
                }

                static const ParamInfo getParamInfo_2()
                {
                    return ParamInfo::defaultGainParam();
                }

                static const ParamInfo getParamInfo_3()
                {
                    return ParamInfo::defaultBandwidthParam();
                }
            };

            template <class FilterClass>
            struct TypeIV : TypeIVBase, FilterClass
            {
                void setParams(const Params& params)
                {
                    FilterClass::setup(params[0], params[1], params[2], params[3]);
                }
            };

            //------------------------------------------------------------------------------

            struct LowPass : TypeI <RBJ::LowPass>
            {
                static Kind getKind() { return kindLowPass; }
                static const char* getName() { return "RBJ Low Pass"; }
            };

            struct HighPass : TypeI <RBJ::HighPass>
            {
                static Kind getKind() { return kindHighPass; }
                static const char* getName() { return "RBJ High Pass"; }
            };

            struct BandPass1 : TypeII <RBJ::BandPass1>
            {
                static Kind getKind() { return kindBandPass; }
                static const char* getName() { return "RBJ Band Pass 1"; }
            };

            struct BandPass2 : TypeII <RBJ::BandPass2>
            {
                static Kind getKind() { return kindBandPass; }
                static const char* getName() { return "RBJ Band Pass 2"; }
            };

            struct BandStop : TypeII <RBJ::BandStop>
            {
                static Kind getKind() { return kindBandStop; }
                static const char* getName() { return "RBJ Band Stop"; }
            };

            struct LowShelf : TypeIII <RBJ::LowShelf>
            {
                static Kind getKind() { return kindLowShelf; }
                static const char* getName() { return "RBJ Low Shelf"; }
            };

            struct HighShelf : TypeIII <RBJ::HighShelf>
            {
                static Kind getKind() { return kindHighShelf; }
                static const char* getName() { return "RBJ High Shelf"; }
            };

            struct BandShelf : TypeIV <RBJ::BandShelf>
            {
                static Kind getKind() { return kindBandShelf; }
                static const char* getName() { return "RBJ Band Shelf"; }
            };

            struct AllPass : TypeI <RBJ::AllPass>
            {
                static Kind getKind() { return kindOther; }
                static const char* getName() { return "RBJ All Pass"; }
            };

        }

    }

}

#endif
