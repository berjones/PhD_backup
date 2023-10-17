#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>


int main() {
  
  // Open the root files
  TFile *file1 = TFile::Open("../../../my_data/PHD_corrected_data_new/hists_grouped_runs/22Ne_R14_to_R40_notall_sumhists_20230127_104449.root");
  
  // Access the subdirectory and histogram
  TDirectory *subdir1 = (TDirectory*)file1->Get("RecoilMode");
  TH1F *Ex_recoil_PH = (TH1F*)subdir1->Get("Ex_recoil");

  // Open the root file
  TFile *file2 = TFile::Open("../../../my_data/No_PHD_correction_data/hists_grouped/22Ne_R14_to_R40_notall_sumhists_20230119_223448.root");
  
  // Access the subdirectory and histogram
  TDirectory *subdir2 = (TDirectory*)file2->Get("RecoilMode");
  TH1F *Ex_recoil_NO_PH = (TH1F*)subdir2->Get("Ex_recoil");
  
  // Create a canvas
  TCanvas *canvas = new TCanvas("canvas","Ex_recoil PH Comparison",800,600);

  Ex_recoil_PH->GetXaxis()->SetRangeUser(-1000, 7000);
  Ex_recoil_PH->SetStats(0);
  Ex_recoil_PH->SetLineColor(4);
  Ex_recoil_PH->SetLineWidth(1.5);
  
  Ex_recoil_NO_PH->SetLineColor(2);
  Ex_recoil_NO_PH->SetLineWidth(1.5);
  
  Ex_recoil_PH->Draw("HIST");
  Ex_recoil_NO_PH->Draw("Same");
  Ex_recoil_PH->Draw("Same");

  TLegend* leg = new TLegend();
  leg->AddEntry(Ex_recoil_NO_PH,"No PH correction");
  leg->AddEntry(Ex_recoil_PH,"PH corrected ");
  //leg->SetBorderSize(0);
  leg->Draw();

  // Save the canvas
  canvas->SaveAs("Ex_PH_comparison_R14-R40.png");
  
  // Close the file
  file1->Close();
  file2->Close();

  return 0;
}