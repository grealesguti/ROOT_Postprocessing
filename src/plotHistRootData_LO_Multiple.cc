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
	
    // Create histograms
    TH1D* hist1 = new TH1D("hist", "", nBins, xMin, xMax);
    for (double d : data1)
        hist1->Fill(d);
    TH1D* hist2 = new TH1D("hist", "", nBins, xMin, xMax);
    for (double d : data2)
        hist2->Fill(d);
    
    // Scale histograms
    Double_t factor = 1.;
    Double_t mlo1=hist1->GetMaximum();
    hist1->Scale(factor/mlo1);
    Double_t mlo2=hist2->GetMaximum();
    hist2->Scale(factor/mlo2);
    // Set histogram styles
    hist1->SetXTitle("Light Output [ph/MeV]");
    hist1->SetYTitle("Intensity");
    hist1->SetMarkerStyle(kMultiply);
    hist1->SetLineWidth(2);
    hist1->SetLineColorAlpha(kGray+1,.7);
    hist1->SetLineStyle(2);

    hist2->SetMarkerStyle(kMultiply);
    hist2->SetLineWidth(2);
    hist2->SetLineColorAlpha(kGray+2.5,.7);
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
    hist1->Draw("E");
    hist1->SetLineColor(kGray+1);
    hist1->SetLineWidth(2);
    hist1->Draw("HIST SAME C ");
  
    hist2->Draw("E SAME");
    hist2->SetLineColor(kGray+2.5);
    hist2->SetLineWidth(2);
    hist2->Draw("HIST SAME C ");
    
	// legend
    TLegend *legend = new TLegend(0.75, 0.7, 0.98, 0.98);
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
    legend->Draw();
   
    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    c->SaveAs((saveFolder + plotName + ".pdf").c_str());

    delete c;
    delete hist1;
    delete hist2;

}
