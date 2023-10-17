#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TLegend.h"
#include <iostream>

int main(){
   TCanvas *c1 = new TCanvas("c1","Energy Residuals",800,600);
   //c1->SetGrid();

   // create the coordinate arrays
   Int_t n = 6;
   Float_t Energy[n]  = {0, 1016.926, 1822.26, 2315.26, 3220.69, 3431.6};
   
   Float_t NO_PHD_residual[n]  = {84.0278, 77.724, 49.12, 71.08, 59.55, 62.65};
   Float_t PHD_residual[n] = {-3.4315, -9.196, -2.89, -3.199999999, -9.37, -8.54};

   // create the error arrays
   Float_t Energy_err[n] = {0,0,0,0,0,0};
   Float_t NO_PHD_residual_err[n] = {1.67,1.27,8.89,2.61,1.36,1.51};
   Float_t PHD_residual_err[n] = {1.52,1.06,9.17,2.53,1.43,1.48};

   // create the known values array
   Float_t Known_val[n] = {0,0,0,0,0,0};
   Float_t Uncertainty[n] = {0,7,8,8,8,8};

   Float_t NO_PHD_residual_err_Total[n];
   for (int i = 0; i < n; i++) {
    NO_PHD_residual_err_Total[i] = TMath::Sqrt(std::pow(NO_PHD_residual_err[i], 2) + std::pow(Uncertainty[i], 2));
   }

   Float_t PHD_residual_err_Total[n];
   for (int i = 0; i < n; i++) {
    PHD_residual_err_Total[i] = TMath::Sqrt(std::pow(PHD_residual_err[i], 2) + std::pow(Uncertainty[i], 2));
   }   

   // create the TGraphErrors and draw it
   TGraphErrors  *NoPHD = new TGraphErrors(n,Energy,NO_PHD_residual,NO_PHD_residual_err,NO_PHD_residual_err_Total);
   NoPHD->SetTitle(0);
   NoPHD->SetMarkerColor(2);
   NoPHD->SetMarkerStyle(20);
   
   NoPHD->GetYaxis()->SetRangeUser(-30., 120.);
   NoPHD->GetXaxis()->SetRangeUser(-250., 3500.);
   NoPHD->GetYaxis()->SetTitle("Residuals [keV]");
   NoPHD->GetXaxis()->SetTitle("Energy [keV]");


   TGraphErrors  *PHD = new TGraphErrors(n,Energy,PHD_residual,PHD_residual_err,PHD_residual_err_Total);
   PHD->SetMarkerColor(4);
   PHD->SetMarkerStyle(20);

   TGraphErrors  *known = new TGraphErrors(n,Energy,Known_val,Uncertainty,Uncertainty);
   known->SetMarkerSize(0);
   known->SetLineWidth(0);
   known->SetFillColor(kYellow+1);

   TF1 *line = new TF1("line", "0", -250, 3600);
   line->SetLineStyle(2);



   TF1 *NoPHD_fit = new TF1("NoPHD_fit", "[0] + [1]*x",NoPHD->GetXaxis()->GetXmin(),NoPHD->GetXaxis()->GetXmax());
   double Intercept_guess = 80;
   double gradient_guess = -1;
   NoPHD_fit->SetParameters(Intercept_guess, gradient_guess);
   NoPHD->Fit("NoPHD_fit", "R");

   TF1 *PHD_fit = new TF1("PHD_fit", "[0] + [1]*x",PHD->GetXaxis()->GetXmin(),PHD->GetXaxis()->GetXmax());
   double Intercept_guess2 = 0;
   double gradient_guess2 = -1;
   PHD_fit->SetParameters(Intercept_guess2, gradient_guess2);
   PHD->Fit("PHD_fit", "B");


   NoPHD->Draw("AP");
   line->Draw("same");
   known->Draw("E2,same");
   PHD->Draw("SameP");
   NoPHD_fit->Draw("Same L");
   PHD_fit->SetLineColor(kBlue);
   PHD_fit->Draw("Same L");


   TLegend* leg = new TLegend(0.65,0.67,0.84,0.89);
   leg->AddEntry(NoPHD,"No PH correction residual");
   leg->AddEntry(PHD,"PH corrected residual");
   leg->AddEntry(NoPHD_fit,"No PH correction residual fit");
   leg->AddEntry(PHD_fit,"PH correction residual fit");
   leg->AddEntry(known,"Literature Uncertainty","F");
   leg->SetBorderSize(0);
   leg->Draw();
   
   c1->RedrawAxis();
   c1->Update();

   c1->SaveAs("Residuals.png");


  return 0;
}

