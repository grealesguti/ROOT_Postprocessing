#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
//#include "src/plotRootScatterGraph.cc"
#include "src/getCurrentDateTimeString.cc"
#include "src/plotRootScatterGraph_ZvsLO.cc"

void plotGraphEndOfEventPrimaryZvsLO(const std::string& folder,double yMax = 12000,
									 const std::string& saveFolder = "Figures/ZvsLO/")
{
	// Get current date and time
	std::string currentDateTime = getCurrentDateTimeString();
	std::cout << currentDateTime << std::endl;

    // Plot data from EndOfEvent tree
    std::cout << "Data folder: "<< folder << std::endl;
    //plotRootScatterGraph("../TierIIData/"+folder, "EndOfEvent", "fPrimaryZ", "EndOfEvent", "fphL", "EndOfEvent_fPrimaryZ_vs_fLO_" + folder + currentDateTime,-29,29,0,yMax,saveFolder);
	plotRootScatterGraph_ZvsLO("../TierIIData/2023_02/"+folder, "EndOfEvent", "fPrimaryZ", "EndOfEvent", "fphL","EndOfEvent", "fphR",
								"EndOfEvent_fPrimaryZ_vs_fLO_" + folder + currentDateTime,-29,29,0,yMax,saveFolder);
}
//fPrimaryZ
//fLOapprox
