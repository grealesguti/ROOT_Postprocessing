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

#include <vector>
#include "readRootDataFromFolder.cc"

void plotRootScatterGraph_ZvsLOppt(const std::string& folder="../../TierIIData/2023_02/Final_NSGAII_375x3", 
							const std::string& treeName1="EndOfEvent", const std::string& branchName1="fPrimaryZ",
							const std::string& treeName2="EndOfEvent", const std::string& branchName2="fphL", 
							const std::string& treeName3="EndOfEvent", const std::string& branchName3="fphR", 
							const std::string& plotName="EndOfEvent_fPrimaryZ_vs_fLO_",
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
    // Create graph
	 
    // Create canvas and draw graph
    TCanvas* canvas = new TCanvas("canvas", "", 2000, 600);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->SetTopMargin(0.15);
    canvas->SetRightMargin(0.15);
	
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
			meanValues[i] = std::accumulate(valuesInRange.begin(), valuesInRange.end(), 0.0) / valuesInRange.size();
			double squaredSum = std::accumulate(valuesInRange.begin(), valuesInRange.end(), 0.0,
				[](double acc, double val) { return acc + (val * val); });
			errors[i] = std::sqrt((squaredSum / valuesInRange.size()) - (meanValues[i] * meanValues[i]));
		}
	}

// Print meanValues
std::cout << "Mean Values:" << std::endl;
for (int i = 0; i < meanValues.size(); i++) {
    std::cout << "Range " << i + 1 << ": " << meanValues[i] << std::endl;
}

// Print errors
std::cout << "Errors:" << std::endl;
for (int i = 0; i < errors.size(); i++) {
    std::cout << "Range " << i + 1 << ": " << errors[i] << std::endl;
}


TGraphErrors* graph3 = new TGraphErrors(n);

for (int i = 0; i < n; i++) {
    double x = xMin + (i + 0.5) * rangeSize;
    double y = meanValues[i];
    double error = errors[i];
    std::cout << "X val:"<< i<<" - "  << x  << std::endl;

    graph3->SetPoint(i, x, y);
    graph3->SetPointError(i, rangeSize / 2, error);
}



// Set graph style
graph3->SetMarkerStyle(20);
graph3->SetMarkerSize(1.5);
graph3->SetMarkerColor(kBlue);
graph3->SetLineColor(kBlue); // Set line color to blue
graph3->SetFillColor(kBlue); // Set fill color of error bars to blue

graph3->GetXaxis()->SetRangeUser(xMin, xMax);
graph3->GetYaxis()->SetRangeUser(yMin, yMax);
graph3->GetXaxis()->SetTitle("Impact Z Position [mm]");
graph3->GetYaxis()->SetTitle("Light Collection [ph]");

    TF1 *pol2 = new TF1("pol2","[0]+[1]*x+[2]*x*x",0.03,+0.03);
	pol2->SetLineColor(kBlue);
    graph3->Fit("pol2");

// Draw graph with error bars
graph3->Draw("AP");

// Add legend and save plot
// ...


    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    canvas->SaveAs((saveFolder + plotName + ".pdf").c_str());
    canvas->SaveAs((saveFolder + plotName + ".tex").c_str());

    delete canvas;
}
