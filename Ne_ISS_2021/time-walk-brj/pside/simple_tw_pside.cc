#include <iostream>
#include <fstream>

using namespace std;


void p_recoil() {
    const int nummods = 3, numrows = 4;

    double fit_vals[13][nummods][numrows];
    TFile* infile = new TFile("22Ne_R8_to_R15_sumhists_20231014_014850_arraydist_135.root");
    TFile* cutfile = new TFile("J-cut.root");
    ofstream outfile("p-recoil_tw.cal");

    TH2F* tw_recoil_array[nummods][numrows];

    // Initialize histograms
    for (int j = 0; j < nummods; j++) {
        for (int k = 0; k < numrows; k++) {
            tw_recoil_array[j][k] = new TH2F();
        }
    }

    TProfile* prof = new TProfile();

    // MAIN LOOP
    for (int j = 0; j < nummods; j++) {
        for (int k = 0; k < numrows; k++) {

            // Save the canvases as PNG
            TString canvasName = Form("mod_%d_row_%d.png", j, k);
            TString profCanvasName = Form("mod_%d_row_%d_prof.png", j, k);

            // READ IN HISTOGRAM
            tw_recoil_array[j][k] = (TH2F*)infile->Get(Form("Timing/tw_recoil_array_hit0_mod_%i_row%i", j, k));

            // REDUCE NOISE
            // ... (can add in from annies code if needed)

            //Add graphical cut 
            TCutG* mycut = (TCutG*)cutfile->Get("CUTG");
            tw_recoil_array[j][k]->GetListOfFunctions()->Add(mycut);

            // MAKE XPROFILE & FIT
            TCanvas* profCanvas = new TCanvas();
            prof = tw_recoil_array[j][k]->ProfileX();
            prof->Draw("");
            int lowfit = -500, highfit = 200;
            // FIT
            TF1* f = new TF1("name", "pol1(0) + expo(2)", lowfit, highfit);
            //Set parameter limits
            f->SetParLimits(0,0,450);
            f->SetParLimits(1, -0.005, 0.005);
            //f->SetParLimits(2, 8, 20);
            f->SetParLimits(3,0,10);

            prof->Fit(f, "R");
            prof->Draw();
            profCanvas->SaveAs(profCanvasName);

            TCanvas* canvas = new TCanvas();
            tw_recoil_array[j][k]->GetYaxis()->SetRangeUser(0, 8000);
            tw_recoil_array[j][k]->Draw("colz");
            // Will plot the fitted function over the full range 
            TF1* fExtendedRange = new TF1("name_extended", "pol1(0) + expo(2)", -3000, 500);
            for (int parnum = 0; parnum < 4; parnum++) {
                fExtendedRange->SetParameter(parnum, f->GetParameter(parnum));
            }
            fExtendedRange->Draw("same");
            canvas->SaveAs(canvasName);
           
            // Clean up memory
            delete canvas;
            delete profCanvas;

            // STORE FIT PARAMETERS
            for (int parnum = 0; parnum < 4; parnum++) {
                fit_vals[parnum][j][k] = f->GetParameter(parnum);
            }
        }  // k - rows
    }      // j - modules

    // READ OUT PARAMETERS FOR CALIBRATION FILE
    int rownum;

    // Print the fit parameters to the output file
    for (int j = 0; j < nummods; j++) {
        for (int asicnum = 0; asicnum < 6; asicnum++) {
            if (asicnum == 1 || asicnum == 4) {
                continue;
            } else if (asicnum == 0) {
                rownum = 0;
            } else if (asicnum == 2) {
                rownum = 1;
            } else if (asicnum == 3) {
                rownum = 2;
            } else if (asicnum == 5) {
                rownum = 3;
            }

            for (int parnum = 0; parnum < 4; parnum++) {
                outfile << "asic_" << j << "_" << asicnum << ".Walk" << parnum;
                outfile << ": " << fit_vals[parnum][j][rownum] << endl;
            }
          
        }
    }

    // Clean up
    outfile.close();
    infile->Close();
}