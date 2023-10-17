///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// ROOT macro for taking projections of the Ex vs z histogram from ISSSort
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>
#include <fstream>

/// input hists file from ISSSort
std::string hists_file = "22Ne_R14_to_R40_notall_sumhists_20230302_143303.root"; 
std::string outfile_name = "";

/// output directory containing the projections and z position 
std::string proj_dir = ".";

/// Number of rows 
int ROW_N = 4;

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// row bin dimensions and divisions - determines the number of projections for each row
int row1_minbin = 22;
int row1_maxbin = 141;
int row1_div = 8;

int row2_minbin = 148;
int row2_maxbin = 267;
int row2_div = 8;

int row3_minbin = 273;
int row3_maxbin = 392;
int row3_div = 8;

int row4_minbin = 398;
int row4_maxbin = 517;
int row4_div = 8;

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// function takes min and max bin of the row and the number of projections you wish to take.
/// returns the up an lower
int** bin_spacer(int row_bin_low, int row_bin_up, int NUM_proj) {
    int range_size = row_bin_up - row_bin_low;
    int bins_perProjection = range_size / NUM_proj;

    int** bin_lowup = new int*[NUM_proj];

    for (int i = 0; i < NUM_proj; i++) {
        bin_lowup[i] = new int[2];
        bin_lowup[i][0] = 0; // Initialize lower bin position
        bin_lowup[i][1] = 0; // Initialize upper bin position
    }

    for (int i = 0; i < NUM_proj; i++) {
        bin_lowup[i][0] = row_bin_low + bins_perProjection * i + i; // [i][0] gives the lower bin position
        bin_lowup[i][1] = row_bin_low + bins_perProjection * (i + 1) + i; // [i][1] gives the upper bin position

        //std::cout << "bin low: " << bin_lowup[i][0] << ", bin up: " << bin_lowup[i][1] << std::endl;
    }
    for (int i = 0; i < NUM_proj; i++) {
        std::cout << "bin low: " << bin_lowup[i][0] << ", bin up: " << bin_lowup[i][1] << std::endl;
    }

    return bin_lowup;
}

// function that takes min and max bin of a projection 
// returns the x value for the min and max and the average in an array
double* bin_to_xvalue(int bin_low, int bin_up, TH2D* histogram){

    double* x_arr = new double[3];
    x_arr[0] = (histogram->GetXaxis()->GetBinLowEdge(bin_low) + histogram->GetXaxis()->GetBinUpEdge(bin_low))/2; // lower bin x value of the projection
    x_arr[1] = (histogram->GetXaxis()->GetBinLowEdge(bin_up) + histogram->GetXaxis()->GetBinUpEdge(bin_up))/2; // upper bin x value of the projection
    x_arr[2] = ((abs(x_arr[0])-abs(x_arr[1]))/2 + abs(x_arr[1]))*-1; // avg x value 

    // for debugging 
    //std::cout << "(x_low, x_max, x_avg) = (" << x_arr[0] << ", " << x_arr[1] << ", " << x_arr[2] << ")" << std::endl; 


    return x_arr;
}


/// function to take projections (and find z position) of input histogram
/// module can be set to 0, 1, 2 or sum  
void projector(std::string module = "sum"){

    if (module == "sum"){
        /// access the hists file 
        TFile *infile = new TFile( hists_file.data() );
        // access the RecoilMode subdir
        TDirectory *dir = (TDirectory*)infile->Get("RecoilMode");
        // access the Ex_vs_z plot
        TH2D *Ex_vs_z_recoil = (TH2D*)dir->Get("Ex_vs_z_recoil");

        // create histograms for each of the rows and projections
        TH1D *Ex_vs_z_recoil_proj_row1[row1_div];
        TH1D *Ex_vs_z_recoil_proj_row2[row2_div];
        TH1D *Ex_vs_z_recoil_proj_row3[row3_div];
        TH1D *Ex_vs_z_recoil_proj_row4[row4_div];

        // Makes the upper and lower bin ranges for the projections
        int **row1_binrange = bin_spacer(row1_minbin, row1_maxbin, row1_div);
        int **row2_binrange = bin_spacer(row2_minbin, row2_maxbin, row2_div);
        int **row3_binrange = bin_spacer(row3_minbin, row3_maxbin, row3_div);
        int **row4_binrange = bin_spacer(row4_minbin, row4_maxbin, row4_div);
        
        // create and fill the projection histograms 
        for (int i=0; i < row1_div; i++){
            Ex_vs_z_recoil_proj_row1[i] = new TH1D();
            std::string projnamer1 =  "row1_" + std::to_string(i);
            Ex_vs_z_recoil_proj_row1[i] = Ex_vs_z_recoil->ProjectionY(projnamer1.c_str(), row1_binrange[i][0], row1_binrange[i][1]);
        }
        for (int i=0; i < row2_div; i++){
            Ex_vs_z_recoil_proj_row2[i] = new TH1D();
            std::string projnamer2 =  "row2_" + std::to_string(i);
            Ex_vs_z_recoil_proj_row2[i] = Ex_vs_z_recoil->ProjectionY(projnamer2.c_str(), row2_binrange[i][0], row2_binrange[i][1]);
        }
        for (int i=0; i < row3_div; i++){
            Ex_vs_z_recoil_proj_row3[i] = new TH1D();
            std::string projnamer3 =  "row3_" + std::to_string(i);
            Ex_vs_z_recoil_proj_row3[i] = Ex_vs_z_recoil->ProjectionY(projnamer3.c_str(), row3_binrange[i][0], row3_binrange[i][1]);
        }
        for (int i=0; i < row4_div; i++){
            Ex_vs_z_recoil_proj_row4[i] = new TH1D();
            std::string projnamer4 =  "row4_" + std::to_string(i);
            Ex_vs_z_recoil_proj_row4[i] = Ex_vs_z_recoil->ProjectionY(projnamer4.c_str(), row4_binrange[i][0], row4_binrange[i][1]);
        }

        // Save the projections to the "projections.root" file
        TFile root_outfile("projections_allmods.root", "RECREATE");
        for (int i = 0; i < row1_div; i++) {
            Ex_vs_z_recoil_proj_row1[i]->Write();
        }
        for (int i = 0; i < row2_div; i++) {
            Ex_vs_z_recoil_proj_row2[i]->Write();
        }
        for (int i = 0; i < row3_div; i++) {
            Ex_vs_z_recoil_proj_row3[i]->Write();
        }
        for (int i = 0; i < row4_div; i++) {
            Ex_vs_z_recoil_proj_row4[i]->Write();
        }
        root_outfile.Close();
        
        // Save the name (e.g. row1_4), xmin, xmax and xavg of each projection to a dat file
        std::ofstream x_outfile("z_values.dat");
        x_outfile << "projection, x_min, x_max, x_avg" << endl; 

        for (int i = 0; i < row1_div; i++) {
            double *row1_xvals = bin_to_xvalue(row1_binrange[i][0], row1_binrange[i][1], Ex_vs_z_recoil);
            std::string projnamer1 =  "row1_" + std::to_string(i);
            x_outfile << projnamer1.c_str() << ", " << row1_xvals[0] << ", " << row1_xvals[1] << ", " << row1_xvals[2] << endl; 
        }
        for (int i = 0; i < row2_div; i++) {
            double *row2_xvals = bin_to_xvalue(row2_binrange[i][0], row2_binrange[i][1], Ex_vs_z_recoil);
            std::string projnamer2 =  "row2_" + std::to_string(i);
            x_outfile << projnamer2.c_str() << ", " << row2_xvals[0] << ", " << row2_xvals[1] << ", " << row2_xvals[2] << endl;
        }
        for (int i = 0; i < row3_div; i++) {
            double *row3_xvals = bin_to_xvalue(row3_binrange[i][0], row3_binrange[i][1], Ex_vs_z_recoil);
            std::string projnamer3 =  "row3_" + std::to_string(i);
            x_outfile << projnamer3.c_str() << ", " << row3_xvals[0] << ", " << row3_xvals[1] << ", " << row3_xvals[2] << endl;
        }
        for (int i = 0; i < row4_div; i++) {
            double *row4_xvals = bin_to_xvalue(row4_binrange[i][0], row4_binrange[i][1], Ex_vs_z_recoil);
            std::string projnamer4 =  "row4_" + std::to_string(i);
            x_outfile << projnamer4.c_str() << ", " << row4_xvals[0] << ", " << row4_xvals[1] << ", " << row4_xvals[2] << endl;
        } 

        x_outfile.close();

        // Clean up memory
        delete infile;

    }

    if (module == "2"){
        /// access the hists file 
        TFile *infile = new TFile( hists_file.data() );
        // access the RecoilMode subdir
        TDirectory *dir = (TDirectory*)infile->Get("RecoilMode/module_2");
        // access the Ex_vs_z plot
        TH2D *Ex_vs_z_recoil_mod2 = (TH2D*)dir->Get("Ex_vs_z_recoil_mod2");

        // create histograms for each of the rows and projections
        TH1D *Ex_vs_z_recoil_mod2_proj_row1[row1_div];
        TH1D *Ex_vs_z_recoil_mod2_proj_row2[row2_div];
        TH1D *Ex_vs_z_recoil_mod2_proj_row3[row3_div];
        TH1D *Ex_vs_z_recoil_mod2_proj_row4[row4_div];

        // Makes the upper and lower bin ranges for the projections
        int **row1_binrange = bin_spacer(row1_minbin, row1_maxbin, row1_div);
        int **row2_binrange = bin_spacer(row2_minbin, row2_maxbin, row2_div);
        int **row3_binrange = bin_spacer(row3_minbin, row3_maxbin, row3_div);
        int **row4_binrange = bin_spacer(row4_minbin, row4_maxbin, row4_div);
        
        // create and fill the projection histograms 
        for (int i=0; i < row1_div; i++){
            Ex_vs_z_recoil_mod2_proj_row1[i] = new TH1D();
            std::string projnamer1 =  "row1_" + std::to_string(i);
            Ex_vs_z_recoil_mod2_proj_row1[i] = Ex_vs_z_recoil_mod2->ProjectionY(projnamer1.c_str(), row1_binrange[i][0], row1_binrange[i][1]);
        }
        for (int i=0; i < row2_div; i++){
            Ex_vs_z_recoil_mod2_proj_row2[i] = new TH1D();
            std::string projnamer2 =  "row2_" + std::to_string(i);
            Ex_vs_z_recoil_mod2_proj_row2[i] = Ex_vs_z_recoil_mod2->ProjectionY(projnamer2.c_str(), row2_binrange[i][0], row2_binrange[i][1]);
        }
        for (int i=0; i < row3_div; i++){
            Ex_vs_z_recoil_mod2_proj_row3[i] = new TH1D();
            std::string projnamer3 =  "row3_" + std::to_string(i);
            Ex_vs_z_recoil_mod2_proj_row3[i] = Ex_vs_z_recoil_mod2->ProjectionY(projnamer3.c_str(), row3_binrange[i][0], row3_binrange[i][1]);
        }
        for (int i=0; i < row4_div; i++){
            Ex_vs_z_recoil_mod2_proj_row4[i] = new TH1D();
            std::string projnamer4 =  "row4_" + std::to_string(i);
            Ex_vs_z_recoil_mod2_proj_row4[i] = Ex_vs_z_recoil_mod2->ProjectionY(projnamer4.c_str(), row4_binrange[i][0], row4_binrange[i][1]);
        }

        // Save the projections to the "projections.root" file
        TFile root_outfile("projections_mod2.root", "RECREATE");
        for (int i = 0; i < row1_div; i++) {
            Ex_vs_z_recoil_mod2_proj_row1[i]->Write();
        }
        for (int i = 0; i < row2_div; i++) {
            Ex_vs_z_recoil_mod2_proj_row2[i]->Write();
        }
        for (int i = 0; i < row3_div; i++) {
            Ex_vs_z_recoil_mod2_proj_row3[i]->Write();
        }
        for (int i = 0; i < row4_div; i++) {
            Ex_vs_z_recoil_mod2_proj_row4[i]->Write();
        }
        root_outfile.Close();
        

        delete infile;

    }    
}









///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// testing script

// void tester_b(){
//     int** returnedArray = bin_spacer(1, 20, 4);
//     cout << returnedArray[0][1] << endl;
// }
