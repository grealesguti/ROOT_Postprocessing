#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include "src/plotHistRootData_LO.cc"
#include "src/getCurrentDateTimeString.cc"

void plotHistEndOfEventEdepLO(const std::string& folder, double xMin = 0, double xMax = 2500, int nBins = 100, const std::string& saveFolder = "Figures/LOHist/")
{
    // Get current date and time
	std::string currentDateTime = getCurrentDateTimeString();
	std::cout << currentDateTime << std::endl;

    // Plot data from EndOfEvent tree
    std::cout << "Data folder: "<< folder << std::endl;
    std::cout << "Hist values: "<< xMin << " " << xMax << " " << nBins << std::endl;

    std::cout << "Save folder: "<< saveFolder << std::endl;
    plotHistRootData_LO("../TierIIData/2023_02/"+folder, "EndOfEvent", "fLOapprox", "EndOfEvent_fLOapprox_" + folder + currentDateTime, xMin , xMax , nBins, saveFolder);
}
