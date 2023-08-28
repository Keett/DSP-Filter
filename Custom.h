#ifndef DSPFILTERS_CUSTOM_H
#define DSPFILTERS_CUSTOM_H

#include "Common.h"
#include "Biquad.h"
#include "Design.h"
#include "Filter.h"

namespace Dsp {

    /*
     * Single pole and Biquad with parameters allowing
     * for directly setting the poles and zeros
     *
     */

    namespace Custom {

        //
        // Raw filters
        //

        struct OnePole : Biquad
        {
            void setup(double scale,
                double pole,
                double zero);
        };

        struct TwoPole : Biquad
        {
            void setup(double scale,
                double poleRho,
                double poleTheta,
                double zeroRho,
                double zeroTheta);
        };

        //------------------------------------------------------------------------------

        //
        // Gui-friendly Design layer
        //

        namespace Design {

            struct OnePole : DesignBase, Custom::OnePole
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
                    return ParamInfo::defaultGainParam();
                }

                static const ParamInfo getParamInfo_2()
                {
                    return ParamInfo::defaultPoleRealParam();
                }

                static const ParamInfo getParamInfo_3()
                {
                    return ParamInfo::defaultZeroRealParam();
                }

                static Kind getKind() { return kindOther; }
                static const char* getName() { return "Custom One-Pole"; }

                void setParams(const Params& params)
                {
                    setup(pow(10., params[1] / 20),
                        params[2],
                        params[3]);
                }
            };

            struct TwoPole : DesignBase, Custom::TwoPole
            {
                enum
                {
                    NumParams = 6
                };

                static int getNumParams()
                {
                    return 6;
                }

                static const ParamInfo getParamInfo_1()
                {
                    return ParamInfo::defaultGainParam();
                }

                static const ParamInfo getParamInfo_2()
                {
                    return ParamInfo::defaultPoleRhoParam();
                }

                static const ParamInfo getParamInfo_3()
                {
                    return ParamInfo::defaultPoleThetaParam();
                }

                static const ParamInfo getParamInfo_4()
                {
                    return ParamInfo::defaultZeroRhoParam();
                }

                static const ParamInfo getParamInfo_5()
                {
                    return ParamInfo::defaultZeroThetaParam();
                }


                static Kind getKind() { return kindOther; }
                static const char* getName() { return "Custom Two-Pole"; }

                void setParams(const Params& params)
                {
                    setup(pow(10., params[1] / 20),
                        params[2],
                        params[3],
                        params[4],
                        params[5]);
                }
            };

        }

    }

}

#endif
