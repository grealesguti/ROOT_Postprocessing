#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include "src/plotHistRootData_LO_Multiple.cc"
#include "src/getCurrentDateTimeString.cc"

void plotHistEndOfEventEdepLOMultiple(const std::string& folder1,const std::string& folder2, double xMin = 0, double xMax = 2500, int nBins = 100, const std::string& saveFolder = "Figures/LOHistMult/")
{
    // Get current date and time
	std::string currentDateTime = getCurrentDateTimeString();
	std::cout << currentDateTime << std::endl;

    // Plot data from EndOfEvent tree
    std::cout << "Data folder: "<< folder1 << std::endl;
    std::cout << "Data folder: "<< folder2 << std::endl;
    std::cout << "Hist values: "<< xMin << " " << xMax << " " << nBins << std::endl;

    std::cout << "Save folder: "<< saveFolder << std::endl;
    plotHistRootData_LO_Multiple("../TierIIData/2023_02/"+folder1, "../TierIIData/2023_02/"+folder2, "EndOfEvent", "fLOapprox", "EndOfEvent_fLOMult_" + folder1+"_"+folder2 + currentDateTime, xMin , xMax , nBins, saveFolder);
}
