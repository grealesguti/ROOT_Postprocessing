#include "TH1.h"
#include <TMath.h>
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootDataFromFolder.cc"

void plotHistRootData_LC(const std::string& folder, const std::string& treeName, const std::string& branchName, const std::string& plotName, double xMin = -1, double xMax = -1, int nBins = 100, const std::string& saveFolder = ".")
{
	
	
    // Read data from root file
    std::vector<double> data = readRootDataFromFolder(folder, treeName, branchName);

	// Assuming you have already read your data into a vector called "data"
	int nEvents = data.size();

	// Calculate the number of bins as the square root of the number of events
	int nBins_c = TMath::Sqrt(nEvents);

    // Create histogram
    TH1D* hist = new TH1D("hist", "", nBins_c, xMin, xMax);
    for (double d : data)
        hist->Fill(d);

    // Set histogram style
    hist->SetLineWidth(2);
    hist->SetLineColor(kBlue);
    //hist->GetXaxis()->SetTitle("X");
    //hist->GetYaxis()->SetTitle("Y");
    //hist->SetStats(0);
    
    Double_t factor = 1.;
    //Double_t mlo=hist->GetMaximum();
    Double_t mlo=hist->GetEntries();
    hist->Scale(factor/mlo);
    hist->SetXTitle("Light Collection [ph]");
    hist->SetYTitle("Relative frequency");
    //hist->SetMarkerStyle(kMultiply);
    //hist->SetLineWidth(2);
    //hist->SetLineColorAlpha(kAzure, 0.7);

	
    // Create canvas
    TCanvas* c = new TCanvas("c", "", 800, 600);
    c->SetTopMargin(0.05);
    c->SetBottomMargin(0.13);
    c->SetLeftMargin(0.16);
    c->SetRightMargin(0.05);
    // Fit to a Gaussian
    //hist->Fit("poisson");
    // Draw histogram
    hist->Draw("HISTO SAME E1");
//TF1* fitFuncexp = new TF1("fitFunc", "expo", xMin, xMax);
//hist->Fit(fitFuncexp, "R");
TF1* fitFunc = new TF1("fitFunc", "landau", xMin, xMax);
hist->Fit(fitFunc, "R");
// Set color of fit line to gray
fitFunc->SetLineColorAlpha(kBlue-2, 0.2);
    //fitFunc->Draw("same");
    
// Define fit function as sum of three Landau functions
TF1* fitFunc3L = new TF1("fitFunc", "[0]*TMath::Landau(x,[1],[2]) + [3]*TMath::Landau(x,[4],[5]) + [6]*TMath::Landau(x,[7],[8])", xMin, xMax);

// Set initial parameter values and ranges for each Landau function
fitFunc3L->SetParameter(0, 100);   // Amplitude of first Landau
fitFunc3L->SetParLimits(0, 0, 1000);
fitFunc3L->SetParameter(1, 10);    // Peak position of first Landau
fitFunc3L->SetParLimits(1, 0, 20);
fitFunc3L->SetParameter(2, 1);     // Width of first Landau
fitFunc3L->SetParLimits(2, 0, 10);

fitFunc3L->SetParameter(3, 50);    // Amplitude of second Landau
fitFunc3L->SetParLimits(3, 0, 500);
fitFunc3L->SetParameter(4, 20);    // Peak position of second Landau
fitFunc3L->SetParLimits(4, 0, 40);
fitFunc3L->SetParameter(5, 2);     // Width of second Landau
fitFunc3L->SetParLimits(5, 0, 10);

fitFunc3L->SetParameter(6, 20);    // Amplitude of third Landau
fitFunc3L->SetParLimits(6, 0, 200);
fitFunc3L->SetParameter(7, 30);    // Peak position of third Landau
fitFunc3L->SetParLimits(7, 0, 60);
fitFunc3L->SetParameter(8, 5);     // Width of third Landau
fitFunc3L->SetParLimits(8, 0, 20);
    
TF1* fitFunc2L = new TF1("fitFunc", "[0]*TMath::Landau(x,[1],[2]) + [3]*TMath::Landau(x,[4],[5]) ", xMin, xMax);
// Set initial parameter values and ranges for each Landau function
fitFunc2L->SetParameter(0, 0.5);   // Amplitude of first Landau
fitFunc2L->SetParLimits(0, 0, 1);
fitFunc2L->SetParameter(1, 7000);    // Peak position of first Landau
fitFunc2L->SetParLimits(1, 0, 15000);
fitFunc2L->SetParameter(2, 3685);     // Width of first Landau
fitFunc2L->SetParLimits(2, 0, 10000);

fitFunc2L->SetParameter(3, 0.2);   // Amplitude of first Landau
fitFunc2L->SetParLimits(3, 0, 5);
fitFunc2L->SetParameter(4, 11000);    // Peak position of first Landau
fitFunc2L->SetParLimits(4, 0, 15000);
fitFunc2L->SetParameter(5, 4000);     // Width of first Landau
fitFunc2L->SetParLimits(5, 0, 10000);

    //hist->Draw("E");
    //hist->SetLineColorAlpha(kBlue, 0.7);
    //hist->SetLineWidth(2);
    //hist->Draw("HIST SAME C ");
    //hist->Fit(fitFunc3L, "Q");
    //fitFunc3L->SetLineColorAlpha(kGreen-2, 0.4);

    //fitFunc3L->Draw("same");
    
    hist->Fit(fitFunc2L, "R");
    fitFunc2L->SetLineColorAlpha(kRed, 0.8);
	fitFunc2L->SetLineStyle(2); // Set the line style to dotted (style 2)

	// Calculate mean and standard deviation
	double mean = fitFunc2L->GetParameter(1);
	double stdDev = fitFunc2L->GetParError(1);
	//hist->SetLegend(nullptr);
	// Calculate the mean
	double meanh = hist->GetMean();

	// Calculate the standard deviation
	double stdDevh = hist->GetRMS();
	// Create legend
	TLegend* legend = new TLegend(0.65, 0.75, 0.95, 0.95);
	legend->SetTextSize(0.035);
	// Add entries to the legend
	legend->AddEntry(hist, Form("Mean: %.2f", meanh), "l");
	legend->AddEntry(hist, Form("StdDev: %.2f", stdDevh), "");
	// Draw the legend
	legend->Draw();
	hist->SetStats(0);
    fitFunc2L->Draw("same");

	hist->SetLabelSize(0.055, "X");
	hist->SetLabelSize(0.055, "Y");
	hist->GetXaxis()->SetTitleSize(0.055); // Increase the x-axis title size
	hist->GetYaxis()->SetTitleSize(0.055); // Increase the x-axis title size

    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    c->SaveAs((saveFolder + plotName + ".pdf").c_str());
    c->SaveAs((saveFolder + plotName + ".tex").c_str());

    delete c;
    delete hist;
}
