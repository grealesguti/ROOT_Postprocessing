#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include "src/plotHistRootData_LC.cc"
#include "src/getCurrentDateTimeString.cc"

void plotHistEndOfEventEdepLC(const std::string& folder, double xMin = 0, double xMax = 25000, int nBins = 50, const std::string& saveFolder = "Figures/LCHist/")
{
    // Get current date and time
	std::string currentDateTime = getCurrentDateTimeString();
	std::cout << currentDateTime << std::endl;

    // Plot data from EndOfEvent tree
    std::cout << "Data folder: "<< folder << std::endl;
    std::cout << "Hist values: "<< xMin << " " << xMax << " " << nBins << std::endl;

    std::cout << "Save folder: "<< saveFolder << std::endl;
    plotHistRootData_LC("../TierIIData/2023_03/"+folder, "EndOfEvent", "fHits", "EndOfEvent_fLC_" + folder + currentDateTime, xMin , xMax , nBins, saveFolder);
}
