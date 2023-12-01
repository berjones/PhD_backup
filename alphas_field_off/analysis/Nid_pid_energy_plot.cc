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
#include "/home/ben/myPackages/ISS_home/ISSSort/include/ISSEvts.hh"
#include "/home/ben/myPackages/ISS_home/ISSSort/include/Settings.hh"
#include "/home/ben/myPackages/ISS_home/ISSSort/include/Reaction.hh"


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



void plot_EvsPixel(){

    TChain *evt_tree = new TChain("evt_tree");
    evt_tree->Add("../data/R189_*_events.root");

    ISSEvts* issevts = nullptr;
    evt_tree->SetBranchAddress("ISSEvts",&issevts);

    // Create a canvas and histogram
    TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600);
    TH2F *En_vs_pid_distance1 = new TH2F("En_vs_pid_distance1", "Alpha energy vs 1 distance ; p-side strip ; Energy (arb.)", 128, -0.5, 127.5, 500, 0, 10000);
    En_vs_pid_distance1->SetStats(0);
    // Draw the histogram
    evt_tree->Draw("array_event.nen:array_event.pid >> En_vs_pid_distance1", "array_event.mod == 1 && array_event.row == 3 && array_event.nid == 5", "colz");

    // Optionally, save the histogram to a file
    TFile *outputFile = new TFile("nid5_En_vs_pid.root", "RECREATE");
    En_vs_pid_distance1->Write();
    outputFile->Close();

}

void plot_EvsPixeld2(){

    TChain *evt_tree = new TChain("evt_tree");
    evt_tree->Add("../data/R190_*_events.root");

    ISSEvts* issevts = nullptr;
    evt_tree->SetBranchAddress("ISSEvts",&issevts);

    // Create a canvas and histogram
    TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600);
    TH2F *En_vs_pid_distance2 = new TH2F("En_vs_pid_distance2", "Alpha energy vs 1 distance ; p-side strip ; Energy (arb.)", 128, -0.5, 127.5, 500, 0, 10000);
    En_vs_pid_distance2->SetStats(0);
    // Draw the histogram
    evt_tree->Draw("array_event.nen:array_event.pid >> En_vs_pid_distance2", "array_event.mod == 1 && array_event.row == 3 && array_event.nid == 5", "colz");

    // Optionally, save the histogram to a file
    TFile *outputFile = new TFile("nid5_En_vs_pid.root", "update");
    En_vs_pid_distance2->Write();
    outputFile->Close();

}








void plot_EnEp() {
    // Load Libraries containing ISS classes to use in ROOT
    gROOT->SetBatch(kTRUE);

    TChain *ch = new TChain("evt_tree");
    ch->Add("../data/R189_*_events.root");

    ISSEvts* issevts = nullptr;
    ch->SetBranchAddress("ISSEvts", &issevts);

    // Create a canvas to display the histogram
    TCanvas *canvas = new TCanvas("canvas", "n-side energy vs p-side energy", 800, 600);

    // Create a TFile to save histograms
    TFile *outfile = new TFile("En_vs_Ep.root", "recreate");

    // Create an array of histograms
    const int numPIDValues = 128;
    TH2F* En_vs_Ep[numPIDValues];

    // Initialize histograms once
    for (int pidValue = 0; pidValue < numPIDValues; ++pidValue) {
        TString histName = Form("En_vs_Ep_PID_%d", pidValue);
        En_vs_Ep[pidValue] = new TH2F(histName, "n-side energy vs p-side energy", 500, 0, 10000, 500, 0, 10000);
    }

    // Loop over the different values of GetPID()
    for (int pidValue = 0; pidValue < numPIDValues; ++pidValue) {
        // Loop over events
        for (int i = 0; i < ch->GetEntries(); i++) {
            ch->GetEntry(i);

            for (int j = 0; j < issevts->GetArrayMultiplicity(); j++) {
                auto array_evt = issevts->GetArrayEvt(j);

                if (array_evt->GetModule() == 1 && array_evt->GetRow() == 3 &&
                    array_evt->GetNID() == 5 && array_evt->GetPID() == pidValue &&
                    issevts->GetArrayMultiplicity() == 1) {
                    En_vs_Ep[pidValue]->Fill(array_evt->GetNEnergy(), array_evt->GetPEnergy());
                }
            }
        }

        // Customize the histogram and draw it on the canvas
        En_vs_Ep[pidValue]->GetXaxis()->SetTitle("p-side energy");
        En_vs_Ep[pidValue]->GetYaxis()->SetTitle("n-side energy");
        En_vs_Ep[pidValue]->Draw("colz");
        //canvas->Update();  // This is optional, but it updates the canvas to show the latest histogram

        // Write the histogram to the output file
        En_vs_Ep[pidValue]->Write();
    }

    // Delete histograms (prevent memory leaks)
    for (int pidValue = 0; pidValue < numPIDValues; ++pidValue) {
        delete En_vs_Ep[pidValue];
    }

    // Close the output file
    outfile->Close();
}


void fitall_EnEp() {
    // Open the root file containing the histograms
    system("mkdir -p En_Ep_plots");

    gROOT->SetBatch(kTRUE);

    TFile *infile = TFile::Open("En_vs_Ep.root");

    if (!infile || infile->IsZombie()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return;
    }

    // Create an array of histograms
    const int numPIDValues = 128;
    TH2F* En_vs_Ep[numPIDValues];

    // Read histograms from the file
    for (int pidValue = 0; pidValue < numPIDValues; ++pidValue) {
        TString histName = Form("En_vs_Ep_PID_%d", pidValue);
        En_vs_Ep[pidValue] = dynamic_cast<TH2F*>(infile->Get(histName));

        if (!En_vs_Ep[pidValue]) {
            std::cerr << "Error: Could not retrieve histogram " << histName << " from the file." << std::endl;
            return;
        }
    }

    // Perform linear fit and draw histograms
    for (int pidValue = 0; pidValue < numPIDValues; ++pidValue) {
        // Create a linear fit function
        TF1 *linearFitFunc = new TF1("linearFitFunc", "[0]*x", 0, 10000);
        linearFitFunc->SetParameters(0.0, 1.0);
        linearFitFunc->SetLineColor(kRed);

        // Fit the histogram
        En_vs_Ep[pidValue]->Fit(linearFitFunc);

        // Draw the histogram and the fit function
        TCanvas *canvas = new TCanvas("canvas", "Linear Fit", 800, 600);
        En_vs_Ep[pidValue]->Draw("colz");
        linearFitFunc->Draw("same");

        TString savePath = Form("En_Ep_plots/fit_pid_%d.png", pidValue);
        canvas->SaveAs(savePath);

        // Update the canvas
        canvas->Update();
    }
    infile->Close();

}


void fit1_EnEp(int pidValue) {
    // Open the root file containing the histograms
    TFile *infile = TFile::Open("En_vs_Ep.root");

    if (!infile || infile->IsZombie()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return;
    }

    // Retrieve the histogram from the file
    TString histName = Form("En_vs_Ep_PID_%d", pidValue);
    TH2F* histogram = dynamic_cast<TH2F*>(infile->Get(histName));

    if (!histogram) {
        std::cerr << "Error: Could not retrieve histogram " << histName << " from the file." << std::endl;
        infile->Close();
        return;
    }

    // Create a linear fit function
    TF1 *linearFitFunc = new TF1("linearFitFunc", "[0]+[1]*x", 0, 10000);
    linearFitFunc->SetParameters(0.0, 1.0);
    linearFitFunc->SetLineColor(kRed);

    // Fit the histogram
    auto fitResult = histogram->Fit(linearFitFunc, "S");

    // Check fit status
    if (fitResult != 0) {
        std::cerr << "Error: Fit for histogram " << histName << " did not converge." << std::endl;
        return;
    }

    // Draw the histogram and the fit function
    TCanvas *canvas = new TCanvas("canvas", "Linear Fit", 800, 600);
    histogram->Draw("colz");
    linearFitFunc->Draw("same");

    // Update the canvas
    canvas->Update();
}


void run_fit1_EnEp() {
    int pidValueToFit = 0;
    fit1_EnEp(pidValueToFit);
}

void plot_PEn_vs_phi(){
    //Load Libraries containing ISS classes to use in ROOT
    TChain *ch = new TChain("evt_tree");
    ch->Add("../data/R189_*_events.root");

    ISSEvts* issevts = nullptr;
    ch->SetBranchAddress("ISSEvts",&issevts);

    //Pside energy vs n side energy 
    //TH2F *En_vs_Ep = new TH2F("En_vs_Ep", "n-side energy vs p-side energy", 500, 0, 10000, 500, 0, 10000);
    TH2F* PEn_vs_phi = new TH2F("PEn_vs_phi", "P-side energy vs phi angle", 500, 0, 2*M_PI, 1000, 0, 10000);

    
    for (int i=0; i < ch->GetEntries();i++){
        ch->GetEntry(i);

        for( int j=0; j < issevts->GetArrayMultiplicity(); j++ ){

            auto array_evt = issevts->GetArrayEvt(j);
            PEn_vs_phi->Fill( array_evt->GetPhi(), array_evt->GetPEnergy() );

            // if (array_evt->GetModule()==1 && array_evt->GetRow()==3 && array_evt->GetNID()==5 && array_evt->GetPID()==2 && issevts->GetArrayMultiplicity()==1){
            //     En_vs_Ep->Fill( array_evt->GetNEnergy(), array_evt->GetPEnergy() ); 
            // }

        }
    }

    // // Create a canvas to display the histogram
    // TCanvas *canvas = new TCanvas("canvas", "n-side energy vs p-side energy", 800, 600);
    // En_vs_Ep->GetXaxis()->SetTitle("p-side energy");
    // En_vs_Ep->GetYaxis()->SetTitle("n-side energy");
    // En_vs_Ep->Draw("colz");

    TCanvas *canvas2 = new TCanvas("canvas", "P-side energy vs. phi", 800, 600);
    PEn_vs_phi->GetXaxis()->SetTitle("#phi");
    PEn_vs_phi->GetYaxis()->SetTitle("Energy [keV]");
    PEn_vs_phi->Draw("colz");

    TFile *outfile = new TFile("P.root", "recreate");
    //En_vs_Ep->Write();
    PEn_vs_phi->Write();
    outfile->Close();

}

void plot_En_vs_Ep_redundant(){

    TChain *evt_tree = new TChain("evt_tree");
    evt_tree->Add("../data/R189_*_events.root");

    ISSEvts* issevts = nullptr;
    evt_tree->SetBranchAddress("ISSEvts",&issevts);

    // Create a canvas and histogram
    TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600);
    TH2F *histogram = new TH2F("h", "n-side energy vs p-side energy ; p-side energy (arb.); n-side energy (arb.)", 500, 0, 10000, 500, 0, 10000);
    histogram->SetStats(0);
    // Draw the histogram
    evt_tree->Draw("array_event.nen:array_event.pen >> h", "array_event.mod == 1 && array_event.row == 3 && array_event.nid == 5 && array_event.pid == 127 ", "colz");


    // // Optionally, save the histogram to a file
    // TFile *outputFile = new TFile("output.root", "RECREATE");
    // histogram->Write();
    // outputFile->Close();

}

void makeProjections(const std::string& inputFileName, bool DoAutofit = false ) {
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
                asicNameproj = (asicName + "_proj_" + std::to_string(i));
                TH1D* projection = h2->ProjectionY(asicNameproj.c_str(), i + 1, i + 1);
                outputFile->cd();
                projection->Write();

                if (DoAutofit == true) {
                    //autoFit(outputFileName.c_str(), asicNameproj.c_str());
                }

                else{
                    continue;
                }
                

            }
        }
    }

    // Close files
    outputFile->Close();
    inputFile->Close();
}

void projRun() {
    // Specify the input file name
    std::string inputFile = "R189_source_added.root";

    // Call the function to save projections to another ROOT file
    makeProjections(inputFile);

}