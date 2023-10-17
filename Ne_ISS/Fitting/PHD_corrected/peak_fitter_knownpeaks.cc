#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>


int main() {
  
  // Open the root file
  TFile *file = TFile::Open("../../my_data/PHD_corrected_data_new/hists_grouped_runs/22Ne_R14_to_R40_notall_sumhists_20230127_104449.root");
  
  // Access the subdirectory and histogram
  TDirectory *subdir = (TDirectory*)file->Get("RecoilMode");
  TH1F *Ex_recoil = (TH1F*)subdir->Get("Ex_recoil");
  
  // Create a canvas
  TCanvas *canvas = new TCanvas("canvas","Fit Ex_recoil",800,600);


  // Gaussian + linear (a + bx) background fit 
  // Peak 1 ---------------------------------------------------------------------------------------------------------------------------
  TF1 *gauss_linearbkg_p1 = new TF1("gauss_linearbkg_p1","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]+[4]*x", Ex_recoil->GetXaxis()->GetXmin(), Ex_recoil->GetXaxis()->GetXmax());
  // Guess at the params and initialise
  double Amplitude_guess1 = 400;
  double mean_guess1 = 0;
  double sigma_guess1 = 100;
  double a_guess1 = 20;
  double b_guess1 = 0;
  gauss_linearbkg_p1->SetParameters(Amplitude_guess1, mean_guess1, sigma_guess1, a_guess1, b_guess1);
  // Set the range of the fit 
  double xmin1 = -600;
  double xmax1 = 600;
  gauss_linearbkg_p1->SetRange(xmin1, xmax1);

  // Gaussian + linear (a + bx) background fit 
  // Peak 2 ---------------------------------------------------------------------------------------------------------------------------
  TF1 *gauss_linearbkg_p2 = new TF1("gauss_linearbkg_p2","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]+[4]*x", Ex_recoil->GetXaxis()->GetXmin(), Ex_recoil->GetXaxis()->GetXmax());
  // Guess at the params and initialise
  double Amplitude_guess2 = 700;
  double mean_guess2 = 1000;
  double sigma_guess2 = 100;
  double a_guess2 = 30;
  double b_guess2 = 0;
  gauss_linearbkg_p2->SetParameters(Amplitude_guess2, mean_guess2, sigma_guess2, a_guess2, b_guess2);
  // Set the range of the fit 
  double xmin2 = 600;
  double xmax2 = 1400;
  gauss_linearbkg_p2->SetRange(xmin2, xmax2);

  // double Gaussian + linear (a + bx) background fit 
  // Peaks 3 ---------------------------------------------------------------------------------------------------------------------------
  TF1 *Dbl_gauss_linearbkg_p3 = new TF1("Dbl_gauss_linearbkg_p3","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-[4])/[5])^2)+[6]+[7]*x", Ex_recoil->GetXaxis()->GetXmin(), Ex_recoil->GetXaxis()->GetXmax());
  // Guess at the params and initialise
  double Amplitude1_guess3 = 20;
  double Amplitude1_min3 = 0;
  double Amplitude1_max3 = 40;
  //Dbl_gauss_linearbkg_p3->SetParLimits(0, Amplitude1_min3, Amplitude1_max3);
  double mean1_guess3 = 1701.59;
  double mean1_min3 = 1650;
  double mean1_max3 = 1715;
  //Dbl_gauss_linearbkg_p3->SetParLimits(1, mean1_min3, mean1_max3);
  double sigma1_guess3 = 60;
  double sigma1_min3 = 30;
  double sigma1_max3 = 120;
  //Dbl_gauss_linearbkg_p3->SetParLimits(2, sigma1_min3, sigma1_max3);
  double Amplitude2_guess3 = 30;
  double mean2_guess3 = 1822.26;
  double sigma2_guess3= 50;
  double a_guess3 = 40;
  double b_guess3 = 0;
  Dbl_gauss_linearbkg_p3->SetParameters(Amplitude1_guess3, mean1_guess3, sigma1_guess3, Amplitude2_guess3, mean2_guess3, sigma2_guess3, a_guess3, b_guess3);
  // Set the range of the fit 
  double xmin3 = 1650;
  double xmax3 = 2100;
  Dbl_gauss_linearbkg_p3->SetRange(xmin3, xmax3);

  // // Gaussian + linear (a + bx) background fit 
  // // Peak 4 ---------------------------------------------------------------------------------------------------------------------------
  // TF1 *gauss_linearbkg_p4 = new TF1("gauss_linearbkg_p4","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]+[4]*x", Ex_recoil->GetXaxis()->GetXmin(), Ex_recoil->GetXaxis()->GetXmax());
  // // Guess at the params and initialise
  // double Amplitude_guess4 = 100;
  // double mean_guess4 = 2300;
  // double sigma_guess4 = 100;
  // double a_guess4 = 30;
  // double b_guess4 = 0;
  // gauss_linearbkg_p4->SetParameters(Amplitude_guess4, mean_guess4, sigma_guess4, a_guess4, b_guess4);
  // // Set the range of the fit 
  // double xmin4 = 2000;
  // double xmax4 = 2650;
  // gauss_linearbkg_p4->SetRange(xmin4, xmax4);


  // double Gaussian + linear (a + bx) background fit 
  // Peaks 4 ---------------------------------------------------------------------------------------------------------------------------
  TF1 *Dbl_gauss_linearbkg_p4 = new TF1("Dbl_gauss_linearbkg_p4","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-[4])/[5])^2)+[6]+[7]*x", Ex_recoil->GetXaxis()->GetXmin(), Ex_recoil->GetXaxis()->GetXmax());
  // Guess at the params and initialise
  double Amplitude1_guess4 = 190;
  double Amplitude1_min4 = 0;
  double Amplitude1_max4 = 40;
  //Dbl_gauss_linearbkg_p3->SetParLimits(0, Amplitude1_min3, Amplitude1_max3);
  double mean1_guess4 = 2315;
  double mean1_min4 = 1650;
  double mean1_max4 = 1715;
  //Dbl_gauss_linearbkg_p3->SetParLimits(1, mean1_min3, mean1_max3);
  double sigma1_guess4 = 80;
  double sigma1_min4 = 30;
  double sigma1_max4 = 120;
  //Dbl_gauss_linearbkg_p3->SetParLimits(2, sigma1_min3, sigma1_max3);
  double Amplitude2_guess4 = 30;
  double mean2_guess4 = 2517;
  double sigma2_guess4= 15;
  double a_guess4 = 50;
  double b_guess4 = 0;
  Dbl_gauss_linearbkg_p4->SetParameters(Amplitude1_guess4, mean1_guess4, sigma1_guess4, Amplitude2_guess4, mean2_guess4, sigma2_guess4, a_guess4, b_guess4);
  // Set the range of the fit 
  double xmin4 = 2000;
  double xmax4 = 2600;
  Dbl_gauss_linearbkg_p4->SetRange(xmin4, xmax4);

  // double Gaussian + linear (a + bx) background fit 
  // Peaks 5 ---------------------------------------------------------------------------------------------------------------------------
  TF1 *Trpl_gauss_linearbkg_p5 = new TF1("Trpl_gauss_linearbkg_p5","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-[4])/[5])^2)+ [6]*exp(-0.5*((x-[7])/[8])^2) +[9]+[10]*x", Ex_recoil->GetXaxis()->GetXmin(), Ex_recoil->GetXaxis()->GetXmax());
  // Guess at the params and initialise
  double Amplitude1_guess5 = 700;
  Trpl_gauss_linearbkg_p5->SetParLimits(0,20,1000);
  double mean1_guess5 = 3220;
  double sigma1_guess5 = 100;
  Trpl_gauss_linearbkg_p5->SetParLimits(2,0,200);
  double Amplitude2_guess5 = 700;
  Trpl_gauss_linearbkg_p5->SetParLimits(3,20,1000);
  double mean2_guess5 = 3431;
  double sigma2_guess5 = 100;
  Trpl_gauss_linearbkg_p5->SetParLimits(5,0,200);
  double Amplitude3_guess5 = 700;
  Trpl_gauss_linearbkg_p5->SetParLimits(6,20,1000);
  double mean3_guess5 = 3458;
  double sigma3_guess5 = 100;
  Trpl_gauss_linearbkg_p5->SetParLimits(8,0,200);
  double a_guess5 = 40;
  double b_guess5 = 0;
  Trpl_gauss_linearbkg_p5->SetParameters(Amplitude1_guess5, mean1_guess5, sigma1_guess5, Amplitude2_guess5, mean2_guess5, sigma2_guess5, Amplitude3_guess5, mean3_guess5, sigma3_guess5, a_guess5, b_guess5);
  // Set the range of the fit 
  double xmin5 = 2950;
  double xmax5 = 3650;
  Trpl_gauss_linearbkg_p5->SetRange(xmin5, xmax5);

  // double Gaussian + linear (a + bx) background fit 
  // Peaks 6 ---------------------------------------------------------------------------------------------------------------------------
  TF1 *Dbl_gauss_linearbkg_p6 = new TF1("Qd_gauss_linearbkg_p6","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-[4])/[5])^2)+[3]*exp(-0.5*((x-[4])/[5])^2)+[6]*exp(-0.5*((x-[7])/[8])^2)++[6]+[7]*x", Ex_recoil->GetXaxis()->GetXmin(), Ex_recoil->GetXaxis()->GetXmax());
  // Guess at the params and initialise
  double Amplitude1_guess6 = 500;
  double mean1_guess6 = 3850;
  double sigma1_guess6 = 100;
  double Amplitude2_guess6 = 500;
  double mean2_guess6 = 4000;
  double sigma2_guess6 = 100;
  double a_guess6 = 80;
  double b_guess6 = 0;
  Dbl_gauss_linearbkg_p6->SetParameters(Amplitude1_guess6, mean1_guess6, sigma1_guess6, Amplitude2_guess6, mean2_guess6, sigma2_guess6, a_guess6, b_guess6);
  // Set the range of the fit 
  double xmin6 = 3650;
  double xmax6 = 4200;
  Dbl_gauss_linearbkg_p6->SetRange(xmin6, xmax6);

  // Gaussian + linear (a + bx) background fit 
  // Peak 7 ---------------------------------------------------------------------------------------------------------------------------
  TF1 *gauss_linearbkg_p7 = new TF1("gauss_linearbkg_p7","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]+[4]*x", Ex_recoil->GetXaxis()->GetXmin(), Ex_recoil->GetXaxis()->GetXmax());
  // Guess at the params and initialise
  double Amplitude_guess7 = 140;
  double mean_guess7 = 5200;
  double sigma_guess7 = 100;
  double a_guess7 = 50;
  double b_guess7 = 0;
  gauss_linearbkg_p7->SetParameters(Amplitude_guess7, mean_guess7, sigma_guess7, a_guess7, b_guess7);
  // Set the range of the fit 
  double xmin7 = 4900;
  double xmax7 = 5500;
  gauss_linearbkg_p7->SetRange(xmin7, xmax7);

  // Gaussian + linear (a + bx) background fit 
  // Peak 8 ---------------------------------------------------------------------------------------------------------------------------
  TF1 *gauss_linearbkg_p8 = new TF1("gauss_linearbkg_p8","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]+[4]*x", Ex_recoil->GetXaxis()->GetXmin(), Ex_recoil->GetXaxis()->GetXmax());
  // Guess at the params and initialise
  double Amplitude_guess8 = 90;
  double mean_guess8 = 5700;
  double sigma_guess8 = 50;
  double a_guess8 = 50;
  double b_guess8 = 0;
  gauss_linearbkg_p8->SetParameters(Amplitude_guess8, mean_guess8, sigma_guess8, a_guess8, b_guess8);
  // Set the range of the fit 
  double xmin8 = 5500;
  double xmax8 = 6100;
  gauss_linearbkg_p8->SetRange(xmin8, xmax8);

  
  // Fit the histograms
  Ex_recoil->Fit("gauss_linearbkg_p1","R");
  Ex_recoil->Fit("gauss_linearbkg_p2","R");
  Ex_recoil->Fit("Dbl_gauss_linearbkg_p3","R", "B");
  Ex_recoil->Fit("Dbl_gauss_linearbkg_p4","R");
  Ex_recoil->Fit("Trpl_gauss_linearbkg_p5","R");
  Ex_recoil->Fit("Dbl_gauss_linearbkg_p6","R");
  Ex_recoil->Fit("gauss_linearbkg_p7","R");
  Ex_recoil->Fit("gauss_linearbkg_p8","R");

  // Draw the histogram
  Ex_recoil->GetXaxis()->SetRangeUser(-800, 7000);
  Ex_recoil->SetStats(0);
  Ex_recoil->Draw("HIST");

  //Draw the fits
  gauss_linearbkg_p1->Draw("Same");
  gauss_linearbkg_p2->Draw("Same");
  Dbl_gauss_linearbkg_p3->Draw("Same");
  Dbl_gauss_linearbkg_p4->Draw("Same");
  Trpl_gauss_linearbkg_p5->Draw("Same");
  Dbl_gauss_linearbkg_p6->Draw("Same");
  gauss_linearbkg_p7->Draw("Same");
  gauss_linearbkg_p8->Draw("Same");

  //Draw legend 
  TLegend* leg = new TLegend(0.2,0.7,0.4,0.85);
  leg->AddEntry(Ex_recoil,"Data");
  leg->AddEntry(gauss_linearbkg_p1,"Fit");
  //leg->SetBorderSize(0);
  leg->Draw();
  
  // Output the fit parameters and FWHM

  //Peak 1 ------------------------------------------------------------------------------------------------------------------------
  std::cout << " " << std::endl;
  std::cout << "Peak 1" << std::endl;
  std::cout << "Chi2: " << gauss_linearbkg_p1->GetChisquare() << " with " << gauss_linearbkg_p1->GetNDF() << " NDF" << std::endl;
  std::cout << "Chi2/NDF: " << gauss_linearbkg_p1->GetChisquare()/gauss_linearbkg_p1->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gauss_linearbkg_p1->GetProb() << std::endl;
  std::cout << "Amplitude: " << gauss_linearbkg_p1->GetParameter(0) << " +- "<< gauss_linearbkg_p1->GetParError(0) << std::endl;
  std::cout << "Mean: " << gauss_linearbkg_p1->GetParameter(1) << " +- "<< gauss_linearbkg_p1->GetParError(1) << std::endl;
  std::cout << "Sigma: " << gauss_linearbkg_p1->GetParameter(2) << " +- "<< gauss_linearbkg_p1->GetParError(2) << std::endl;
  std::cout << "Background Intercept: " << gauss_linearbkg_p1->GetParameter(3) << " +- "<< gauss_linearbkg_p1->GetParError(3) << std::endl;
  std::cout << "Background Slope: " << gauss_linearbkg_p1->GetParameter(4) << " +- "<< gauss_linearbkg_p1->GetParError(4) << std::endl;
  double fwhm_p1 = 2 * sqrt(2 * log(2)) * gauss_linearbkg_p1->GetParameter(2);
  std::cout << "FWHM of the peak: " << fwhm_p1 << std::endl;

  //Peak 2 ------------------------------------------------------------------------------------------------------------------------
  std::cout << " " << std::endl;
  std::cout << "Peak 2" << std::endl;
  std::cout << "Chi2: " << gauss_linearbkg_p2->GetChisquare() << " with " << gauss_linearbkg_p2->GetNDF() << " NDF" << std::endl;
  std::cout << "Chi2/NDF: " << gauss_linearbkg_p2->GetChisquare()/gauss_linearbkg_p2->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gauss_linearbkg_p2->GetProb() << std::endl;
  std::cout << "Amplitude: " << gauss_linearbkg_p2->GetParameter(0) << " +- "<< gauss_linearbkg_p2->GetParError(0) << std::endl;
  std::cout << "Mean: " << gauss_linearbkg_p2->GetParameter(1) << " +- "<< gauss_linearbkg_p2->GetParError(1) << std::endl;
  std::cout << "Sigma: " << gauss_linearbkg_p2->GetParameter(2) << " +- "<< gauss_linearbkg_p2->GetParError(2) << std::endl;
  std::cout << "Background Intercept: " << gauss_linearbkg_p2->GetParameter(3) << " +- "<< gauss_linearbkg_p2->GetParError(3) << std::endl;
  std::cout << "Background Slope: " << gauss_linearbkg_p2->GetParameter(4) << " +- "<< gauss_linearbkg_p2->GetParError(4) << std::endl;
  double fwhm_p2 = 2 * sqrt(2 * log(2)) * gauss_linearbkg_p2->GetParameter(2);
  std::cout << "FWHM of the peak: " << fwhm_p2 << std::endl;

  //Peaks 3 ------------------------------------------------------------------------------------------------------------------------
  std::cout << " " << std::endl;
  std::cout << "Peaks 3" << std::endl;
  std::cout << "Chi2: " << Dbl_gauss_linearbkg_p3->GetChisquare() << " with " << Dbl_gauss_linearbkg_p3->GetNDF() << " NDF" << std::endl;
  std::cout << "Chi2/NDF: " << Dbl_gauss_linearbkg_p3->GetChisquare()/Dbl_gauss_linearbkg_p3->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << Dbl_gauss_linearbkg_p3->GetProb() << std::endl;
  std::cout << "Amplitude 1: " << Dbl_gauss_linearbkg_p3->GetParameter(0) << " +- "<< Dbl_gauss_linearbkg_p3->GetParError(0) << std::endl;
  std::cout << "Mean 1: " << Dbl_gauss_linearbkg_p3->GetParameter(1) << " +- "<< Dbl_gauss_linearbkg_p3->GetParError(1) << std::endl;
  std::cout << "Sigma 1: " << Dbl_gauss_linearbkg_p3->GetParameter(2) << " +- "<< Dbl_gauss_linearbkg_p3->GetParError(2) << std::endl;
  std::cout << "Amplitude 2: " << Dbl_gauss_linearbkg_p3->GetParameter(3) << " +- "<< Dbl_gauss_linearbkg_p3->GetParError(3) << std::endl;
  std::cout << "Mean 2: " << Dbl_gauss_linearbkg_p3->GetParameter(4) << " +- "<< Dbl_gauss_linearbkg_p3->GetParError(4) << std::endl;
  std::cout << "Sigma 2: " << Dbl_gauss_linearbkg_p3->GetParameter(5) << " +- "<< Dbl_gauss_linearbkg_p3->GetParError(5) << std::endl;
  std::cout << "Background Intercept: " << Dbl_gauss_linearbkg_p3->GetParameter(6) << " +- "<< Dbl_gauss_linearbkg_p3->GetParError(6) << std::endl;
  std::cout << "Background Slope: " << Dbl_gauss_linearbkg_p3->GetParameter(7) << " +- "<< Dbl_gauss_linearbkg_p3->GetParError(7) << std::endl;
  double fwhm_1_p3 = 2 * sqrt(2 * log(2)) * Dbl_gauss_linearbkg_p3->GetParameter(2);
  std::cout << "FWHM of the peak 1: " << fwhm_1_p3 << std::endl;
  double fwhm_2_p3 = 2 * sqrt(2 * log(2)) * Dbl_gauss_linearbkg_p3->GetParameter(5);
  std::cout << "FWHM of the peak 2: " << fwhm_2_p3 << std::endl;

  //Peaks 4 ------------------------------------------------------------------------------------------------------------------------
  std::cout << " " << std::endl;
  std::cout << "Peaks 4" << std::endl;
  std::cout << "Chi2: " << Dbl_gauss_linearbkg_p4->GetChisquare() << " with " << Dbl_gauss_linearbkg_p4->GetNDF() << " NDF" << std::endl;
  std::cout << "Chi2/NDF: " << Dbl_gauss_linearbkg_p4->GetChisquare()/Dbl_gauss_linearbkg_p4->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << Dbl_gauss_linearbkg_p4->GetProb() << std::endl;
  std::cout << "Amplitude 1: " << Dbl_gauss_linearbkg_p4->GetParameter(0) << " +- "<< Dbl_gauss_linearbkg_p4->GetParError(0) << std::endl;
  std::cout << "Mean 1: " << Dbl_gauss_linearbkg_p4->GetParameter(1) << " +- "<< Dbl_gauss_linearbkg_p4->GetParError(1) << std::endl;
  std::cout << "Sigma 1: " << Dbl_gauss_linearbkg_p4->GetParameter(2) << " +- "<< Dbl_gauss_linearbkg_p4->GetParError(2) << std::endl;
  std::cout << "Amplitude 2: " << Dbl_gauss_linearbkg_p4->GetParameter(3) << " +- "<< Dbl_gauss_linearbkg_p4->GetParError(3) << std::endl;
  std::cout << "Mean 2: " << Dbl_gauss_linearbkg_p4->GetParameter(4) << " +- "<< Dbl_gauss_linearbkg_p4->GetParError(4) << std::endl;
  std::cout << "Sigma 2: " << Dbl_gauss_linearbkg_p4->GetParameter(5) << " +- "<< Dbl_gauss_linearbkg_p4->GetParError(5) << std::endl;
  std::cout << "Background Intercept: " << Dbl_gauss_linearbkg_p4->GetParameter(6) << " +- "<< Dbl_gauss_linearbkg_p4->GetParError(6) << std::endl;
  std::cout << "Background Slope: " << Dbl_gauss_linearbkg_p4->GetParameter(7) << " +- "<< Dbl_gauss_linearbkg_p4->GetParError(7) << std::endl;
  double fwhm_1_p4 = 2 * sqrt(2 * log(2)) * Dbl_gauss_linearbkg_p4->GetParameter(2);
  std::cout << "FWHM of the peak 1: " << fwhm_1_p4 << std::endl;
  double fwhm_2_p4 = 2 * sqrt(2 * log(2)) * Dbl_gauss_linearbkg_p4->GetParameter(5);
  std::cout << "FWHM of the peak 2: " << fwhm_2_p4 << std::endl;

  //Peaks 5 ------------------------------------------------------------------------------------------------------------------------
  std::cout << " " << std::endl;
  std::cout << "Peaks 5" << std::endl;
  std::cout << "Chi2: " << Trpl_gauss_linearbkg_p5->GetChisquare() << " with " << Trpl_gauss_linearbkg_p5->GetNDF() << " NDF" << std::endl;
  std::cout << "Chi2/NDF: " << Trpl_gauss_linearbkg_p5->GetChisquare()/Trpl_gauss_linearbkg_p5->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << Trpl_gauss_linearbkg_p5->GetProb() << std::endl;
  std::cout << "Amplitude 1: " << Trpl_gauss_linearbkg_p5->GetParameter(0) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(0) << std::endl;
  std::cout << "Mean 1: " << Trpl_gauss_linearbkg_p5->GetParameter(1) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(1) << std::endl;
  std::cout << "Sigma 1: " << Trpl_gauss_linearbkg_p5->GetParameter(2) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(2) << std::endl;
  std::cout << "Amplitude 2: " << Trpl_gauss_linearbkg_p5->GetParameter(3) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(3) << std::endl;
  std::cout << "Mean 2: " << Trpl_gauss_linearbkg_p5->GetParameter(4) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(4) << std::endl;
  std::cout << "Sigma 2: " << Trpl_gauss_linearbkg_p5->GetParameter(5) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(5) << std::endl;
  std::cout << "Amplitude 3: " << Trpl_gauss_linearbkg_p5->GetParameter(6) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(6) << std::endl;
  std::cout << "Mean 3: " << Trpl_gauss_linearbkg_p5->GetParameter(7) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(7) << std::endl;
  std::cout << "Sigma 3: " << Trpl_gauss_linearbkg_p5->GetParameter(8) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(8) << std::endl;
  std::cout << "Background Intercept: " << Trpl_gauss_linearbkg_p5->GetParameter(9) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(9) << std::endl;
  std::cout << "Background Slope: " << Trpl_gauss_linearbkg_p5->GetParameter(10) << " +- "<< Trpl_gauss_linearbkg_p5->GetParError(10) << std::endl;
  double fwhm_1_p5 = 2 * sqrt(2 * log(2)) * Trpl_gauss_linearbkg_p5->GetParameter(2);
  std::cout << "FWHM of the peak 1: " << fwhm_1_p5 << std::endl;
  double fwhm_2_p5 = 2 * sqrt(2 * log(2)) * Trpl_gauss_linearbkg_p5->GetParameter(5);
  std::cout << "FWHM of the peak 2: " << fwhm_2_p5 << std::endl;
  double fwhm_3_p5 = 2 * sqrt(2 * log(2)) * Trpl_gauss_linearbkg_p5->GetParameter(8);
  std::cout << "FWHM of the peak 2: " << fwhm_3_p5 << std::endl;

  //Peaks 6 ------------------------------------------------------------------------------------------------------------------------
  std::cout << " " << std::endl;
  std::cout << "Peaks 6" << std::endl;
  std::cout << "Chi2: " << Dbl_gauss_linearbkg_p6->GetChisquare() << " with " << Dbl_gauss_linearbkg_p6->GetNDF() << " NDF" << std::endl;
  std::cout << "Chi2/NDF: " << Dbl_gauss_linearbkg_p6->GetChisquare()/Dbl_gauss_linearbkg_p6->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << Dbl_gauss_linearbkg_p6->GetProb() << std::endl;
  std::cout << "Amplitude 1: " << Dbl_gauss_linearbkg_p6->GetParameter(0) << " +- "<< Dbl_gauss_linearbkg_p6->GetParError(0) << std::endl;
  std::cout << "Mean 1: " << Dbl_gauss_linearbkg_p6->GetParameter(1) << " +- "<< Dbl_gauss_linearbkg_p6->GetParError(1) << std::endl;
  std::cout << "Sigma 1: " << Dbl_gauss_linearbkg_p6->GetParameter(2) << " +- "<< Dbl_gauss_linearbkg_p6->GetParError(2) << std::endl;
  std::cout << "Amplitude 2: " << Dbl_gauss_linearbkg_p6->GetParameter(3) << " +- "<< Dbl_gauss_linearbkg_p6->GetParError(3) << std::endl;
  std::cout << "Mean 2: " << Dbl_gauss_linearbkg_p6->GetParameter(4) << " +- "<< Dbl_gauss_linearbkg_p6->GetParError(4) << std::endl;
  std::cout << "Sigma 2: " << Dbl_gauss_linearbkg_p6->GetParameter(5) << " +- "<< Dbl_gauss_linearbkg_p6->GetParError(5) << std::endl;
  std::cout << "Background Intercept: " << Dbl_gauss_linearbkg_p6->GetParameter(6) << " +- "<< Dbl_gauss_linearbkg_p6->GetParError(6) << std::endl;
  std::cout << "Background Slope: " << Dbl_gauss_linearbkg_p6->GetParameter(7) << " +- "<< Dbl_gauss_linearbkg_p6->GetParError(7) << std::endl;
  double fwhm_1_p6 = 2 * sqrt(2 * log(2)) * Dbl_gauss_linearbkg_p6->GetParameter(2);
  std::cout << "FWHM of the peak 1: " << fwhm_1_p6 << std::endl;
  double fwhm_2_p6 = 2 * sqrt(2 * log(2)) * Dbl_gauss_linearbkg_p6->GetParameter(5);
  std::cout << "FWHM of the peak 2: " << fwhm_2_p6 << std::endl;

  //Peak 7 ------------------------------------------------------------------------------------------------------------------------
  std::cout << " " << std::endl;
  std::cout << "Peak 7" << std::endl;
  std::cout << "Chi2: " << gauss_linearbkg_p7->GetChisquare() << " with " << gauss_linearbkg_p7->GetNDF() << " NDF" << std::endl;
  std::cout << "Chi2/NDF: " << gauss_linearbkg_p7->GetChisquare()/gauss_linearbkg_p7->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gauss_linearbkg_p7->GetProb() << std::endl;
  std::cout << "Amplitude: " << gauss_linearbkg_p7->GetParameter(0) << " +- "<< gauss_linearbkg_p7->GetParError(0) << std::endl;
  std::cout << "Mean: " << gauss_linearbkg_p7->GetParameter(1) << " +- "<< gauss_linearbkg_p7->GetParError(1) << std::endl;
  std::cout << "Sigma: " << gauss_linearbkg_p7->GetParameter(2) << " +- "<< gauss_linearbkg_p7->GetParError(2) << std::endl;
  std::cout << "Background Intercept: " << gauss_linearbkg_p7->GetParameter(3) << " +- "<< gauss_linearbkg_p7->GetParError(3) << std::endl;
  std::cout << "Background Slope: " << gauss_linearbkg_p7->GetParameter(4) << " +- "<< gauss_linearbkg_p7->GetParError(4) << std::endl;
  double fwhm_p7 = 2 * sqrt(2 * log(2)) * gauss_linearbkg_p7->GetParameter(2);
  std::cout << "FWHM of the peak: " << fwhm_p7 << std::endl;

  //Peak 8 ------------------------------------------------------------------------------------------------------------------------
  std::cout << " " << std::endl;
  std::cout << "Peak 8" << std::endl;
  std::cout << "Chi2: " << gauss_linearbkg_p8->GetChisquare() << " with " << gauss_linearbkg_p8->GetNDF() << " NDF" << std::endl;
  std::cout << "Chi2/NDF: " << gauss_linearbkg_p8->GetChisquare()/gauss_linearbkg_p8->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gauss_linearbkg_p8->GetProb() << std::endl;
  std::cout << "Amplitude: " << gauss_linearbkg_p8->GetParameter(0) << " +- "<< gauss_linearbkg_p8->GetParError(0) << std::endl;
  std::cout << "Mean: " << gauss_linearbkg_p8->GetParameter(1) << " +- "<< gauss_linearbkg_p8->GetParError(1) << std::endl;
  std::cout << "Sigma: " << gauss_linearbkg_p8->GetParameter(2) << " +- "<< gauss_linearbkg_p8->GetParError(2) << std::endl;
  std::cout << "Background Intercept: " << gauss_linearbkg_p8->GetParameter(3) << " +- "<< gauss_linearbkg_p8->GetParError(3) << std::endl;
  std::cout << "Background Slope: " << gauss_linearbkg_p8->GetParameter(4) << " +- "<< gauss_linearbkg_p8->GetParError(4) << std::endl;
  double fwhm_p8 = 2 * sqrt(2 * log(2)) * gauss_linearbkg_p8->GetParameter(2);
  std::cout << "FWHM of the peak: " << fwhm_p8 << std::endl;


  // Save the canvas
  canvas->SaveAs("Fits/Fitted__known_peaks.png");
  
  // Close the file
  file->Close();

  return 0;
}