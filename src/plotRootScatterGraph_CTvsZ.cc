#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootCTDataFromFolder.cc"


void plotRootScatterGraph_CTvsZ(const std::string& folder, const std::string& plotName,
                          double xMin = -30, double xMax = 30, double yMin = -1, double yMax = 2000, const std::string& saveFolder = ".")
{
    // Read data from root file

	auto [CTevt, CTevtDiv, CTevtDiv_R, fPrimaryZ] = readRootCTDataFromFolder(folder);

    // Create graph
    TGraph* graph = new TGraph(fPrimaryZ.size());
    for (int i = 0; i < fPrimaryZ.size(); i++)
		if(CTevt[i]>0){graph->SetPoint(i, fPrimaryZ[i], CTevt[i]);}

    // Set graph style
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.5);
    graph->SetMarkerColor(kGray);
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
    
    // Add quadratic fit
    TF1* fit = new TF1("fit", "[0] + [1]*x + [2]*x*x", xMin, xMax);
    fit->SetLineColor(kRed);
    fit->SetParameters(100, 10, 1);
    graph->Fit(fit, "QR");
    
    // Add legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->SetHeader("Trees");
    legend->AddEntry(graph, "Data", "p");
    legend->AddEntry(fit, "Quadratic Fit", "l");
    legend->Draw();
    
    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    canvas->SaveAs((saveFolder + plotName + ".pdf").c_str());
    delete canvas;
    delete graph;
    // Create graph
    TGraph* graph1 = new TGraph(fPrimaryZ.size());
    for (int i = 0; i < fPrimaryZ.size(); i++)
		if(CTevtDiv[i]>0){graph1->SetPoint(i, fPrimaryZ[i], CTevtDiv[i]);}

    // Set graph style
    graph1->SetMarkerStyle(20);
    graph1->SetMarkerSize(1.5);
    graph1->SetMarkerColor(kGray);
    graph1->GetXaxis()->SetTitle("Impact Z Position [mm]");
    graph1->GetYaxis()->SetTitle("Ph. of neighbour by Ph. of main channel [a.u.]");

    // Create canvas and draw graph
    TCanvas* canvas1 = new TCanvas("canvas", "", 800, 600);
    canvas1->SetLeftMargin(0.15);
    canvas1->SetBottomMargin(0.15);
    canvas1->SetTopMargin(0.15);
    canvas1->SetRightMargin(0.15);
    graph1->Draw("AP");
    graph1->GetXaxis()->SetRangeUser(xMin, xMax);
    graph1->GetYaxis()->SetRangeUser(yMin, 1);
    
    // Add quadratic fit
    TF1* fit1 = new TF1("fit", "[0] + [1]*x + [2]*x*x", xMin, xMax);
    fit1->SetLineColor(kRed);
    fit1->SetParameters(100, 10, 1);
    graph1->Fit(fit, "QR");
    
    // Add legend
    TLegend* legend1 = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend1->SetHeader("Trees");
    legend1->AddEntry(graph, "Data", "p");
    legend1->AddEntry(fit, "Quadratic Fit", "l");
    legend1->Draw();
    
    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + "_DivL" + ".pdf" << std::endl;

    canvas1->SaveAs((saveFolder + plotName + "_DivL"+".pdf").c_str());
    std::cout << "Saved" << std::endl;

    
    delete canvas1;
    delete graph1;
    
    // Create graph
    TGraph* graph2 = new TGraph(fPrimaryZ.size());
    for (int i = 0; i < fPrimaryZ.size(); i++)
		if(CTevtDiv[i]>0){graph2->SetPoint(i, fPrimaryZ[i], CTevtDiv_R[i]);}

    // Set graph style
    graph2->SetMarkerStyle(20);
    graph2->SetMarkerSize(1.5);
    graph2->SetMarkerColor(kGray);
    graph2->GetXaxis()->SetTitle("Impact Z Position [mm]");
    graph2->GetYaxis()->SetTitle("Ph. of neighbour by Ph. of opposite channel [a.u.]");

    // Create canvas and draw graph
    TCanvas* canvas2 = new TCanvas("canvas", "", 800, 600);
    canvas2->SetLeftMargin(0.15);
    canvas2->SetBottomMargin(0.15);
    canvas2->SetTopMargin(0.15);
    canvas2->SetRightMargin(0.15);
    graph2->Draw("AP");
    graph2->GetXaxis()->SetRangeUser(xMin, xMax);
    graph2->GetYaxis()->SetRangeUser(yMin, 1);
    // Add legend
    TLegend* legend2 = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend2->SetHeader("Trees");
    
    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + "_DivR" + ".pdf" << std::endl;

    canvas2->SaveAs((saveFolder + plotName +"_DivR"+ ".pdf").c_str());
    canvas2->SaveAs((saveFolder + plotName +"_DivR"+ ".tex").c_str());

    delete canvas2;
    delete graph2;
	
}

