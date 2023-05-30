#!/bin/bash
# Default
#root -b -q -l 'plotHistRootData_LC_Multiple2.cc("../../TierIIData/2023_02/Rst_SingleSiPM_Na_23-02-09v2","../../TierIIData/2023_02/Rst_SingleSiPM_Muon_23-02-09v2","EndOfEvent","fLOapprox","EndOfEvent_fLOMult_Single",0,2000,1,"../Figures/LOHistMult/Journal_Def/","Single Muon 2MeV","Single Na 0.511MeV","Light Output [ph/MeV]")'
#root -b -q -l 'plotHistRootData_LC_Multiple2.cc("../../TierIIData/2023_02/Rst_FL_Na_23-02-09v2","../../TierIIData/2023_02/Rst_FL_Muon_23-02-09v2","EndOfEvent","fLOapprox","EndOfEvent_fLOMult_FL",0,2000,1,"../Figures/LOHistMult/Journal_Def/","FL Muon 2MeV","FL Na 0.511MeV","Light Output [ph/MeV]")'
#root -b -q -l 'plotHistRootData_LC_Multiple2.cc("../../TierIIData/2023_02/Rst_GC3_Na_23-02-09v2","../../TierIIData/2023_02/Rst_GC3_Muon_23-02-09v2","EndOfEvent","fLOapprox","EndOfEvent_fLOMult_GC3",0,2000,1,"../Figures/LOHistMult/Journal_Def/","Module Muon 2MeV" ,"Module Na 0.511MeV","Light Output [ph/MeV]")'
.q
# Optimized
#root -b -q -l 'plotHistRootData_LC_Multiple2.cc("../../TierIIData/2023_02/Rst_SingleSiPM_Na_23-02-09v2","../../TierIIData/2023_02/Rst_SingleSiPM_Muon_23-02-09v2","EndOfEvent","fHits","EndOfEvent_fLOMult_Single",0,14000,0,"../Figures/LOHistMult/Journal_NSGA/","Single Muon 2MeV","Single Na 0.511MeV","Light Output [ph/MeV]")'
#root -b -q -l 'plotHistRootData_LC_Multiple2.cc("../../TierIIData/2023_02/Final_NSGAII_375xW","../../TierIIData/2023_02/Final_NSGAII_375x3","EndOfEvent","fHits","EndOfEvent_fLCMult_test",0,13500,0,"../Figures/LOHistMult/Journal_NSGA/","Default SiPM 3.75x3","Opt. SiPM 3.75x3","Light Collection [ph]")'

root -b -q -l 'plotHistRootData_LC_Multiple2.cc("../../TierIIData/2023_02/Rst_FL_Muon_23-02-10_SiPM375","../../TierIIData/2023_02/Final_NSGAII_375x3","EndOfEvent","fHits","EndOfEvent_fLCMult_375300",0,25000,0,"../Figures/LOHistMult/Journal_NSGA/","Default SiPM 3.75x3","Opt. SiPM 3.75x3","Light Collection [ph]")'
root -b -q -l 'plotHistRootData_LC_Multiple2.cc("../../TierIIData/2023_02/Rst_FL_Muon_23-02-10_SiPM375","../../TierIIData/2023_02/Final_NSGAII_375xW","EndOfEvent","fHits","EndOfEvent_fLCMult_375W",0,30000,0,"../Figures/LOHistMult/Journal_NSGA/","Default SiPM 3.75x3","Opt. SiPM 3.75xW","Light Collection [ph]")'
root -b -q -l 'plotHistRootData_LC_Multiple2.cc("../../TierIIData/2023_02/Rst_FL_Muon_23-02-10_SiPM375","../../TierIIData/2023_02/Final_NSGAII_6xW","EndOfEvent","fHits","EndOfEvent_fLCMult_600W",0,30000,0,"../Figures/LOHistMult/Journal_NSGA/","Default SiPM 3.75x3","Opt. SiPM 6xW","Light Collection [ph]")'
