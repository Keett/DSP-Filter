#include "pch.h"
#include "Custom.h"
#include "Common.h"

namespace Dsp {

    namespace Custom {

        void OnePole::setup(double scale,
            double pole,
            double zero)
        {
            setOnePole(pole, zero);
            applyScale(scale);
        }

        void TwoPole::setup(double scale,
            double poleRho,
            double poleTheta,
            double zeroRho,
            double zeroTheta)
        {
            complex_t pole = std::polar(poleRho, poleTheta);
            complex_t zero = std::polar(zeroRho, zeroTheta);

            setTwoPole(pole, zero, std::conj(pole), std::conj(zero));
            applyScale(scale);
        }

    }

}
