#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootCTDataFromFolder.cc"

void plotHistRootData_CTX(const std::string& folder, const std::string& plotName, 
							double xMin = -1, double xMax = -1, int nBins = 100, 
							const std::string& saveFolder = ".")
{
    // Read data from root file
	auto [ctl1, ctl1Div_L,ctl1Div_R, gunZ] = readRootCTDataFromFolder(folder);
    
    
    // Create histogram
    TH1D* hist_L = new TH1D("hist", "", nBins, xMin, xMax);
    for (double d : ctl1Div_L)
        hist_L->Fill(d);
        
    TH1D* hist_R = new TH1D("hist", "", nBins, xMin, xMax);
    for (double d : ctl1Div_R)
        hist_R->Fill(d);

    Double_t factor = 1.;
    Double_t mlo=hist_L->GetMaximum();
    hist_L->Scale(factor/mlo);
    hist_L->SetXTitle("Light Collection [ph]");
    hist_L->SetYTitle("Intensity");
    hist_L->SetMarkerStyle(kMultiply);
    hist_L->SetLineWidth(2);
    hist_L->SetLineColorAlpha(kGray, 0.7);
   
    Double_t mlo1=hist_R->GetMaximum();
    hist_R->Scale(factor/mlo1);
    hist_R->SetXTitle("Light Collection [ph]");
    hist_R->SetYTitle("Intensity");
    hist_R->SetMarkerStyle(kMultiply);
    hist_R->SetLineWidth(2);
    hist_R->SetLineColorAlpha(kGray+2, 0.7);
    
    // Create canvas
    TCanvas* c = new TCanvas("c", "", 800, 600);
    c->SetTopMargin(0.05);
    c->SetBottomMargin(0.13);
    c->SetLeftMargin(0.16);
    c->SetRightMargin(0.05);

    // Draw histogram
    /*hist_L->Draw("E");
    hist_L->SetLineColorAlpha(kGray, 0.7);
    hist_L->SetLineWidth(2);
    hist_L->Draw("HIST SAME C ");
    */
    //hist_R->Draw("E SAME");
    hist_R->Draw("E");
    hist_R->SetLineColorAlpha(kGray+2, 0.7);
    hist_R->SetLineWidth(2);
    hist_R->Draw("HIST SAME C ");
    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    c->SaveAs((saveFolder + plotName + ".pdf").c_str());
    c->SaveAs((saveFolder + plotName + ".tex").c_str());

    delete c;
    delete hist_L;
    delete hist_R;

}

