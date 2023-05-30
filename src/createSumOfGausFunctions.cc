#include "TF1.h"
#include "TH1.h"

TF1* createSumOfGausFunctions(const TH1D* histogram, double xMin, double xMax)
{
    // Create the TF1 object for the sum of Gaussian functions
    TF1* fitFunc = new TF1("fitFunc", "[0]*TMath::Gaus(x, [1], [2]) + [3]*TMath::Gaus(x, [4], [5])", xMin, xMax);

    // Calculate the average value and sigma of the histogram
    double histogramAmpl = histogram->GetMaximum();
    std::cout << "histogramAmpl:" << histogramAmpl <<std::endl;
    double histogramMean = histogram->GetMean();
    double histogramSigma = histogram->GetRMS();

    // Set initial parameter values and ranges for the second Gaussian function
    fitFunc->SetParameters(histogramAmpl, histogramMean-histogramSigma/2, histogramSigma,histogramAmpl*0.7, histogramMean+histogramSigma/2, histogramSigma);
    fitFunc->SetParLimits(0, histogramAmpl*0.5, 1);     // Amplitude of the first Gaussian
    fitFunc->SetParLimits(1, histogramMean-histogramSigma*1.5, histogramMean);  // Mean of the first Gaussian
    fitFunc->SetParLimits(2, histogramSigma*0.1, histogramSigma*1.5);     // Sigma of the first Gaussian
    fitFunc->SetParLimits(3, histogramAmpl*0.1, 1);     // Amplitude of the second Gaussian
    fitFunc->SetParLimits(4, histogramMean, histogramMean+histogramSigma*1.5);  // Mean of the second Gaussian
    fitFunc->SetParLimits(5, histogramSigma*0.25, histogramSigma);     // Sigma of the second Gaussian

    // Print the parameters
    std::cout << "Parameters set to the sum of Gaussian functions:" << std::endl;
    for (int i = 0; i < fitFunc->GetNpar(); ++i) {
        std::cout << "Parameter " << i << ": " << fitFunc->GetParameter(i) << std::endl;
    }

    return fitFunc;
}
