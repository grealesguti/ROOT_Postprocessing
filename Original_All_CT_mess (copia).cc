#include <string>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include "TError.h"


#include<string>

namespace fs = std::filesystem;

void CT2212(string folder){

// Change number of evts per run!! , we know how many evts per run!!!! do we use it?? for initialization!!!, not needed?? we can find it out with the size of a single file in endofevent!!!
	int runevts=10;
	
	
    std::cout << folder << std::endl;
    int nfile;nfile=0;
    char tmp[256];
    std::string path = getcwd(tmp, 256);
    std::cout << path << std::endl;
    path=path+"/"+folder;
    std::cout << path << std::endl;
    std::cout << "End printing path" << std::endl;
    TFile *input;
    char filename[256];
    double fLOapprox, fEdep, fPrimaryX,fPrimaryZ,fGlueL,fResinL,fDetPosX1,fDetPosX2,fDetPosY1,fDetPosY2, fPhot,fX,fZ,fHits,fphL; 
    int fEvent;
    int entries;
    int muon=1;
    int incr;

    TH1F *LO;
    TH1F *DetectedHits;
    TH1F *EDEP;
    TH1F *hCT;
    TH1F *hCTnom;
    TH1F *hCT2;
    TH1F *hCT3;
    LO = new TH1F("LO","LO",75,0.,2000);
    EDEP = new TH1F("EDEP","EDEP",75,0.,10);
    hCTnom = new TH1F("CT1nom","CT1nom",75,0.,3000);
    hCT = new TH1F("CT1","CT1",75,0.,0.5);
    hCT2 = new TH1F("CT2","CT2",75,0.,0.5);
    hCT3 = new TH1F("CT3","CT3",75,0.,0.5);

    THStack *hs = new THStack("hs","");
    double pincr,pincrend,entriesDet;
    string incrnum;
    string path1;
    int nhits=0, nevts=0, entriesHits=0,nevt=0;
// Initialization finished
// Find number of files for initializations
//##################################################################################
    for (const auto & entry : fs::directory_iterator(path)){
        std::cout << entry.path() << std::endl;
        int len = strlen(entry.path().c_str());
        if (strcmp(".root", &entry.path().c_str()[len-5]) == 0){
            std::cout << "This is a .root file." << std::endl;
            TFile myfile(entry.path().c_str());
              if (myfile.IsZombie()) {
                    std::cout << "something very wrong, cannot use this file, exit" << std::endl;
                 
                }else if (myfile.TestBit(TFile::kRecovered)) {
                    std::cout << "the Recover procedure has been run when opening the file and the Recover was successful" << std::endl;
                }else{
                    std::cout << "all OK" << std::endl;
                    input = new TFile( entry.path().c_str(), "READONLY");  
                    TTree *EndOfEvent = (TTree*)input->Get("EndOfEvent");
                    entries = EndOfEvent->GetEntries();
                    /////////////////////////
                    // Start of data analysis
                        nfile+=1;
                        for(int i =0; i<entries; i++){
                            EndOfEvent->GetEntry(i);
                            if (fEdep>0. && i>0){
                                nevt+=1;
                            }                    
                        }
                    input->Close();
                    /////////////////////////
                }   
            std::cout << "Added" << std::endl;
            }
        }   

std::cout << "### Data Analysis start::" << std::endl;
// Data Analysis -> we analyze the data per file!!! instead of at the end
//##################################################################################
    int idx0=0,evt=0;
    nevt=runevts*nfile+1;
    double gunZ[nevt],LD[nevt],CTL1[nevt],CTL1Div[nevt],CTL2[nevt],CTL2Div[nevt],CTL3[nevt], CTL3Div[nevt],EdepArr[nevt], GlueArr[nevt],ResinArr[nevt],RGArr[nevt];
int L0[nevt],cCT1=0,cCT2=0,cCT3=0;
for(int i =0; i<nevt; i++){
gunZ[i]=0.;LD[i]=0.;CTL1[i]=0.;CTL1Div[i]=0.;CTL2[i]=0.;CTL2Div[i]=0.;CTL3[i]=0.;CTL3Div[i]=0.;EdepArr[i]=0.;L0[i]=0;GlueArr[i]=0;ResinArr[i]=0;RGArr[i]=0;
}
    int nnevet=nevt,e511=0;    
    entries=0;
    nfile=0;nevt=0;
std::cout << "### Variables initialized::" << std::endl;
    for (const auto & entry : fs::directory_iterator(path)){
std::cout << "### loop starts::" << std::endl;
        std::cout << entry.path() << std::endl;
        int len = strlen(entry.path().c_str());
        if (strcmp(".root", &entry.path().c_str()[len-5]) == 0){
            //std::cout << "This is a .root file." << std::endl;
            TFile myfile(entry.path().c_str());
std::cout << "### My File defined::" << std::endl;
              if (myfile.IsZombie()) {
                    std::cout << "something very wrong, cannot use this file, exit" << std::endl;
                 
                }else if (myfile.TestBit(TFile::kRecovered)) {
                    std::cout << "the Recover procedure has been run when opening the file and the Recover was successful" << std::endl;
                }else{
                    std::cout << "all OK" << std::endl;
                        
                    /////////////////////////
                    // Start of data analysis
                    input = new TFile( entry.path().c_str(), "READONLY");  
                    std::cout << "input" << std::endl;
                    TTree *EndOfEvent = (TTree*)input->Get("EndOfEvent");
                    EndOfEvent->SetBranchAddress("fHits",&fHits);
                    EndOfEvent->SetBranchAddress("fPrimaryZ",&fPrimaryZ);
                    EndOfEvent->SetBranchAddress("fEdep",&fEdep);
                    EndOfEvent->SetBranchAddress("fphR",&fphL);
                    EndOfEvent->SetBranchAddress("fGlueL",&fGlueL);
                    EndOfEvent->SetBranchAddress("fResinL",&fResinL);
                    std::cout << "End Of Event Tree" << std::endl;

                    TTree *Detected = (TTree*)input->Get("Detected");
                    Detected->SetBranchAddress("fX",&fX);
                    Detected->SetBranchAddress("fZ",&fZ);
                    Detected->SetBranchAddress("fEvent",&fEvent);
                    entriesDet = Detected->GetEntries();
                    std::cout << "Detected Tree" << std::endl;

                    for(int i =0; i<entriesDet; i++){
                        Detected->GetEntry(i);
                        evt=fEvent;
                        if (fX>-3.1 && fX<-0.1 && fZ>0){L0[idx0+evt]+=1;}  
                        else if (fX>-6.4 && fX<-3.2 && fZ>0){CTL1[idx0+evt]+=1;}  
                        else if (fX>-9.6 && fX<-6.4 && fZ>0){CTL2[idx0+evt]+=1;}
                        else if (fX>-12.8 && fX<-9.6 && fZ>0){CTL3[idx0+evt]+=1;}                                            
                    }
                    std::cout << "Detected loop D events "<<entriesDet << std::endl;
                    //entries1 = EndOfEvent->GetEntries();
                    std::cout << "EoE events "<<EndOfEvent->GetEntries() << std::endl;
                    for(int i =0; i<EndOfEvent->GetEntries(); i++){
                        EndOfEvent->GetEntry(i);
                        if (fEdep>0. && i>0){
                            EndOfEvent->GetEntry(i);
                            EdepArr[idx0+i]=fEdep;
                            gunZ[idx0+i]=fPrimaryZ*1000;
                            LD[idx0+i]=fphL;
                            GlueArr[idx0+i]=fGlueL;
                            ResinArr[idx0+i]=fResinL;
                            RGArr[idx0+i]=fGlueL+fResinL;
                            CTL1Div[idx0+i]=CTL1[idx0+i]/fphL;
                            CTL2Div[idx0+i]=CTL2[idx0+i]/fphL;
                            CTL3Div[idx0+i]=CTL3[idx0+i]/fphL;
                            if (fEdep>0.5 ){
std::cout << "evt CTL1: " << CTL1[idx0+i] <<" with ph: "<< fphL << " or "<< L0[idx0+i] << " at "<< fPrimaryZ*1000 <<" or "<< gunZ[idx0+i] << "and edep "<<fEdep<< "in idx "<<idx0+i << std::endl;
                                hCTnom->Fill(CTL1[idx0+i]);
                                hCT->Fill(CTL1Div[idx0+i]);
                                hCT2->Fill(CTL2Div[idx0+i]);
                                hCT3->Fill(CTL3Div[idx0+i]);
                                e511+=1;
                            }
                        }    
                    //std::cout << "End of Event entry" << std::endl;                
                    }
                    idx0+=evt+1;
                    input->Close();
                        // Add histogram to stack
                        //nfile+=1;
                    /////////////////////////
                }   
            std::cout << "Added" << std::endl;
            }
        }   
// Graphing only edep>0.5
    /*double gunZ511[e511],CTL1511[e511];
    int cc=0;
    for(int i =0; i<idx0; i++){
        if(EdepArr[i]>0.5){
            gunZ511[cc]=gunZ511[i];
            CTL1511[cc]=CTL1Div[i];
            cc+=1;
        }                                   
    }*/
            std::cout << "Plotting" << std::endl;
// Plotting
//##################################################################################
    string ImageFolder= "Figures";
    //hCT->Scale(1./hCT->Integral());
    //hCT2->Scale(1./hCT2->Integral());
    //hCT3->Scale(1./hCT3->Integral());

    //THStack *hs = new THStack("hs","");
    hs->Add(hCT,"L+1");
    hs->Add(hCT2,"L+2");
    hs->Add(hCT3,"L+3");

    TCanvas *cs = new TCanvas();
    Double_t factor = 1.;
    Double_t mlo=hCT->GetMaximum();
    hCT->Scale(factor/mlo);
    hCT->SetXTitle("Light Collection +1 Channel/0 Channel");
    hCT->SetYTitle("Intensity");
    hCT->SetMarkerStyle(kMultiply);
    hCT->SetLineWidth(2);
    hCT->SetLineColorAlpha(kAzure, 0.7);
    hCT->Draw("E");
    hCT->SetLineColorAlpha(kBlue, 0.7);
    hCT->SetLineWidth(2);
        //hCT->GetXaxis()->SetLimits(0,0.3);
        //hCT->GetYaxis()->SetRangeUser(LOlow,LOhigh);
    hCT->Draw("HIST SAME C ");

    TCanvas *cs4 = new TCanvas();
    //Double_t factor1 = 1.;
    //Double_t mlonom=hCTnom->GetMaximum();
    //hCTnom->Scale(factor1/mlonom);
    hCTnom->SetXTitle("Light Collection +1 Channel [ph]");
    hCTnom->SetYTitle("Intensity");
    hCTnom->SetMarkerStyle(kMultiply);
    hCTnom->SetLineWidth(2);
    hCTnom->SetLineColorAlpha(kAzure, 0.7);
    hCTnom->Draw("E");
    hCTnom->SetLineColorAlpha(kBlue, 0.7);
    hCTnom->SetLineWidth(2);
        //hCT->GetXaxis()->SetLimits(0,0.3);
        //hCT->GetYaxis()->SetRangeUser(LOlow,LOhigh);
    hCTnom->Draw("HIST SAME C ");

    TCanvas *cs2 = new TCanvas();
    hCT2->Draw();


    TCanvas *cs1 = new TCanvas();
       TPad *grid2 = new TPad("grid","",0,0,1,1); 
       grid2->Draw();
       grid2->cd();
       grid2->SetGrid();
    auto g1 = new TGraph(nnevet,gunZ,CTL1);
        g1->SetMarkerStyle(kFullCircle);
        g1->SetTitle("");
        g1->Draw("AP");
        g1->GetXaxis()->SetTitle("Z[mm]");
        g1->GetYaxis()->SetTitle("CTL+1");
        g1->GetXaxis()->CenterTitle(true);
        g1->GetYaxis()->SetRangeUser(0,1000);
        g1->GetXaxis()->SetLimits(-28.5,28.5);
    //hs->SetXTitle("ph_i/ph_0");
    //hs->SetYTitle("Intensity");


    TF1 *pol2 = new TF1("pol2","[0]+[1]*x+[2]*x*x",0.03,+0.03);
    TCanvas *cs3 = new TCanvas();
       TPad *grid3 = new TPad("grid","",0,0,1,1); 
       grid3->Draw();
       grid3->cd();
       grid3->SetGrid();
    auto g2 = new TGraph(nnevet,gunZ,CTL1Div);
        g2->SetMarkerStyle(kFullCircle);
        g2->SetTitle("");
        g2->GetXaxis()->SetTitle("Z[mm]");
        g2->GetYaxis()->SetTitle("CTL+1");
        g2->GetXaxis()->CenterTitle(true);
        g2->GetYaxis()->SetRangeUser(0,0.3);
        g2->GetXaxis()->SetLimits(-28.5,28.5);
        g2->Fit("pol2");
        g2->Draw("AP");
        
        
    TCanvas *cs10 = new TCanvas();
       TPad *grid4 = new TPad("grid","",0,0,1,1); 
       grid4->Draw();
       grid4->cd();
       grid4->SetGrid();
    auto g4 = new TGraph(nnevet,GlueArr,CTL1Div);
        g4->SetMarkerStyle(kFullCircle);
        g4->SetTitle("");
        g4->GetXaxis()->SetTitle("Z[mm]");
        g4->GetYaxis()->SetTitle("CTL+1");
        g4->GetXaxis()->CenterTitle(true);
        g4->GetYaxis()->SetRangeUser(0,0.3);
        g4->GetXaxis()->SetLimits(0,0.5);
        g4->Fit("pol2");
        g4->Draw("AP");
        
    TCanvas *cs11 = new TCanvas();
       TPad *grid5 = new TPad("grid","",0,0,1,1); 
       grid5->Draw();
       grid5->cd();
       grid5->SetGrid();
    auto g5 = new TGraph(nnevet,ResinArr,CTL1Div);
        g5->SetMarkerStyle(kFullCircle);
        g5->SetTitle("");
        g5->GetXaxis()->SetTitle("Z[mm]");
        g5->GetYaxis()->SetTitle("CTL+1");
        g5->GetXaxis()->CenterTitle(true);
        g5->GetYaxis()->SetRangeUser(0,0.3);
        g5->GetXaxis()->SetLimits(0,0.5);
        g5->Fit("pol2");
        g5->Draw("AP");
        
    TCanvas *cs12 = new TCanvas();
       TPad *grid6 = new TPad("grid","",0,0,1,1); 
       grid6->Draw();
       grid6->cd();
       grid6->SetGrid();
    auto g6 = new TGraph(nnevet,RGArr,CTL1Div);
        g6->SetMarkerStyle(kFullCircle);
        g6->SetTitle("");
        g6->GetXaxis()->SetTitle("Z[mm]");
        g6->GetYaxis()->SetTitle("CTL+1");
        g6->GetXaxis()->CenterTitle(true);
        g6->GetYaxis()->SetRangeUser(0,0.3);
        g6->GetXaxis()->SetLimits(0,0.5);
        g6->Fit("pol2");
        g6->Draw("AP");



}
