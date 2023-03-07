#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>

#include <dirent.h>

#include "plotHistEndOfEventEdepLC.cc"
//#include "plotHistEndOfEventDetectedHits.cc"
//#include "plotGraphEndOfEventPrimaryZvsLO.cc"
//#include "plotEndOfEventEdep.cc"

#include <vector>

std::vector<std::string> get_subfolder_names(const std::string& folder_path) {
    std::vector<std::string> subfolder_names;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (folder_path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            std::string subfolder_name(ent->d_name);
            // Check if the entity is a directory and not the current or parent directory
            if (ent->d_type == DT_DIR && subfolder_name != "." && subfolder_name != "..") {
                subfolder_names.push_back(subfolder_name);
            }
        }
        closedir (dir);
    } else {
        std::cerr << "Could not open directory " << folder_path << std::endl;
    }
    return subfolder_names;
}

void loop_LC() {
    std::string folder_path="../TierIIData/2023_02/";
	std::vector<std::string> subfolder_names = get_subfolder_names(folder_path);
	
    std::cout << "Subfolder names in " << folder_path << ":" << std::endl;
    for (const auto& subfolder_name : subfolder_names) {
        std::cout << subfolder_name << std::endl;
        plotHistEndOfEventEdepLC(subfolder_name);
        //plotHistEndOfEventDetectedHits(subfolder_name);
        //plotGraphEndOfEventPrimaryZvsLO(subfolder_name);
        //plotEndOfEventEdep(subfolder_name);
    }	
}
