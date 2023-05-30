#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>

#include <iostream>

int HorizontalHistoTest() {
// Create the histograms
   TH1F* histogram1 = new TH1F("histogram1", "Histogram 1", 10, 0, 10);
   TH1F* histogram2 = new TH1F("histogram2", "Histogram 2", 10, 0, 10);

   // Fill the histograms with data
   histogram1->Fill(10);
   histogram1->Fill(9);
   histogram1->Fill(8);
   histogram1->Fill(8);
   histogram1->Fill(8);
   histogram1->Fill(8);
   histogram1->Fill(7);
   histogram1->Fill(6);
   histogram1->Fill(6);
   histogram1->Fill(6);
   histogram1->Fill(6);
   histogram1->Fill(5);
   histogram1->Fill(4);
   histogram1->Fill(4);
   histogram1->Fill(4);
   histogram1->Fill(3);
   histogram1->Fill(2);
   histogram1->Fill(2);
   histogram1->Fill(1);

   histogram2->Fill(1);
   histogram2->Fill(1);
   histogram2->Fill(1);
   histogram2->Fill(1);
   histogram2->Fill(2);
   histogram2->Fill(2);
   histogram2->Fill(3);
   histogram2->Fill(3);
   histogram2->Fill(3);
   histogram2->Fill(4);
   histogram2->Fill(4);
   histogram2->Fill(5);
   histogram2->Fill(5);
   histogram2->Fill(6);
   histogram2->Fill(7);
   histogram2->Fill(7);
   histogram2->Fill(8);
   histogram2->Fill(9);
   histogram2->Fill(10);
   histogram2->Fill(10);

   // Create a canvas to display the histograms
   TCanvas* canvas = new TCanvas("canvas", "Horizontal Histograms", 1200, 600);
   canvas->Divide(2, 1); // Divide the canvas into two parts

   // Set the histogram options
   histogram1->SetFillColor(kBlue);
   histogram2->SetFillColor(kRed);

   // Set the bar offsets to point in opposite directions
   //histogram1->SetBarOffset(0.5);
   //histogram2->SetBarOffset(0.5);

   // Draw the histograms
   canvas->cd(1); // Select the first part of the canvas
   histogram1->Draw("hbar");

   canvas->cd(2); // Select the second part of the canvas
   histogram2->Draw("hbar");
   histogram1->GetYaxis()->SetRangeUser(5, 0);

   // Update the canvas
   canvas->Update();

   // Wait for the user to close the canvas
   canvas->WaitPrimitive();

   // Clean up
   delete histogram1;
   delete histogram2;
   delete canvas;

   return 0;
}
