#include "TF1.h"
#include "TH1.h"

TF1* createNSumOfLandauFunctions(const TH1D* histogram, double xMin, double xMax, int numLandau)
{
    // Create the TF1 object for the sum of Landau functions
    TString formula;
    for (int i = 0; i < numLandau; ++i) {
        formula += TString::Format("[%d]/100*TMath::Landau(x, [%d], [%d])", i, i + 1, i + 2);
        if (i < numLandau - 1) {
            formula += " + ";
        }
    }
    TF1* fitFunc = new TF1("fitFunc", formula, xMin, xMax);

    // Calculate the average value and sigma of the histogram
    double histogramAmpl = histogram->GetBinContent(histogram->GetMaximumBin()) / histogram->GetBinWidth(histogram->GetMaximumBin());
    double histogramMean = histogram->GetMean();
    double histogramSigma = histogram->GetRMS();

    // Set initial parameter values and ranges for each Landau function
    for (int i = 0; i < numLandau; ++i) {
        fitFunc->SetParameter(i, histogramAmpl * (1.0 + 0.01 * (i + 1)));
        fitFunc->SetParameter(i + 1, histogramMean - histogramSigma * (0.5 - 0.1 * i));
        fitFunc->SetParameter(i + 2, histogramSigma * (0.75 - 0.1 * i));

        fitFunc->SetParLimits(i, 0.05, 200);                                           // Amplitude
        fitFunc->SetParLimits(i + 1, histogramMean - histogramSigma * (1.5 - 0.1 * i), histogramMean + histogramSigma * (1.1 - 0.1 * i));  // Mean
        fitFunc->SetParLimits(i + 2, histogramSigma * 0.25, histogramSigma);            // Sigma

        std::cout << "Parameter " << i << ": " << fitFunc->GetParameter(i) << std::endl;
        std::cout << "Parameter " << i + 1 << ": " << fitFunc->GetParameter(i + 1) << std::endl;
        std::cout << "Parameter " << i + 2 << ": " << fitFunc->GetParameter(i + 2) << std::endl;
    }

    return fitFunc;
}
