#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
//#include "src/plotRootScatterGraph.cc"
#include "src/getCurrentDateTimeString.cc"
#include "src/plotRootScatterGraph_CTvsZ.cc"

void plotGraphEndOfEventCTvsZ(const std::string& folder,double yMax = 12000,
									 const std::string& saveFolder = "Figures/ZvsCT/")
{
	// Get current date and time
	std::string currentDateTime = getCurrentDateTimeString();
	std::cout << currentDateTime << std::endl;

    // Plot data from EndOfEvent tree
    std::cout << "Data folder: "<< folder << std::endl;
    //plotRootScatterGraph("../TierIIData/"+folder, "EndOfEvent", "fPrimaryZ", "EndOfEvent", "fphL", "EndOfEvent_fPrimaryZ_vs_fLO_" + folder + currentDateTime,-29,29,0,yMax,saveFolder);

//void plotRootScatterGraph_CTvsZ(const std::string& folder, const std::string& treeName1, const std::string& branchName1,
//                          const std::string& treeName2, const std::string& branchName2, const std::string& plotName,
//                          double xMin = -30, double xMax = 30, double yMin = -1, double yMax = 6000, const std::string& saveFolder = ".")
	
	plotRootScatterGraph_CTvsZ("../TierIIData/2023_02/"+folder,
								"EndOfEvent_fPrimaryZ_vs_fLO_" + folder + currentDateTime,-29,29,0,yMax,saveFolder);
}
//fPrimaryZ
//fLOapprox
