#include <fstream>
#include <string>
#include <iostream>
#include <iomanip> // for std::setprecision
#include <cmath>   // for std::fabs

void saveFitResultsToFile(const TF1* fitFunc, int numParameters, const std::string& fileName)
{
    // Create a file to save the fit results
    std::ofstream outputFile(fileName);

    // Check if the file was opened successfully
    if (outputFile.is_open()) {
        // Write the fit results to the file
        for (int i = 0; i < numParameters; ++i) {
            Double_t fitParameter = fitFunc->GetParameter(i);
            Double_t fitError = fitFunc->GetParError(i);
            double relativeError = std::fabs(fitError / fitParameter);

            outputFile << "Fit Parameter " << i+1 << ": " << fitParameter << " +/- " << fitError
                       << " (Relative Error: " << relativeError << ")\n";
        }

        // Close the file
        outputFile.close();

        // Print a message indicating successful file writing
        std::cout << "Fit results saved to " << fileName << std::endl;
    }
    else {
        // Print an error message if the file couldn't be opened
        std::cerr << "Error opening " << fileName << " for writing" << std::endl;
    }
}
