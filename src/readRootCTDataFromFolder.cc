
#include "readRootCT.cc"
#include <iostream>
#include <vector>
#include <filesystem>
#include <tuple>

// Function to read all ROOT files in a folder and append the resulting vectors of ctl1, ctl1Div, and gunZ
std::tuple<std::vector<double>, std::vector<double>, std::vector<double>> readRootCTDataFromFolder(const std::string& folderPath)
{
    std::vector<double> ctl1All, ctl1DivAll, gunZAll;
    
    // Loop through all files in the folder
    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        // Check if file is a ROOT file
        if (entry.path().extension() == ".root")
        {
            std::cout << "Reading file: " << entry.path() << std::endl;
            
            // Read the ROOT file using readRootCT function
            auto [ctl1, ctl1Div, gunZ] = readRootCT(entry.path().c_str());
            
            // Append the resulting vectors to the overall vectors
            ctl1All.insert(ctl1All.end(), ctl1.begin(), ctl1.end());
            ctl1DivAll.insert(ctl1DivAll.end(), ctl1Div.begin(), ctl1Div.end());
            gunZAll.insert(gunZAll.end(), gunZ.begin(), gunZ.end());
        }
    }
    
    // Return the resulting appended vectors
    return std::make_tuple(ctl1All, ctl1DivAll, gunZAll);
}
