void nside_pside() {
    const int nummods = 3, numrows = 4;

    double fit_vals[13][nummods][numrows];
    TFile* infile = new TFile("R8_to_R15_sumevents_for_tw_141023.root");
    ofstream outfile("pn_timewalk.cal");

    TH1F* pn_td[nummods][numrows];
    TH2F* pn_td_Ep[nummods][numrows];
    TH2F* pn_td_En[nummods][numrows];

    // Initialize histograms
    for (int j = 0; j < nummods; j++) {
        for (int k = 0; k < numrows; k++) {
            pn_td[j][k] = new TH1F();
            pn_td_Ep[j][k] = new TH2F();
            pn_td_En[j][k] = new TH2F();
        }
    }

    // MAIN LOOP
    for (int j = 0; j < nummods; j++) {
        for (int k = 0; k < numrows; k++) {
            // READ IN HISTOGRAMS
            pn_td[j][k] = (TH1F*)infile->Get(Form("array/module_%i/pn_td_uncorrected_mod%i_row%i", j, j, k));
            pn_td_Ep[j][k] = (TH2F*)infile->Get(Form("array/module_%i/pn_td_Ep_uncorrected_mod%i_row%i", j, j, k));
            pn_td_En[j][k] = (TH2F*)infile->Get(Form("array/module_%i/pn_td_En_uncorrected_mod%i_row%i", j, j, k));

            // REDUCE NOISE (you can add your noise reduction code here)

            // MAKE PROFILE & FIT
            TCanvas* canvas = new TCanvas();
            TProfile* prof = pn_td_En[j][k]->ProfileX();
            prof->Draw("");
            int lowfit = -500, highfit = 200;
            // FIT
            TF1* f = new TF1("name", "pol1(0) + expo(2)", lowfit, highfit);
            // Set parameter limits
            f->SetParLimits(0, 0, 450);
            f->SetParLimits(1, -0.005, 0.005);
            f->SetParLimits(3, 0, 10);
            prof->Fit(f, "R");
            prof->GetYaxis()->SetRangeUser(500, 8000);
            prof->Draw("same");

            // Save the canvases as PNG
            TString canvasName = Form("mod_%d_row_%d.png", j, k);
            TString profCanvasName = Form("mod_%d_row_%d_prof.png", j, k);
            canvas->SaveAs(canvasName);

            // STORE FIT PARAMETERS
            for (int parnum = 0; parnum < 4; parnum++) {
                fit_vals[parnum][j][k] = f->GetParameter(parnum);
            }

            // Write fit parameters to the output file
            for (int asicnum : {1, 4}) {
                int rownum;
                // Define rownum based on asicnum
                if (asicnum == 1) {
                    rownum = 0;
                } else if (asicnum == 4) {
                    rownum = 2;
                } else {
                    // Skip for other asicnum values
                    continue;
                }

                for (int parnum = 0; parnum < 4; parnum++) {
                    outfile << "asic_" << j << "_" << asicnum << ".Walk" << parnum;
                    outfile << ": " << fit_vals[parnum][j][rownum] << endl;
                }
            }

            // Clean up memory
            delete canvas;
        }  // k - rows
    }      // j - modules

    // Clean up
    outfile.close();
    infile->Close();
}
