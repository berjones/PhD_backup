#include <iostream>
#include <fstream>

void p_recoil() {
    const int numxbins = 1100, numybins = 800, nummods = 3, numrows = 4;

    double fit_vals[13][nummods][numrows];
    TFile* infile = new TFile("22Ne_R8_to_R15_sumhists_20231014_014850_arraydist_135.root");
    ofstream outfile("p-recoil_tw.cal");

    TH2F* tw_recoil_array[nummods][numrows];

    // Initialize histograms
    for (int j = 0; j < nummods; j++) {
        for (int k = 0; k < numrows; k++) {
            tw_recoil_array[j][k] = new TH2F();
        }
    }

    // MAIN LOOP
    for (int j = 0; j < nummods; j++) {
        for (int k = 0; k < numrows; k++) {

            // READ IN HISTOGRAM
            tw_recoil_array[j][k] = (TH2F*)infile->Get(Form("Timing/tw_recoil_array_hit0_mod_%i_row%i", j, k));

            // FIT GAUSSIAN TO PROJECTIONS
            TCanvas* canvas = new TCanvas();
            tw_recoil_array[j][k]->GetYaxis()->SetRangeUser(500, 8000);

            // Number of projections
            int numProjections = 10;
            TH1D* projections[numProjections];
            double projectionYValues[numProjections];
            double projectionXValues[numProjections];

            for (int i = 0; i < numProjections; i++) {
                double yMin = 500 + (i * (8000 - 500) / numProjections);
                double yMax = 500 + ((i + 1) * (8000 - 500) / numProjections);
                projections[i] = tw_recoil_array[j][k]->ProjectionX(Form("Projection_%i_%i_%i", j, k, i), tw_recoil_array[j][k]->GetYaxis()->FindBin(yMin), tw_recoil_array[j][k]->GetYaxis()->FindBin(yMax));
                //projections[i]->Scale(1.0 / projections[i]->Integral());  // Normalize the projection
                projections[i]->Draw("SAME");

                // Get middle y-value of the projection
                projectionYValues[i] = (yMin + yMax) / 2.0;

                int maxBin = projections[i]->GetMaximumBin();
                double maxXValue = projections[i]->GetXaxis()->GetBinCenter(maxBin);
                projectionXValues[i] = maxXValue;

                // Save Gaussian fit
                TString fitCanvasName = Form("projections/mod_%d_row_%d_proj_%i.png", j, k, i);
                TCanvas* fitCanvas = new TCanvas();
                projections[i]->Draw();

                // Draw a marker at the maximum bin position
                TMarker* markerX = new TMarker(maxXValue, maxBin, 20);
                markerX->SetMarkerSize(2);
                markerX->SetMarkerColor(kRed);
                markerX->Draw("SAME");
                //fitCanvas->SaveAs(fitCanvasName);

                delete fitCanvas;

            }

            // Create a graph for the points (y, x)
            TGraph* graph = new TGraph(numProjections, projectionXValues, projectionYValues);

            // Plot the graph
            graph->SetMarkerStyle(20);
            graph->SetTitle(Form("mod_%d_row_%d Projections", j, k));
            graph->GetYaxis()->SetTitle("Energy (keV)");
            graph->GetXaxis()->SetTitle("Mean Delta t (ns)");
            graph->Draw("AP");

            // FIT
            int lowfit = -1500, highfit = 200;
            TF1* f = new TF1("name", "pol1(0) + expo(2)", lowfit, highfit);
            graph->Fit(f, "R");
            f->Draw("same");

            // Save canvas
            TString canvasName = Form("mod_%d_row_%d_projections.png", j, k);
            canvas->SaveAs(canvasName);

            // Clean up memory
            delete canvas;

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

