/* Tree and Tuple values from G4
 * 	* Arrivals
 * 		- fEvent
 * 		- fX
 * 		- fY
 * 		- fZ
 * 		- fWlen
 * 		- fMeanPath
 * 	* Detected
 * 		- fEvent
 * 		- ftrackID
 * 		- fX
 * 		- fY
 * 		- fZ
 * 		- ftimeG
 * 		- ftimeL
 * 		- fPDE
 * 		- fwlen
 * 		- fMeanPath
 * 	* Stepping
 * 		- fKilledLength
 * 		- ftime
 * 		- fX
 * 		- fY
 * 		- fZ
 * 	* EndOfEvent
 * 		- fEdep
 * 		- fPhot
 * 		- fHits
 * 		- fPDE
 * 		- fLOapprox
 * 		- fPrimaryX
 * 		- fPrimaryZ
 * 		- fGlueL
 * 		- fResinL
 * 		- fDetXpos
 * 		- fDetXpos2
 * 		- fCrossTalkCount
 * 		- fCrossTalkMev
 * 		- fTiming
 * 		- fphL
 * 		- fphR
 * 		- fevt
 * 	* EndofRun
 * 		- fLOAvg
 * 		- fLOStd
 * 		- fTimAvg
 * 		- fTimStd
 * 		- frun
 * 		- fincr
*/

bool checkRootData(const char* fileName, const char* treeName, const char* branchName)
{
	/*
	std::cout << "fileName: "<< fileName << std::endl;
	std::cout << "treeName: "<< treeName << std::endl;
	std::cout << "branchName: "<< branchName << std::endl;
	*/

    TFile* file = TFile::Open(fileName);
	//std::cout << "Starting Check:" << std::endl;

    // Check if file is open
    if (!file || file->IsZombie()){
		std::cout << "File is Zombie" << std::endl;
        return false;}
	//std::cout << "File is NOT Zombie:" << std::endl;
	
    // Check if tree exists
    if (!file->Get(treeName)){
		std::cout << "Tree does not exist" << std::endl;
        return false;}
	//std::cout << "Tree EXISTS" << std::endl;
	
    TTree* tree = (TTree*)file->Get(treeName);

    // Check if branch exists
    if (!tree->GetBranch(branchName)){
		std::cout << "Branch name does not exist" << std::endl;
        return false;}
	//std::cout << "Branch name EXISTS" << std::endl;

    TBranch* branch = tree->GetBranch(branchName);

    // Check if branch is readable
    double value;
    branch->SetAddress(&value);
    
    if (branch->GetEntry(0) < 0){
		std::cout << "Branch is not readable" << std::endl;
        return false;}
	//std::cout << "Branch is READABLE " << std::endl;

    file->Close();

    return true;
}
