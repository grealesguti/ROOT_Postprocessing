#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootDataFromFolder.cc"
#include "createSumOfLandauFunctions.cc"
#include "createSumOfGausFunctions.cc"
#include "saveFitResultsToFile.cc"

void plotHistRootData_LC_Single(	const std::string& folder1="../../TierIIData/2023_02/Rst_FL_Muon_23-02-10_SiPM375", 
									const std::string& folder2="../../TierIIData/2023_02/Final_NSGAII_375x3" , 
									const std::string& treeName="EndOfEvent", 
									const std::string& branchName="fHits", 
									const std::string& plotName="EndOfEvent_fLCMult", 
									double xMin = 0, double xMax = 35000, int nBins = 1, 
									const std::string& saveFolder = "../Figures/LOHistMult/",
									const std::string& leg_name = "",const std::string& leg_name2 = "",
									const std::string& x_name = "Light Output [ph/MeV]")
{
	
		std::cout << "folder1: "<< folder1 << std::endl;
		std::cout << "folder1: "<< folder2<< std::endl;

	    std::cout << "Branch: "<< branchName << std::endl;

    // Read data from root files
    std::vector<double> data1 = readRootDataFromFolder(folder1, treeName, branchName);
    		std::cout << "size data 1: "<< data1.size() << std::endl;

	std::vector<double> data2 = readRootDataFromFolder(folder2, treeName, branchName);
			std::cout << "size data 2: "<< data2.size() << std::endl;

    std::vector<double> edep1 = readRootDataFromFolder(folder1, treeName, "fEdep");
	std::vector<double> edep2 = readRootDataFromFolder(folder2, treeName, "fEdep");
	
	// Assuming you have already read your data into a vector called "data"
	int nEvents = data1.size();
	// Calculate the number of bins as the square root of the number of events
	int nBins_c = TMath::Sqrt(nEvents);

		std::cout << "size data 1: "<< data1.size() << std::endl;
		std::cout << "size data 2: "<< data2.size() << std::endl;

    // Create histograms
    TH1D* hist1 = new TH1D("hist", "", nBins_c, xMin, xMax);
	for (int i = 0; i < data1.size(); i++) {
		if (edep1[i] > 0.51 && data1[i]>100) {
			hist1->Fill(data1[i]);
		}
	}
        
	// Assuming you have already read your data into a vector called "data"
	nEvents = data2.size();
	// Calculate the number of bins as the square root of the number of events
	nBins_c = TMath::Sqrt(nEvents);
	        
    TH1D* hist2 = new TH1D("hist", "", nBins_c, xMin, xMax);
	for (int i = 0; i < data2.size(); i++) {
		if (edep2[i] > 0.5 && data2[i]>100) {
			hist2->Fill(data2[i]);
		}
	}
        

    // Scale histograms
    Double_t factor = 1.;
    //Double_t mlo1=hist1->GetMaximum();
    Double_t mlo1=hist1->GetEntries();
    hist1->Scale(factor/mlo1);
    //Double_t mlo2=hist2->GetMaximum();
    Double_t mlo2=hist2->GetEntries();
    hist2->Scale(factor/mlo2);

    // Set histogram styles
    //hist1->SetXTitle("Light Collection [ph]");
    //hist1->SetXTitle("Light Output [ph/MeV]");
    hist1->SetXTitle(x_name.c_str());
    hist1->SetYTitle("Relative Frequency");
    hist1->SetMarkerStyle(kMultiply);
    hist1->SetLineColorAlpha(kGray+1,.8);
    hist1->SetLineStyle(1);

    hist2->SetMarkerStyle(kMultiply);
    hist2->SetLineColorAlpha(kGray+2.5,.8);
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
    hist1->SetLineColor(kBlue+2);
    hist1->SetLineWidth(2);
    //hist1->SetFillColorAlpha(kBlue+2,0.7);
    hist1->Draw("HISTO SAME  ");
  
    //hist2->Draw("E SAME");
    hist2->SetLineColor(kRed+2);
    hist2->SetLineWidth(2);
    //hist2->SetFillColorAlpha(kRed+2,0.7);
    //hist2->Draw("HISTO SAME  ");
    
	// legend
    //TLegend *legend = new TLegend(0.55, 0.55, 0.95, 0.95);
    
    		// same line histogram writting
    //legend->AddEntry(hist1, Form("Na (mean=%.2f, std=%.2f)", hist1->GetMean(), hist1->GetRMS()), "l");
    //legend->AddEntry(hist2, Form("Muon (mean=%.2f, std=%.2f)", hist2->GetMean(), hist2->GetRMS()), "l");

   
		//legend->SetFillStyle(0); // Make legend background transparent

    TF1* fitL = nullptr;
    TF1* fitFunc2 = nullptr;
    TLegend *legend = nullptr;
    const std::string fit_name = (nBins == 1) ? "Gauss" : "Landau";

    if (nBins == 1)
    {
        fitL = createSumOfGausFunctions(hist1, xMin, xMax);
        fitFunc2 = createSumOfGausFunctions(hist2, xMin, xMax);
        legend = new TLegend(0.2, 0.75, 0.6, 0.95);

    }
    else
    {
        fitL = fitL = new TF1("fitFunc", "landau", xMin, xMax);
        fitFunc2 = createSumOfLandauFunctions(hist2, xMin, xMax);
        legend = new TLegend(0.55, 0.75, 0.95, 0.95);
    }
    legend->Draw();

	//TF1* fitL = createSumOfGausFunctions(hist1, xMin, xMax);
	//TF1* fitL = new TF1("fitFunc", "gaus", xMin, xMax);
	//TF1* fitL = createSumOfLandauFunctions(hist1, xMin, xMax);
	hist1->Fit(fitL, "R");
	fitL->SetLineColorAlpha(kBlue, 1);
    fitL->SetLineStyle(2);
	//fitL->Draw("same");



   //legend->AddEntry(hist1, "NSGA - 375x300", "l");
    legend->AddEntry(hist1, (leg_name).c_str(), "l");
   legend->AddEntry((TObject*)0, Form("Mean: %.2f", hist1->GetMean()), "");
   legend->AddEntry((TObject*)0, Form("Std Dev: %.2f", hist1->GetStdDev()), "");
   //legend->AddEntry(fitL, (fit_name + " fit").c_str(), "l");


	saveFitResultsToFile(fitL,6,(saveFolder+plotName+"_fit1.txt").c_str());
	

    double maxY=0;
    if (hist1->GetMaximum()>hist2->GetMaximum()){
		maxY=hist1->GetMaximum();
		}else{maxY=hist2->GetMaximum();}
    
	hist1->SetAxisRange(0, maxY*1.1, "Y"); // Set the Y-axis scale from 0 to 50

    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    c->SaveAs((saveFolder + plotName + ".pdf").c_str());
    c->SaveAs((saveFolder + plotName + ".tex").c_str());
c->Update(); // Update the canvas to show the plot
c->Modified(); // Notify the canvas that it has been modified
c->WaitPrimitive(); // Wait for user interaction (closing the plot window) before exiting
    

    delete c;
    delete hist1;
    delete hist2;


}
