#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TF1.h"
#include <cmath>   // Include for mathematical functions
#include <numeric> // Include for accumulate and inner_product functions
#include <iostream>
#include <algorithm>
#include "TROOT.h"
#include <fstream>



#include "TCanvas.h"
#include "TBox.h"
#include "TLine.h"

#include <vector>
#include "readRootDataFromFolder.cc"

void plotRootScatterGraph_ZvsLOpptBox(const std::string& folder="../../TierIIData/2023_02/Final_NSGAII_375x3", 
							const std::string& treeName1="EndOfEvent", const std::string& branchName1="fPrimaryZ",
							const std::string& treeName2="EndOfEvent", const std::string& branchName2="fphL", 
							const std::string& treeName3="EndOfEvent", const std::string& branchName3="fphR", 
							const std::string& plotName="EndOfEvent_fPrimaryZ_vs_fLC_box_NSGA_",
							double xMin = -29, double xMax = 29, double yMin = 0, 
							double yMax = 6000, const std::string& saveFolder = "../Figures/LOHistMult/")
{
    // Read data from root file
    std::cout << "Reading X data " << std::endl;
    std::vector<double> data1 = readRootDataFromFolder(folder, treeName1, branchName1);
    std::cout << "Reading Y1 data " << std::endl;
    std::vector<double> data2 = readRootDataFromFolder(folder, treeName2, branchName2);
    std::cout << "Reading Y2 data " << std::endl;
    std::vector<double> data3 = readRootDataFromFolder(folder, treeName3, branchName3);

    // Check that data is of same size
    if (data1.size() != data2.size())
        return;
    // Check that data is of same size
    if (data1.size() != data3.size())
        return;


		
	int n = 20; // Number of ranges
	double rangeSize = xMax*2 / n;
    std::vector<double> meanValues(n, 0.0);
	std::vector<double> errors(n, 0.0);
	std::vector<std::vector<double>> quartiles(n, std::vector<double>(3, 0.0));
	std::vector<double> minValues(n, std::numeric_limits<double>::max());
	std::vector<double> maxValues(n, std::numeric_limits<double>::lowest());

    // Create graph
	
	
	for (int i = 0; i < n; i++) {
		double rangeMin = xMin/1000  + i * rangeSize/1000;
		double rangeMax = rangeMin + rangeSize/1000;
    std::cout << "Range min" << rangeMin << std::endl;
    std::cout << "Range max" << rangeMax << std::endl;

		std::vector<double> valuesInRange;

		for (int j = 0; j < data1.size(); j++) {
			if (data1[j] >= rangeMin && data1[j] < rangeMax) {
				if (data2[j] > 0 && data3[j] > 0) {
					valuesInRange.push_back((data3[j] + data2[j]) / 2);
				}
			}
		}

		
		if (!valuesInRange.empty()) {
			
					std::sort(valuesInRange.begin(), valuesInRange.end());

		int size = valuesInRange.size();
		int lowerIndex = size * 0.25;
		int medianIndex = size * 0.5;
		int upperIndex = size * 0.75;

		quartiles[i][0] = valuesInRange[lowerIndex];
		quartiles[i][1] = valuesInRange[medianIndex];
		quartiles[i][2] = valuesInRange[upperIndex];
			
			meanValues[i] = std::accumulate(valuesInRange.begin(), valuesInRange.end(), 0.0) / valuesInRange.size();
			double squaredSum = std::accumulate(valuesInRange.begin(), valuesInRange.end(), 0.0,
				[](double acc, double val) { return acc + (val * val); });
			errors[i] = std::sqrt((squaredSum / valuesInRange.size()) - (meanValues[i] * meanValues[i]));
			
        minValues[i] = valuesInRange.front();
        maxValues[i] = valuesInRange.back();
		}

	}
	
	
// Specify the file path where you want to save the CSV
std::string filePath = (saveFolder + plotName + "_quartile_data.csv").c_str();

// Create a file stream object
std::ofstream file(filePath);
std::cout << "### Starting csv " << filePath << std::endl;

// Check if the file was successfully opened
if (file.is_open()) {
    // Write the quartile data to the CSV file
    file << "Range Min,Range Max,Lower Quartile,Median,Upper Quartile,Mean,Std,Min,Max" << std::endl;
    for (int i = 0; i < n; i++) {
        double rangeMin = xMin / 1000 + i * rangeSize / 1000;
        double rangeMax = rangeMin + rangeSize / 1000;

        file << rangeMin << "," << rangeMax << ","
            << quartiles[i][0] << "," << quartiles[i][1] << "," << quartiles[i][2] << ","
            << meanValues[i] << "," << errors[i] << ","
            << minValues[i] << "," << maxValues[i] << std::endl;
    }

    // Close the file stream
    file.close();
    std::cout << "Data saved to " << filePath << std::endl;
} else {
    std::cout << "Error opening the file." << std::endl;
}


// Print meanValues
std::cout << "median:" << std::endl;
for (int i = 0; i < meanValues.size(); i++) {
    std::cout << "Range " << i + 1 << ": " << quartiles[i][1] << std::endl;
}

    // Create canvas and draw graph
    TCanvas* canvas = new TCanvas("canvas", "", 800, 600);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->SetTopMargin(0.15);
    canvas->SetRightMargin(0.15);

for (int i = 0; i < n; i++) {
    double x = xMin + (i + 0.5) * rangeSize;
    double y = quartiles[i][1];  // Use the median as the y-coordinate
    double lower = quartiles[i][0];  // Lower quartile
    double upper = quartiles[i][2];  // Upper quartile

    TBox* box = new TBox(x - rangeSize / 2, lower, x + rangeSize / 2, upper);
    box->SetFillColor(kBlue);  // Set fill color of the box
    box->SetLineColor(kBlack);  // Set outline color of the box
    box->SetLineWidth(1);  // Set line width of the box
    box->Draw("AP" 	);

    // Draw a horizontal line at the median
    TLine* medianLine = new TLine(x - rangeSize / 2, y, x + rangeSize / 2, y);
    medianLine->SetLineColor(kRed);  // Set line color of the median line
    medianLine->SetLineWidth(2);  // Set line width of the median line
    medianLine->Draw("AP");
}


    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    canvas->SaveAs((saveFolder + plotName + ".pdf").c_str());
    canvas->SaveAs((saveFolder + plotName + ".tex").c_str());

}
