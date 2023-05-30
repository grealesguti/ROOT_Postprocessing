#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootDataFromFolder.cc"

void plotHistRootData_LO_Multiple(const std::string& folder1, const std::string& folder2, const std::string& treeName, const std::string& branchName, const std::string& plotName, double xMin = -1, double xMax = -1, int nBins = 100, const std::string& saveFolder = ".")
{
    // Read data from root files
    std::vector<double> data1 = readRootDataFromFolder(folder1, treeName, branchName);
	std::vector<double> data2 = readRootDataFromFolder(folder2, treeName, branchName);
	
	// Assuming you have already read your data into a vector called "data"
	int nEvents = data1.size();
	// Calculate the number of bins as the square root of the number of events
	int nBins_c = TMath::Sqrt(nEvents);
	
    // Create histograms
    TH1D* hist1 = new TH1D("hist", "", nBins_c, 0, xMax);
    for (double d : data1)
        hist1->Fill(d);
        
	// Assuming you have already read your data into a vector called "data"
	nEvents = data2.size();
	// Calculate the number of bins as the square root of the number of events
	nBins_c = TMath::Sqrt(nEvents);
    TH1D* hist2 = new TH1D("hist", "", nBins_c, 0, xMax);
    for (double d : data2)
        hist2->Fill(d);
    
    // Scale histograms
    Double_t factor = 1.;
    Double_t mlo1=hist1->GetEntries();
    hist1->Scale(factor/mlo1);
    Double_t mlo2=hist2->GetEntries();
    hist2->Scale(factor/mlo2);
    
	hist1->SetLabelSize(0.035, "X");
	hist1->SetLabelSize(0.035, "Y");
	hist1->GetXaxis()->SetTitleSize(0.055); // Increase the x-axis title size
	hist1->GetYaxis()->SetTitleSize(0.055); // Increase the x-axis title size
    
    // Set histogram styles
    hist1->SetXTitle("Light Output [ph/MeV]");
    hist1->SetYTitle("Relative Frequency");
    hist1->SetMarkerStyle(kMultiply);
    hist1->SetLineWidth(2);
    hist1->SetLineColorAlpha(kBlue+1,.7);
    hist1->SetLineStyle(1);

    hist2->SetMarkerStyle(kMultiply);
    hist2->SetLineWidth(2);
    hist2->SetLineColorAlpha(kRed+2.5,.7);
    hist2->SetLineStyle(1);

    // Avoid standard hist std and mean writting
	hist1->SetStats(kFALSE);
	hist2->SetStats(kFALSE);
   
    // Create canvas
    TCanvas* c = new TCanvas("c", "", 800, 600);
    c->SetTopMargin(0.05);
    c->SetBottomMargin(0.13);
    c->SetLeftMargin(0.16);
    c->SetRightMargin(0.05);

    // Draw histogram
    //hist1->Draw("E");
    hist1->SetLineColor(kBlue+1);
    hist1->SetLineWidth(2);
    hist1->Draw("HISTO SAME E1 ");
  
    //hist2->Draw("E SAME");
    hist2->SetLineColor(kRed+2.5);
    hist2->SetLineWidth(2);
    hist2->Draw("HISTO SAME E1 ");
    
	// legend
    TLegend *legend = new TLegend(0.75, 0.55, 0.95, 0.95);
		// same line histogram writting
    //legend->AddEntry(hist1, Form("Na (mean=%.2f, std=%.2f)", hist1->GetMean(), hist1->GetRMS()), "l");
    //legend->AddEntry(hist2, Form("Muon (mean=%.2f, std=%.2f)", hist2->GetMean(), hist2->GetRMS()), "l");
   legend->AddEntry(hist1, "SiPM 3x3.75 Na", "l");
   legend->AddEntry((TObject*)0, Form("Mean: %.2f", hist1->GetMean()), "");
   legend->AddEntry((TObject*)0, Form("Std Dev: %.2f", hist1->GetStdDev()), "");
   legend->AddEntry(hist2, "SiPM 3x3.75 Muon", "l");
   legend->AddEntry((TObject*)0, Form("Mean: %.2f", hist2->GetMean()), "");
   legend->AddEntry((TObject*)0, Form("Std Dev: %.2f", hist2->GetStdDev()), "");
		//legend->SetFillStyle(0); // Make legend background transparent
	legend->SetTextSize(0.045);

    legend->Draw();
	hist1->SetAxisRange(0, 0.28, "Y"); // Set the Y-axis scale from 0 to 50

	TF1* fitFunc = new TF1("fitFunc", "gaus", xMin, xMax);
	hist2->Fit(fitFunc, "R");
	// Set color of fit line to gray
	fitFunc->SetLineColorAlpha(kRed-2, 1);
	//fitFunc->Draw("same");
// Define the composite fit function
TF1* fitFuncG3 = new TF1("fitFunc", "[0]*TMath::Gaus(x, [1], [2]) + [3]*TMath::Gaus(x, [4], [5])", xMin, xMax);

// Set initial parameter values and ranges
fitFuncG3->SetParameters(0.25, 1200, 300, 0.25, 1250, 100, 0.25, 1300, 100);
fitFuncG3->SetParLimits(0, 0, 0.3);     // Amplitude of the first Gaussian
fitFuncG3->SetParLimits(1, 1000, 2000);     // Mean of the first Gaussian
fitFuncG3->SetParLimits(2, 0, 2000);     // Sigma of the first Gaussian
fitFuncG3->SetParLimits(3, 0, 0.3);     // Amplitude of the second Gaussian
fitFuncG3->SetParLimits(4, 1000, 2000);     // Mean of the second Gaussian
fitFuncG3->SetParLimits(5, 0, 2000);     // Sigma of the second Gaussian


// Define the log-normal fit function
TF1* fitFunclN = new TF1("fitFunc", "[0]*TMath::LogNormal(x, [1], [2])", xMin, xMax);

// Set initial parameter values and ranges
fitFunclN->SetParameters(0.2, 1300, 300);
fitFunclN->SetParLimits(0, 0, 1);     // Amplitude of the log-normal function
fitFunclN->SetParLimits(1, 0, 2000);     // Mean of the log-normal function
fitFunc->SetParLimits(2, 0, 800);     // Sigma of the log-normal function


// Fit the histogram with the composite function
hist2->Fit(fitFuncG3);
	fitFuncG3->Draw("same");
hist2->Fit(fitFuncG3);
	fitFuncG3->Draw("same");
    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    c->SaveAs((saveFolder + plotName + ".pdf").c_str());
    c->SaveAs((saveFolder + plotName + ".tex").c_str());

    delete c;
    delete hist1;
    delete hist2;

}
