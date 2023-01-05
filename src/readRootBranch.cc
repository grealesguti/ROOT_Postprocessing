#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <vector>
#include "checkRootData.cc"
std::vector<double> readRootBranch(const char* fileName, const char* treeName, const char* branchName)
{
	
	
	std::cout << "fileName: "<< fileName << std::endl;
	//std::cout << "treeName: "<< treeName << std::endl;
	//std::cout << "branchName: "<< branchName << std::endl;
	
	std::vector<double> data;

	if(checkRootData(fileName,treeName,branchName)){

			TFile* file = TFile::Open(fileName);
			TTree* tree = (TTree*)file->Get(treeName);
			TBranch* branch = tree->GetBranch(branchName);

			double value;
			branch->SetAddress(&value);
				//std::cout << "Values:"<< std::endl;

			for (int i = 0; i < tree->GetEntries(); i++)
			{
				branch->GetEntry(i);
				//std::cout << value<< std::endl;

				data.push_back(value);
			}

			file->Close();
    }else {
		std::cout << "Sth wrong with the file!!!"<< std::endl;
		}

    return data;
}
