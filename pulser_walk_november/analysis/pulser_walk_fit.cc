/// \file
/// \ingroup tutorial_spectrum

/// Macro to perform analysis on pulser walk 
/// Tests for linerity and corresponding ADC value for a zero keV event 

/// Takes in 4 summed root files:
//--Even channel, low energies
//--Odd channel, low energies
//--Even channel, higher energies
//--Odd channel, higher energies

/// Performs analysis for each summed file,
/// then combines (low + high) for the odd and even

// Step 1: Take projections of each channel and perform subsequent analysis on each channel

// Step 2: Fit each peak in the projection (maybe rob some of patricks code for this or maybe hdtv has a feature) using a peak finding algorithm
// (Step 2.5: for the low + high added runs I will need to define cut off points for the "good" data)

// Step 3: Plot the centroids 

// Step 4:

// Step 5:

#include <iostream>
#include <string>
#include <TFile.h>
#include <TH2.h>
#include <TH1D.h>
#include "TCanvas.h"
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TVirtualFitter.h"
#include "TFile.h"

int strip_threshold( TH1 *h ) {
	
	int thres;
	int maxbin = h->GetMaximumBin();
	double maxval = h->GetBinContent(maxbin);
	double curval;
	
	int binlimit = 100;
	double peakfraction = 0.001;

	for( int i = maxbin+1; i < maxbin + binlimit; ++i ){
		
		curval = h->GetBinContent(i);
		thres = h->GetBinCenter(i);
		if( curval < maxval*peakfraction ) break;
					
	}
		
	return thres;

}

// Comment out the line below, if you want "peaks' heights".
// Uncomment the line below, if you want "peaks' areas".
#define __PEAKS_C_FIT_AREAS__ 1 /* fit peaks' areas */

Int_t npeaks = 30;

Double_t fpeaks(Double_t *x, Double_t *par) {
    Double_t result = par[0] + par[1] * x[0];
    for (Int_t p = 0; p < npeaks; p++) {
        Double_t norm = par[3 * p + 2]; // "height" or "area"
        Double_t mean = par[3 * p + 3];
        Double_t sigma = par[3 * p + 4];
#if defined(__PEAKS_C_FIT_AREAS__)
        norm /= sigma * (TMath::Sqrt(TMath::TwoPi())); // "area"
#endif /* defined(__PEAKS_C_FIT_AREAS__) */
        result += norm * TMath::Gaus(x[0], mean, sigma);
    }
    return result;
}


void autoFit(const char *rootFileName, const char *histogramName, Int_t np = 10, Int_t NpeaksToExtrapolate = 20) {
    Int_t p;
    std::vector<Double_t> xpValues;
    std::vector<Double_t> CentroidVals;
    std::vector<Double_t> CentroidErrors;
    Double_t par[3000];
    TFile *file = new TFile(rootFileName, "READ");

    // Check if the file is open successfully
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Unable to open file " << rootFileName << std::endl;
        return;
    }

    gROOT->SetBatch(kTRUE);

    // Retrieve the specified histogram from the file
    TH1 *h = dynamic_cast<TH1*>(file->Get(histogramName));
    TH1 *h_unfiltered = dynamic_cast<TH1*>(file->Get(histogramName));

    // Check if the histogram is retrieved successfully
    if (!h) {
        std::cerr << "Error: Unable to retrieve histogram " << histogramName << " from file " << rootFileName << std::endl;
        file->Close();
        return;
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 10, 10, 1000, 900);
    c1->Divide(1, 2);
    c1->cd(1);

    // Clone the original histogram
    TH1F *h_thres = (TH1F*)h->Clone("h_thres");

    // Get the x threshold
    int xThreshold = strip_threshold(h);
    cout << "Strip threshold is: " << strip_threshold(h) << endl;

    // Exclude data below the x threshold
    for (int i = 1; i <= h_thres->GetNbinsX(); ++i) {
        double binCenter = h_thres->GetBinCenter(i);

        // Set the bin content to zero for bins below the x threshold
        if (binCenter < xThreshold) {
            h_thres->SetBinContent(i, 0);
        }
    }
    
    h_thres->SetStats(0);
    TH1F *h2 = (TH1F*)h_thres->Clone("h2");

    // Use TSpectrum to find the peak candidates
    TSpectrum *s = new TSpectrum(2 * npeaks);
    Int_t nfound = s->Search(h_thres, 2.5, "", 0.05);
    printf("Found %d candidate peaks to fit\n", nfound);

    h_unfiltered->SetLineColor(kGray);
    h->Draw("same HIST");

    h_thres->SetLineColor(kBlue);
    h_thres->Draw("same HIST");

    // Estimate background using TSpectrum::Background
    TH1 *hb = s->Background(h, 20, "same");
    if (hb) c1->Update();
    if (np < 0) {
        file->Close();
        return;
    }


    // Estimate linear background using a fitting method
    //estimate linear background using a fitting method
    c1->cd(2);
    TF1 *fline = new TF1("fline","pol1",xThreshold,1275);
    //h->Fit("fline","qn");
    // Loop on all found peaks. Eliminate peaks at the background level
    fline->SetParameter(0, 0);
    fline->SetParameter(1, 0);
    par[0] = fline->GetParameter(0);
    par[1] = fline->GetParameter(1);
    npeaks = 0;
    Double_t *xpeaks;
    xpeaks = s->GetPositionX();
    for (p = 0; p < nfound; p++) {
        Double_t xp = xpeaks[p];
        Int_t bin = h_thres->GetXaxis()->FindBin(xp);
        Double_t yp = h_thres->GetBinContent(bin);
        if (yp - TMath::Sqrt(yp) < fline->Eval(xp)) continue;
        par[3 * npeaks + 2] = yp; // "height"
        par[3 * npeaks + 3] = xp; // "mean"
        par[3 * npeaks + 4] = 1.0;  // "sigma"
#if defined(__PEAKS_C_FIT_AREAS__)
        par[3 * npeaks + 2] *= par[3 * npeaks + 4] * (TMath::Sqrt(TMath::TwoPi())); // "area"
#endif /* defined(__PEAKS_C_FIT_AREAS__) */
        npeaks++;

        // Add centroid value to the TGraph
        xpValues.push_back(xp);
    }
    printf("Found %d useful peaks to fit\n", npeaks);
    printf("Now fitting: Be patient\n");
    TF1 *fit = new TF1("fit", fpeaks, 0, 1500, 2 + 3 * npeaks);
    // We may have more than the default 25 parameters
    TVirtualFitter::Fitter(h2, 10 + 3 * npeaks);
    fit->SetParameters(par);
    fit->SetParameter(0, 0);
    fit->SetParameter(1, 0);
    fit->SetNpx(1000);

    for (Int_t p = 0; p < npeaks; p++) {
        // Index of sigma parameter
        Int_t sigmaIndex = 3 * p + 4;

        // Constrain sigma to [0.5, 3.5]
        fit->SetParLimits(sigmaIndex, 0.5, 1.5);

        // Index of mean (xp) parameter
        Int_t meanIndex = 3 * p + 3;

        // Get the determined value of xp from xpeaks
        Double_t xpValue = xpeaks[p];

        // Constrain mean to be within ±10 of xpValue
        fit->SetParLimits(meanIndex, xpValue - 10, xpValue + 10);
    }

    h2->Fit("fit", "R+");

    // TFile *fitResultFile = new TFile(Form("fit_results/%s_fit_result.root", histogramName), "RECREATE");
    // fit->Write("fitFunction");
    // h2->Write();
    // fitResultFile->Close();

    // Save the canvas to a file in the "plots" folder
    c1->SaveAs(Form("plots/%s_fit_result.png", histogramName));

    // Create a new canvas for the centroid graph
    std::sort(xpValues.begin(), xpValues.end());

    TCanvas *c2 = new TCanvas("c2", "c2", 0, 0, 1000, 800);
    c2->SetLeftMargin(0.14);
    c2->SetBottomMargin(0.1);
    // Create a new TGraph for the centroid values
    TGraphErrors *centroidGraph = new TGraphErrors();
    centroidGraph->SetTitle("DAC vs ADC");
    centroidGraph->GetXaxis()->SetTitle("ADC value (arb.)");
    centroidGraph->GetYaxis()->SetTitle("DAC value (arb.)");
    centroidGraph->SetMarkerColor(kBlue);
    centroidGraph->SetMarkerStyle(20);

    // Loop on all found peaks and store centroid values and errors in a 2D vector plus the DAC voltage 
    std::vector<std::vector<Double_t>> CentroidData;
    std::vector<Double_t> DAC_values;
    for (p = 0; p < NpeaksToExtrapolate; p++) {
        Double_t centroid = fit->GetParameter(3 * p + 3);  // "mean"
        Double_t centroidError = fit->GetParError(3 * p + 3);  // Error on "mean"
        CentroidData.push_back({centroid, centroidError});

        Double_t DAC_val = 4 + p; // smallest value corresponded to an amplitude of 4 bits
        DAC_val /= 128; // get in terms of volt/bit 
        DAC_values.push_back(DAC_val);
    }

    // Sort the 2D vector based on centroid values
    std::sort(CentroidData.begin(), CentroidData.end(), [](const auto &a, const auto &b) {
        return a[0] < b[0];
    });

    // // Create a new TGraphErrors for the centroid values
    // for (size_t i = 0; i < CentroidData.size(); i++) {
    //     centroidGraph->SetPoint(i, CentroidData[i][0], DAC_values[i]);
    //     centroidGraph->SetPointError(i, CentroidData[i][1], 0);  // Assuming y errors are negligible
    // }


    // Create a new TGraphErrors for the xP values
    for (size_t i = 0; i < CentroidData.size(); i++) {
        centroidGraph->SetPoint(i, xpValues[i], DAC_values[i]);
        centroidGraph->SetPointError(i, xpValues[i]*0.0001, 0);  // Assuming y errors are negligible
    }

    // Dynamically set fit range based on CentroidData
    double minRange = xpValues.front() - 400.0; // Extend by an additional -200
    double maxRange = xpValues.back();
    double maxDac = DAC_values.back() + 0.02;

    centroidGraph->GetXaxis()->SetRangeUser(100, maxRange);
    centroidGraph->GetYaxis()->SetRangeUser(-0.01, maxDac);
    centroidGraph->GetXaxis()->SetLimits(0, maxRange);

    centroidGraph->Draw("APL");

    // Create a TF1 object (polynomial fit function)
    TF1 *polynomialFit = new TF1("polynomialFit", "pol1");

    // Set initial parameter estimates
    polynomialFit->SetParameter(0, 0.005);  // p0
    polynomialFit->SetParameter(1, 0.00002); // p1

    // Set fit range
    polynomialFit->SetRange(minRange, maxRange);

    // Perform the fit
    centroidGraph->Fit("polynomialFit", "R");

    // Optionally retrieve the fit results
    double slope = polynomialFit->GetParameter(1);
    double intercept = polynomialFit->GetParameter(0);

    // Access the uncertainties on the fitted parameters
    double slopeError = polynomialFit->GetParError(1);
    double interceptError = polynomialFit->GetParError(0);

    // Print the fitted parameters and uncertainties
    cout << "Fit Result:" << endl;
    cout << "Slope: " << slope << " +/- " << slopeError << endl;
    cout << "Intercept: " << intercept << " +/- " << interceptError << endl;

    // Calculate the x position for y = 0 (x-intercept)
    double x_intercept = -intercept / slope;
    double x_intercept_error = x_intercept * sqrt((interceptError / intercept) * (interceptError / intercept) +
                                                (slopeError / slope) * (slopeError / slope));

    // Print the x position for y = 0 and its uncertainty
    cout << "Offset (x position for y=0): " << x_intercept << " +/- " << x_intercept_error << endl;

    c2->SaveAs(Form("plots/%s_centroid_values.png", histogramName));

    // Clean up
    delete c1;
    delete c2;
    delete file;
}



void makeProjections(const std::string& inputFileName, bool DoAutofit = false, bool writeEvenChannels = true) {
    TFile *inputFile = new TFile(inputFileName.c_str());

    std::string outputFileName = "proj_" + inputFileName;
    TFile *outputFile = new TFile(outputFileName.c_str(), "RECREATE");

    std::string histName;
    std::string asicName;
    std::string asicNameproj;
    TH2F* h2;

    // Loop over all modules
    for (int mod = 0; mod < 3; ++mod) {
        // Loop over all asics
        for (int asic = 0; asic < 6; ++asic) {
            // Loop over all channels
            for (int i = 0; i < 128; ++i) {
                if (asic == 1 || asic == 4) {
                    if (i < 11) continue;
                    if (i > 21 && i < 28) continue;
                    if (i > 38 && i < 89) continue;
                    if (i > 99 && i < 106) continue;
                    if (i > 116) continue;
                }

                // Get histogram from file
                histName = "asic_hists/module_" + std::to_string(mod);
                histName += "/asic_" + std::to_string(mod) + "_";
                histName += std::to_string(asic);

                asicName = "asic_" + std::to_string(mod) + "_";
                asicName += std::to_string(asic);

                h2 = dynamic_cast<TH2F*>(inputFile->Get(histName.c_str()));

                if (!h2) {
                    std::cerr << "Error: Histogram not found - " << histName << std::endl;
                    continue;
                }

                // Create y projection histogram for each x bin
                asicNameproj = (asicName + "_" + std::to_string(i));
                TH1D* projection = h2->ProjectionY(asicNameproj.c_str(), i + 1, i + 1);
                projection->GetXaxis()->SetRangeUser(0, 1500);

                if ((i % 2 == 0 && writeEvenChannels) || (i % 2 != 0 && !writeEvenChannels)) {
                    outputFile->cd();
                    projection->Write();

                    if (DoAutofit == true) {
                        autoFit(outputFileName.c_str(), asicNameproj.c_str());
                    }

                    else{
                        continue;
                    }

                }
                
            }
        }
    }

    // Close files
    outputFile->Close();
    inputFile->Close();
}

void projRun() {
    // Files to be processed 
    makeProjections("R197add_EvenHigh.root", false, true);
    makeProjections("R198add_OddHigh.root", false, false);
    makeProjections("R199_EvenLow.root", false, true);
    makeProjections("R200_OddLow.root", false, false);
}

void runFit_test(){
    const char* fileName = "proj_R197add_EvenHigh.root";
    const char* histName1 = "asic_0_0_12";
    const char* histName2 = "asic_1_2_24";
    const char* histName3 = "asic_2_5_120";
    const char* histName4 = "asic_2_2_120";
    const char* histName5 = "asic_2_4_14";
    const char* histName6 = "asic_2_0_28";


    //fitPeaksInRootFile(fileName, histName);
    autoFit(fileName, histName1);
    autoFit(fileName, histName2);
    // autoFit(fileName, histName3);
    // autoFit(fileName, histName4);
    // autoFit(fileName, histName5);
    // autoFit(fileName, histName6);
}

