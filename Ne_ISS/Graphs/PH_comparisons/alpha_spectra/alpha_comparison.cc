#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include <iostream>


int main() {
  
  // Open the root files
  TFile *file1 = TFile::Open("../../../my_data/alpha_calibrated_PHD-new/R1_allrunsadded.root");
  
  // Access the subdirectory and histogram
  TDirectory *subdir1 = (TDirectory*)file1->Get("asic_hists/module_0");
  TH1F *alpha_PH = (TH1F*)subdir1->Get("pside_mod0");

  // Open the root file
  TFile *file2 = TFile::Open("../../../my_data/alpha_source_calib_NOPHD/R1_allrunsadded.root");
  
  // Access the subdirectory and histogram
  TDirectory *subdir2 = (TDirectory*)file2->Get("asic_hists/module_0");
  TH1F *alpha_NO_PH = (TH1F*)subdir2->Get("pside_mod0");
  
  // Create a canvas
  TCanvas *canvas = new TCanvas("canvas","alpha PH Comparison",800,600);

  alpha_PH->GetXaxis()->SetRangeUser(2300, 6500);
  alpha_PH->SetStats(0);
  alpha_PH->SetLineColor(4);
  alpha_PH->SetLineWidth(1.5);
  
  alpha_NO_PH->SetLineColor(2);
  alpha_NO_PH->SetLineWidth(1.5);
  
  alpha_PH->Draw("HIST");
  alpha_NO_PH->Draw("Same");
  alpha_PH->Draw("Same");

  TLine* alpha1 = new TLine(3182.68, 0, 3182.68, 545e3);
  alpha1->SetLineStyle(2);
  alpha1->SetLineWidth(2);
  alpha1->SetLineColor(1);
  alpha1->Draw("Same");

  TLine* alpha2 = new TLine(5156.59, 0, 5156.59, 545e3);
  alpha2->SetLineStyle(2);
  alpha2->SetLineWidth(2);
  alpha2->SetLineColor(1);
  alpha2->Draw("Same");

  TLine* alpha3 = new TLine(5485.56, 0, 5485.56, 545e3);
  alpha3->SetLineStyle(2);
  alpha3->SetLineWidth(2);
  alpha3->SetLineColor(1);
  alpha3->Draw("Same");

  TLine* alpha4 = new TLine(5804.77, 0, 5804.77, 545e3);
  alpha4->SetLineStyle(2);
  alpha4->SetLineWidth(2);
  alpha4->SetLineColor(1);
  alpha4->Draw("Same");

  TLegend* leg = new TLegend(0.35,0.5,0.6,0.7);
  leg->AddEntry(alpha_NO_PH,"No PH correction");
  leg->AddEntry(alpha_PH,"PH corrected ");
  //leg->SetBorderSize(0);
  leg->Draw();

  // Save the canvas
  canvas->SaveAs("alpha_PH_comparison_R14-R40.png");
  
  // Close the file
  file1->Close();
  file2->Close();

  return 0;
}