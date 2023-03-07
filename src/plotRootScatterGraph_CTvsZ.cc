#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootCTDataFromFolder.cc"
#include "utils.cc"


void plotRootScatterGraph_CTvsZ(const std::string& folder, const std::string& plotName,
                          double xMin = -30, double xMax = 30, double yMin = -1, double yMax = 2000, const std::string& saveFolder = ".")
{
    // Read data from root file

	auto [CTevt, CTevtDiv, fPrimaryZ] = readRootCTDataFromFolder(folder);

    // Create graph
    TGraph* graph = new TGraph(fPrimaryZ.size());
    for (int i = 0; i < fPrimaryZ.size(); i++)
		if(CTevt[i]>0){graph->SetPoint(i, fPrimaryZ[i], CTevt[i]);}

    // Set graph style
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.5);
    graph->SetMarkerColor(kGray);
    //graph->GetXaxis()->SetTitle(branchName1.c_str());
    //graph->GetYaxis()->SetTitle(branchName2.c_str());
    graph->GetXaxis()->SetTitle("Impact Z Position [mm]");
    graph->GetYaxis()->SetTitle("Photons detected by a neighbouring SiPM[ph]");

    // Create canvas and draw graph
    TCanvas* canvas = new TCanvas("canvas", "", 800, 600);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->SetTopMargin(0.15);
    canvas->SetRightMargin(0.15);
    graph->Draw("AP");
    graph->GetXaxis()->SetRangeUser(xMin, xMax);
    graph->GetYaxis()->SetRangeUser(yMin, 2000);
    // Add legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->SetHeader("Trees");
    //legend->AddEntry(graph, (treeName1 + " - " + branchName1 + " vs " + treeName2 + " - " + branchName2).c_str());
    
    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    canvas->SaveAs((saveFolder + plotName + ".pdf").c_str());

    delete canvas;
    delete graph;
}

