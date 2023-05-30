#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootDataFromFolder.cc"

void plotHistRootData_LC_Multiple(const std::string& folder1="../../TierIIData/2023_02/Rst_FL_Muon_23-02-10_SiPM375", const std::string& folder2="../../TierIIData/2023_02/Final_NSGAII_375x3", const std::string& folder3="../../TierIIData/2023_02/Final_NSGAII_375xW", const std::string& folder4="../../TierIIData/2023_02/Final_NSGAII_6xW" , const std::string& treeName="EndOfEvent", const std::string& branchName="fHits", const std::string& plotName="EndOfEvent_fLCMult", double xMin = 0, double xMax = 35000, int nBins = 100, const std::string& saveFolder = "../Figures/LOHistMult/")
{
    // Read data from root files
    std::vector<double> data1 = readRootDataFromFolder(folder1, treeName, branchName);
	std::vector<double> data2 = readRootDataFromFolder(folder2, treeName, branchName);
	std::vector<double> data3 = readRootDataFromFolder(folder3, treeName, branchName);
	std::vector<double> data4 = readRootDataFromFolder(folder4, treeName, branchName);
    // Create histograms
	int nEvents = data1.size();
	int nBins_c = TMath::Sqrt(nEvents);
    TH1D* hist1 = new TH1D("hist", "", nBins_c, xMin, xMax);
    for (double d : data1)
        hist1->Fill(d);
	nEvents = data2.size();
	nBins_c = TMath::Sqrt(nEvents);
	TH1D* hist2 = new TH1D("hist", "", nBins_c, xMin, xMax);
    for (double d : data2)
        hist2->Fill(d);
	nEvents = data3.size();
	nBins_c = TMath::Sqrt(nEvents);
    TH1D* hist3 = new TH1D("hist", "", nBins_c, xMin, xMax);
    for (double d : data3)
        hist3->Fill(d);
	nEvents = data4.size();
	nBins_c = TMath::Sqrt(nEvents);
    TH1D* hist4 = new TH1D("hist", "", nBins_c, xMin, xMax);
    for (double d : data4)
        hist4->Fill(d);
    // Scale histograms
    Double_t factor = 1.;
    Double_t mlo1=hist1->GetEntries();
    hist1->Scale(factor/mlo1);
    Double_t mlo2=hist2->GetEntries();
    hist2->Scale(factor/mlo2);
    Double_t mlo3=hist3->GetEntries();
    hist3->Scale(factor/mlo3);
    Double_t mlo4=hist4->GetEntries();
    hist4->Scale(factor/mlo4);
    // Set histogram styles
    hist1->SetXTitle("Light Collection [ph]");
    hist1->SetYTitle("Intensity");
    hist1->SetMarkerStyle(kMultiply);
    hist1->SetLineColorAlpha(kGray+1,.8);
    hist1->SetLineStyle(1);

    hist2->SetMarkerStyle(kMultiply);
    hist2->SetLineColorAlpha(kGray+2.5,.8);
    hist2->SetLineStyle(1);

    hist3->SetMarkerStyle(kMultiply);
    hist3->SetLineColorAlpha(kGray+2.5,.8);
    hist3->SetLineStyle(1);
    
    hist4->SetMarkerStyle(kMultiply);
    hist4->SetLineColorAlpha(kGray+2.5,.8);
    hist4->SetLineStyle(1);
    // Avoid standard hist std and mean writting
	hist1->SetStats(kFALSE);
	hist2->SetStats(kFALSE);
	hist3->SetStats(kFALSE);
	hist4->SetStats(kFALSE);
    // Create canvas
    TCanvas* c = new TCanvas("c", "", 800, 600);
    c->SetTopMargin(0.05);
    c->SetBottomMargin(0.13);
    c->SetLeftMargin(0.16);
    c->SetRightMargin(0.05);

    // Draw histogram
    //hist1->Draw("E");
    hist1->SetLineColor(kGreen+2);
    hist1->SetLineWidth(2);
    //hist1->SetFillColorAlpha(kBlue,0.7);
    hist1->Draw("HISTO SAME ");
  
    //hist2->Draw("E SAME");
    hist2->SetLineColor(kBlack);
    hist2->SetLineWidth(2);
    //hist2->SetFillColorAlpha(kRed,0.7);
    hist2->Draw("HISTO SAME ");
    
    //hist3->Draw("E SAME");
    hist3->SetLineColor(kBlue+1);
    hist3->SetLineWidth(2);
    //hist3->SetFillColorAlpha(kBlue+2,0.7);
    hist3->Draw("HISTO SAME  ");
    
    //hist4->Draw("E SAME");
    hist4->SetLineColor(kRed+2);
    hist4->SetLineWidth(2);
    //hist4->SetFillColorAlpha(kRed+2,0.7);
    hist4->Draw("HISTO SAME ");
	// legend
    TLegend *legend = new TLegend(0.75, 0.5, 0.98, 0.98);
		// same line histogram writting
    //legend->AddEntry(hist1, Form("Na (mean=%.2f, std=%.2f)", hist1->GetMean(), hist1->GetRMS()), "l");
    //legend->AddEntry(hist2, Form("Muon (mean=%.2f, std=%.2f)", hist2->GetMean(), hist2->GetRMS()), "l");
   legend->AddEntry(hist1, "Default - 375x300", "l");
   legend->AddEntry((TObject*)0, Form("Mean: %.2f", hist1->GetMean()), "");
   legend->AddEntry((TObject*)0, Form("Std Dev: %.2f", hist1->GetStdDev()), "");
   
   legend->AddEntry(hist2, "NSGA - 375x300", "l");
   legend->AddEntry((TObject*)0, Form("Mean: %.2f", hist2->GetMean()), "");
   legend->AddEntry((TObject*)0, Form("Std Dev: %.2f", hist2->GetStdDev()), "");
   
   legend->AddEntry(hist3, "NSGA - 375xW", "l");
   legend->AddEntry((TObject*)0, Form("Mean: %.2f", hist3->GetMean()), "");
   legend->AddEntry((TObject*)0, Form("Std Dev: %.2f", hist3->GetStdDev()), "");
   
   legend->AddEntry(hist4, "NSGA - 600xW", "l");
   legend->AddEntry((TObject*)0, Form("Mean: %.2f", hist4->GetMean()), "");
   legend->AddEntry((TObject*)0, Form("Std Dev: %.2f", hist4->GetStdDev()), "");
		//legend->SetFillStyle(0); // Make legend background transparent
    legend->Draw();

    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    c->SaveAs((saveFolder + plotName + ".pdf").c_str());
    c->SaveAs((saveFolder + plotName + ".tex").c_str());

    delete c;
    delete hist1;
    delete hist2;
    delete hist3;
    delete hist4;

}
