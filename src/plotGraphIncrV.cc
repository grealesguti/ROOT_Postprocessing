#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootDataFromFolder.cc"

void plotRootScatterGraph(const std::string& folder, const std::string& treeName1, const std::string& branchName1,
                          const std::string& treeName2, const std::string& branchName2, const std::string& plotName,
                          double xMin = -30, double xMax = 30, double yMin = -1, double yMax = 6000, const std::string& saveFolder = ".")
{
    // Read data from root file
    std::vector<double> data1 = readRootDataFromFolder(folder, treeName1, branchName1);
    std::vector<double> data2 = readRootDataFromFolder(folder, treeName2, branchName2);

    // Check that data is of same size
    if (data1.size() != data2.size())
        return;

    // Create graph
    TGraph* graph = new TGraph(data1.size());
    for (int i = 0; i < data1.size(); i++)
		if(data2[i]>0){graph->SetPoint(i, data1[i], data2[i]);}

    // Set graph style
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.5);
    graph->SetMarkerColor(kBlue);
    //graph->GetXaxis()->SetTitle(branchName1.c_str());
    //graph->GetYaxis()->SetTitle(branchName2.c_str());
    graph->GetXaxis()->SetTitle("X");
    graph->GetYaxis()->SetTitle("Y");
    graph->GetXaxis()->SetRangeUser(xMin, xMax);
    graph->GetYaxis()->SetRangeUser(yMin, yMax);

    // Create canvas and draw graph
    TCanvas* canvas = new TCanvas("canvas", "", 800, 600);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->SetTopMargin(0.15);
    canvas->SetRightMargin(0.15);
    graph->Draw("AP");

    // Add legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->SetHeader("Trees");
    legend->AddEntry(graph, (treeName1 + " - " + branchName1 + " vs " + treeName2 + " - " + branchName2).c_str());
    
    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    canvas->SaveAs((saveFolder + plotName + ".pdf").c_str());

    delete canvas;
    delete graph;
}
