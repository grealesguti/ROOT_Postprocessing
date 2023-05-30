#include "TF1.h"
#include "TH1.h"

TF1* createSumOfLandauFunctions(const TH1D* histogram, double xMin, double xMax)
{
    // Create the TF1 object for the sum of Landau functions
    TF1* fitFunc = new TF1("fitFunc", "[0]/100*TMath::Landau(x, [1], [2]) + [3]/100*TMath::Landau(x, [4], [5])", xMin, xMax);

    // Calculate the average value and sigma of the histogram
    double histogramAmpl = histogram->GetBinContent(histogram->GetMaximumBin()) / histogram->GetBinWidth(histogram->GetMaximumBin());
    std::cout << "histogramAmpl:" << histogramAmpl <<std::endl;
    double histogramMean = histogram->GetMean();
    std::cout << "histogramMean:" << histogramMean <<std::endl;
    double histogramSigma = histogram->GetRMS();
    std::cout << "histogramSigma:" << histogramSigma <<std::endl;

    // Set initial parameter values and ranges for the second Landau function
    fitFunc->SetParameters(histogramAmpl*1.01, histogramMean-histogramSigma*0.5, histogramSigma*0.75, histogramAmpl*0.5, histogramMean+histogramSigma*0.5, histogramSigma*0.5);
    fitFunc->SetParLimits(0, 0.05, 200);     // Amplitude of the first Landau
    fitFunc->SetParLimits(1, histogramMean-histogramSigma*1.5, histogramMean+histogramSigma*1.1);  // Mean of the first Landau
    fitFunc->SetParLimits(2, histogramSigma*0.05, histogramSigma*3);     // Sigma of the first Landau
    fitFunc->SetParLimits(3, 0.05, 200);     // Amplitude of the second Landau
    fitFunc->SetParLimits(4, histogramMean-histogramSigma*0.1, histogramMean+histogramSigma*1.5);  // Mean of the second Landau
    fitFunc->SetParLimits(5, histogramSigma*0.05, histogramSigma*3);     // Sigma of the second Landau

    // Print the parameters
    std::cout << "Parameters set to the sum of Landau functions:" << std::endl;
    for (int i = 0; i < fitFunc->GetNpar(); ++i) {
        std::cout << "Parameter " << i << ": " << fitFunc->GetParameter(i) << std::endl;
    }

    return fitFunc;
}


/*
 * Estimate the peak position: Identify the approximate peak position of the histogram. This can be done visually by locating the highest bin or using a peak-finding algorithm.

Determine the bin with the maximum count: Find the bin with the highest count in the histogram. This bin can correspond to the peak of the Landau distribution.

Obtain the count value at the peak bin: Retrieve the count value at the bin with the maximum count. This value represents an estimate of the maximum count in the Landau distribution.

Normalize the count value: Divide the count value obtained in step 3 by the bin width to obtain an estimate of the maximum count per unit bin width. This normalized value can serve as an initial estimate for the scaling parameter of the Landau distribution.
 * */
