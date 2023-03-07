#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include "src/plotHistRootData2Dependence_LC.cc"
#include "src/getCurrentDateTimeString.cc"

void plotHistEndOfEventDetectedHits(const std::string& folder, double xMin = 0, double xMax = 12000, int nBins = 100, const std::string& saveFolder = "Figures/DetectedHits/")
{
    // Get current date and time
	std::string currentDateTime = getCurrentDateTimeString();
	std::cout << currentDateTime << std::endl;

    // Plot data from EndOfEvent tree
    std::cout << "Data folder: "<< folder << std::endl;
    std::cout << "Hist values: "<< xMin << " " << xMax << " " << nBins << std::endl;

    std::cout << "Save folder: "<< saveFolder << std::endl;
    plotHistRootData2Dependence_LC("../TierIIData/2023_02/"+folder, "EndOfEvent", "fLOapprox","EndOfEvent", "fEdep", "EndOfEvent_fDetectedHits_" + folder + currentDateTime, xMin , xMax , nBins, saveFolder);
}
