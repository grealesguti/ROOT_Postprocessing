#include <dirent.h>
#include <vector>
#import "utils.cc"
#include "readRootBranch.cc"
std::vector<double> readRootDataFromFolder(const std::string& folder, const std::string& treeName, const std::string& branchName)
{
    // Vector to store data from all files
    std::vector<double> data;

    // Open folder
    DIR* dir = opendir(folder.c_str());
    if (dir)
    {
        // Read all files in folder
        struct dirent* ent;
        while ((ent = readdir(dir)) != NULL)
        {
            // Skip . and ..
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;

            // Read data from file
            std::vector<double> fileData = readRootBranch((folder + "/" + ent->d_name).c_str(), treeName.c_str(), branchName.c_str());
            data.insert(data.end(), fileData.begin(), fileData.end());
        }

        closedir(dir);
    }

    return data;
}
