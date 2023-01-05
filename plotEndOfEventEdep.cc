#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include "src/plotHistRootData.cc"
#include "src/getCurrentDateTimeString.cc"

void plotEndOfEventEdep(const std::string& folder, double xMin = -1, double xMax = -1, int nBins = 100, const std::string& saveFolder = "Figures/Other/")
{
    // Get current date and time
	std::string currentDateTime = getCurrentDateTimeString();
	std::cout << currentDateTime << std::endl;

    // Plot data from EndOfEvent tree
    std::cout << "Data folder: "<< folder << std::endl;
    std::cout << "Hist values: "<< xMin << " " << xMax << " " << nBins << std::endl;

    std::cout << "Save folder: "<< saveFolder << std::endl;
    plotHistRootData("../TierIIData/"+folder, "EndOfEvent", "fEdep", "EndOfEvent_fEdep_" + folder + currentDateTime, xMin , xMax , nBins, saveFolder);
}
