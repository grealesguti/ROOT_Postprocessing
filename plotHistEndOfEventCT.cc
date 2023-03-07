#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
//#include "src/plotRootScatterGraph.cc"
#include "src/getCurrentDateTimeString.cc"
#include "src/plotHistRootData_CTX.cc"

void plotHistEndOfEventCT(const std::string& folder,double yMax = 1,
									 const std::string& saveFolder = "Figures/CT/")
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

	plotHistRootData_CTX("../TierIIData/2023_02/"+folder,
								"EndOfEvent_fPrimaryZ_vs_fLO_" + folder + currentDateTime,0,1,100,saveFolder);
}
//fPrimaryZ
//fLOapprox
