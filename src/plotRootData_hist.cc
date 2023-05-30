#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootDataFromFolder.cc"

void plotRootData_hist(const std::string& folder, const std::string& treeName, const std::string& branchName, const std::string& plotName)
{
    // Read data from root file
    std::vector<double> data = readRootDataFromFolder(folder, treeName, branchName);

    // Create histogram
    TH1D* hist = new TH1D("hist", "", 100, 0, 100);
    for (double d : data)
        hist->Fill(d);

    // Set histogram style
    hist->SetLineWidth(2);
    hist->SetLineColor(kBlue);
    hist->GetXaxis()->SetTitle("X");
    hist->GetYaxis()->SetTitle("Y");
    hist->SetStats(0);

    // Create canvas
    TCanvas* c = new TCanvas("c", "", 800, 600);
    c->SetTopMargin(0.05);
    c->SetBottomMargin(0.13);
    c->SetLeftMargin(0.16);
    c->SetRightMargin(0.05);

    // Draw histogram
    hist->Draw("HIST");

    // Save plot to pdf file
    c->SaveAs((plotName + ".pdf").c_str());
    c->SaveAs((plotName + ".tex").c_str());

    delete c;
    delete hist;
}
