///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// ROOT Macro to determine which channels are on/off
/// Takes projections of all channels in each module and notes how many entries. 
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#include <iostream>
#include <fstream>

/// input hists file from ISSSort
std::string hists_file = "22Ne_R8_to_R15_sumhists_20230622_004428_Al_deadlayer_0004_arraydist_129p1_alpha_recalibrated.root"; 
std::string outfile_name = "Eff_channel_OnOff.dat";

std::vector<int> Entries_mod0; 
std::vector<int> Entries_mod2; 
std::vector<int> Entries_mod1; 
std::vector<double> z_positions_mod0; 
std::vector<double> z_positions_mod1; 
std::vector<double> z_positions_mod2; 


int total_channels; 

// scans across all channels in Ex_vs_z for mod_0, mod_1, mod_2 and determines the number of entries

void channel_scan(){
    /// access the hists file and Ex_vs_z histograms 
    TFile *infile = new TFile( hists_file.data() );
    TH2D *Ex_vs_z_recoil_mod0 = (TH2D*)infile->Get("RecoilMode/module_0/Ex_vs_z_recoil_mod0");
    TH2D *Ex_vs_z_recoil_mod1 = (TH2D*)infile->Get("RecoilMode/module_1/Ex_vs_z_recoil_mod1");
    TH2D *Ex_vs_z_recoil_mod2 = (TH2D*)infile->Get("RecoilMode/module_2/Ex_vs_z_recoil_mod2");


    // Calculate the total number of bins in the histogram
    total_channels = Ex_vs_z_recoil_mod0->GetNbinsX();

    // Module 0 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Size of the arrays containing all the projections, entries and boolean for channel being on/off for each bin of a single module 
    TH1D *Ex_vs_z_recoil_mod0proj[total_channels];
    int NEntries_mod0 = 0;
    std::vector<bool> Channel_OnOff_mod0(total_channels);
    
    // loop over each channel and make a single projection
    // note the number of entries and check if bins are empty 
    for (int i=0; i < total_channels; i++){
        
        // makes projections and gets the # of entries 
        Ex_vs_z_recoil_mod0proj[i] = new TH1D();
        std::string projnamermod0 =  "mod0_Ex_z_proj_" + std::to_string(i);
        Ex_vs_z_recoil_mod0proj[i] = Ex_vs_z_recoil_mod0->ProjectionY(projnamermod0.c_str(), i, i);
        NEntries_mod0 = Ex_vs_z_recoil_mod0proj[i]->GetEntries();
        Entries_mod0.push_back(NEntries_mod0);
        z_positions_mod0.push_back((Ex_vs_z_recoil_mod0->GetXaxis()->GetBinLowEdge(i) + Ex_vs_z_recoil_mod0->GetXaxis()->GetBinUpEdge(i))/2);
        

        // if statement to see if channel is on/off 
        if (NEntries_mod0 > 0 ){
            Channel_OnOff_mod0[i] = true; 
        }
        else if (NEntries_mod0 == 0){
            Channel_OnOff_mod0[i] = false;
        }
        
        // debugging 
        //std::cout << "Number of entries in bin " << i << " is: " << NEntries_mod0 << std::endl;
        //std::cout << "Channel " << i << " is on(1) or off(0) : " << Channel_OnOff_mod0[i] << std::endl;
    }

    // Module 1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // see module 0 for comments 

    TH1D *Ex_vs_z_recoil_mod1proj[total_channels];
    int NEntries_mod1 = 0;
    std::vector<bool> Channel_OnOff_mod1(total_channels);
    
    for (int i=0; i < total_channels; i++){
        Ex_vs_z_recoil_mod1proj[i] = new TH1D();
        std::string projnamermod1 =  "mod1_Ex_z_proj_" + std::to_string(i);
        Ex_vs_z_recoil_mod1proj[i] = Ex_vs_z_recoil_mod1->ProjectionY(projnamermod1.c_str(), i, i);
        NEntries_mod1 = Ex_vs_z_recoil_mod1proj[i]->GetEntries();
        Entries_mod1.push_back(NEntries_mod1);
        z_positions_mod1.push_back((Ex_vs_z_recoil_mod1->GetXaxis()->GetBinLowEdge(i) + Ex_vs_z_recoil_mod1->GetXaxis()->GetBinUpEdge(i))/2);


        if (NEntries_mod1 > 0 ){
            Channel_OnOff_mod1[i] = true; 
        }
        else if (NEntries_mod1 == 0){
            Channel_OnOff_mod1[i] = false;
        }
    }

    // Module 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // see module 0 for comments 

    TH1D *Ex_vs_z_recoil_mod2proj[total_channels];
    int NEntries_mod2 = 0;
    std::vector<bool> Channel_OnOff_mod2(total_channels);
    
    for (int i=0; i < total_channels; i++){
        Ex_vs_z_recoil_mod2proj[i] = new TH1D();
        std::string projnamermod2 =  "mod2_Ex_z_proj_" + std::to_string(i);
        Ex_vs_z_recoil_mod2proj[i] = Ex_vs_z_recoil_mod2->ProjectionY(projnamermod2.c_str(), i, i);
        NEntries_mod2 = Ex_vs_z_recoil_mod2proj[i]->GetEntries();
        Entries_mod2.push_back(NEntries_mod2);
        z_positions_mod2.push_back((Ex_vs_z_recoil_mod2->GetXaxis()->GetBinLowEdge(i) + Ex_vs_z_recoil_mod2->GetXaxis()->GetBinUpEdge(i))/2);


        if (NEntries_mod2 > 0 ){
            Channel_OnOff_mod2[i] = true; 
        }
        else if (NEntries_mod2 == 0){
            Channel_OnOff_mod2[i] = false;
        }
    }

    // Now save all of the entries into an outfile

    std::ofstream Eff_outfile(outfile_name);
    Eff_outfile << "Channel, mod_0_Entries, mod_0_OnOff, mod_1_Entries, mod_1_OnOff, mod_2_Entries, mod_2_OnOff," << std::endl; 
    
    for (int i=0; i < total_channels; i++){
        Eff_outfile << i << ", " << Entries_mod0[i] << ", " << Channel_OnOff_mod0[i] << ", " << Entries_mod1[i] << ", " << Channel_OnOff_mod1[i] << ", " << Entries_mod2[i] << ", " << Channel_OnOff_mod2[i]  << std::endl; 
    }

    Eff_outfile.close();
}


void Entries_plot(){
    channel_scan();

    TGraph *Entries_mod0_plot = new TGraph(total_channels);
    TGraph *Entries_mod1_plot = new TGraph(total_channels);
    TGraph *Entries_mod2_plot = new TGraph(total_channels);

    for (int i=0; i < total_channels; i++){
        Entries_mod0_plot->SetPoint(i, z_positions_mod0[i], Entries_mod0[i]);
        Entries_mod1_plot->SetPoint(i, z_positions_mod1[i], Entries_mod1[i]);
        Entries_mod2_plot->SetPoint(i, z_positions_mod2[i], Entries_mod2[i]);
    }

    TCanvas *canvas = new TCanvas("canvas", "Entries", 800, 600);

    Entries_mod0_plot->GetXaxis()->SetTitle("z (mm)");
    Entries_mod0_plot->GetYaxis()->SetTitle("Entries");
    Entries_mod0_plot->SetTitle("Entries vs z for Ex_vs_z_recoil");
    Entries_mod0_plot->SetLineColor(kRed);
    Entries_mod0_plot->GetXaxis()->SetLimits(-640, -130);

    Entries_mod0_plot->Draw("AL");
    Entries_mod1_plot->SetLineColor(kBlue);
    Entries_mod2_plot->SetLineColor(kGreen);

    Entries_mod1_plot->Draw("same");
    Entries_mod2_plot->Draw("same");

    TLegend* leg = new TLegend(0.71, 0.7, 0.9, 0.85);
    leg->AddEntry(Entries_mod0_plot, "Module 0", "l");
    leg->AddEntry(Entries_mod1_plot, "Module 1", "l");
    leg->AddEntry(Entries_mod2_plot, "Module 2", "l");

    leg->SetBorderSize(0);
    leg->SetTextSize(0.04); // Adjust the text size to your preference
    leg->SetFillStyle(0); // Remove the box fill
    leg->Draw();

    canvas->SaveAs("Entries_vs_channel_Ex_vs_z_recoil_gated.png");

}
