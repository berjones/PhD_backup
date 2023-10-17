///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// ROOT macro to fit the ELUM counts 
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>
#include <fstream>
#include "TMath.h"  

/// input hists file from ISSSort
std::string hists_file = "22Ne_R8_to_R15_sumhists_20230622_004428_Al_deadlayer_0004_arraydist_129p1_alpha_recalibrated.root"; 

void ELUM_fitter(){

    TFile *infile = new TFile( hists_file.data() );
    // access the RecoilMode subdir
    TDirectory *dir = (TDirectory*)infile->Get("ElumDetector");
    // access the Ex_vs_z plot
    TH1D *elum_recoil = (TH1D*)dir->Get("elum_recoil");

    /// Fit two gaussians + 1 - erf function
    // FIT ---------------------------------------------------------------------------------------------------------------------------
    TF1 *Dbl_gauss_logistic = new TF1("Dbl_gauss_logistic",
    "[0]*exp(-0.5*((x-[1])/[2])^2) + [3]*exp(-0.5*((x-[4])/[5])^2) + [6]*(1/(1+exp(-[7]*(x-[8]))))",
    elum_recoil->GetXaxis()->GetXmin(), elum_recoil->GetXaxis()->GetXmax());
    // Guess at the params and initialise
    double Amplitude1_guess = 400;
    Dbl_gauss_logistic->SetParLimits(0, 0, 1e3);
    double mean1_guess = 172.1;
    Dbl_gauss_logistic->SetParLimits(1, 0, 200);
    double sigma1_guess = 100;
    //Dbl_gauss_linear_erf->SetParLimits(2, sigma1_min, sigma1_max);
    double Amplitude2_guess = 5e3;
    double mean2_guess = 313.1;
    double sigma2_guess= 70;
    double logistic_amplitude_guess = 3;
    double logistic_slope_guess = 0.05;
    double logistic_midpoint_guess = 200;
    Dbl_gauss_logistic->SetParLimits(7, 0, 0.1);
    Dbl_gauss_logistic->SetParLimits(8, 100, 400);

    Dbl_gauss_logistic->SetParameters(
    Amplitude1_guess, mean1_guess, sigma1_guess,
    Amplitude2_guess, mean2_guess, sigma2_guess,
    logistic_amplitude_guess, logistic_slope_guess, logistic_midpoint_guess);
    // Set the range of the fit 
    double xmin = 0;
    double xmax = 600;
    Dbl_gauss_logistic->SetRange(xmin, xmax);

    
    TCanvas *canvas = new TCanvas("canvas", "Elum recoil fit", 800, 600);

    //canvas->SetLogy();

    elum_recoil->Fit("Dbl_gauss_logistic","R");

    // Draw the histogram
    elum_recoil->GetXaxis()->SetRangeUser(0, 700);
    elum_recoil->SetStats(0);
    elum_recoil->Draw("hist");

    //Draw the fits
    Dbl_gauss_logistic->Draw("Same");

    // Create two separate Gaussian functions
    TF1 *Gaussian1 = new TF1("Gaussian1", "[0]*exp(-0.5*((x-[1])/[2])^2)", xmin, xmax);
    TF1 *Gaussian2 = new TF1("Gaussian2", "[0]*exp(-0.5*((x-[1])/[2])^2)", xmin, xmax);
    TF1 *bkg_logistic = new TF1("bkg_logistic", "[0]*(1/(1+exp(-[1]*(x-[2]))))", xmin, xmax);


    // Set parameters of Gaussian functions from the fit result
    Gaussian1->SetParameters(Dbl_gauss_logistic->GetParameter(0), Dbl_gauss_logistic->GetParameter(1), Dbl_gauss_logistic->GetParameter(2));
    Gaussian2->SetParameters(Dbl_gauss_logistic->GetParameter(3), Dbl_gauss_logistic->GetParameter(4), Dbl_gauss_logistic->GetParameter(5));
    bkg_logistic->SetParameters(Dbl_gauss_logistic->GetParameter(6), Dbl_gauss_logistic->GetParameter(7), Dbl_gauss_logistic->GetParameter(8));

    // Draw Gaussian functions
    Gaussian1->SetLineColor(kBlue+1);
    Gaussian2->SetLineColor(kMagenta-3);
    bkg_logistic->SetLineColor(kGreen+2);
    Dbl_gauss_logistic->SetLineColor(kRed);

    Gaussian1->Draw("same");
    Gaussian2->Draw("same");
    bkg_logistic->Draw("same");
    
    canvas->SaveAs("Elum_recoil_fit.png");
    //infile->Close();
    //canvas->Close();
}