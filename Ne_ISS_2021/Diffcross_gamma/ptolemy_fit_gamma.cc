///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// ROOT macro for reading in hdtv, z_data and theta_cm data and then fitting the 
/// angular distributions with read in ptolemy data 
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Usage:
// You simply need to define a new void function for your fit giving all appropriate files 
// i.e. ptolemy DWBA cleaned file, hdtv cleaned file, z_values and theta_values 


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <cmath>

#include "/home/ben/myPackages/ISS_home/ISSSort/include/Settings.hh"
#include "/home/ben/myPackages/ISS_home/ISSSort/include/Reaction.hh"

#include "TVector3.h"
#include "TGraph2D.h"

#include <iostream>


// Defines the data structure for relavent information 
struct DataPoint {
    std::string projection;
    double pos, vol, width;
    double posError, volError, widthError;
    double x_min, x_max, x_avg;
    double theta_min, theta_max, theta_avg;
    double total, on, eff; 
};

// Reads the files and stores them in a data structure. Maps data to projections that exist 
class FileReader {
public:
    FileReader(const std::string& hdtv_file, const std::string& x_value_file, const std::string& theta_file, const std::string& eff_proj_file) {
        dataPoints1 = hdtv_reader(hdtv_file);
        dataPoints2 = x_val_reader(x_value_file);
        dataPoints3 = theta_reader(theta_file);
        dataPoints4 = eff_proj_reader(eff_proj_file);
        matchDataPoints();
    }

    std::vector<DataPoint> getDataPoints1() const {
        return dataPoints1;
    }

    std::vector<DataPoint> getDataPoints2() const {
        return dataPoints2;
    }

    std::vector<DataPoint> getDataPoints3() const {
        return dataPoints3;
    }

    const DataPoint& getDataPoint(const std::string& projection) const {
        return projectionMap.at(projection);
    }

    const std::map<std::string, DataPoint>& getProjectionMap() const {
        return projectionMap;
    }

private:
    std::vector<DataPoint> dataPoints1;
    std::vector<DataPoint> dataPoints2;
    std::vector<DataPoint> dataPoints3;
    std::vector<DataPoint> dataPoints4;
    std::map<std::string, DataPoint> projectionMap;

    double convertToDouble(const std::string& value) {
        double result = 0.0;
        size_t ePos = value.find('e');
        if (ePos != std::string::npos) {
            double base = std::stod(value.substr(0, ePos));
            int exponent = std::stoi(value.substr(ePos + 1));
            result = base * std::pow(10, exponent);
        } else {
            result = std::stod(value);
        }
        return result;
    }

    std::vector<DataPoint> hdtv_reader(const std::string& filename) {
        std::vector<DataPoint> dataPoints;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            std::getline(file, line); // skip the header line
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                DataPoint dp;
                std::getline(ss, dp.projection, ',');
                std::string pos, vol, width;
                std::getline(ss, pos, ',');
                std::getline(ss, vol, ',');
                std::getline(ss, width, ',');

                size_t posBracket = pos.find('(');
                size_t volBracket = vol.find('(');
                size_t widthBracket = width.find('(');

                dp.pos = convertToDouble(pos.substr(0, posBracket));
                dp.vol = convertToDouble(vol.substr(0, volBracket));
                dp.width = convertToDouble(width.substr(0, widthBracket));

                if (posBracket != std::string::npos)
                    dp.posError = convertToDouble(pos.substr(posBracket + 1, pos.length() - posBracket - 2));
                if (volBracket != std::string::npos)
                    dp.volError = convertToDouble(vol.substr(volBracket + 1, vol.length() - volBracket - 2));
                if (widthBracket != std::string::npos)
                    dp.widthError = convertToDouble(width.substr(widthBracket + 1, width.length() - widthBracket - 2));

                dataPoints.push_back(dp);
            }
            file.close();
        }
        return dataPoints;
    }

    std::vector<DataPoint> x_val_reader(const std::string& filename) {
        std::vector<DataPoint> dataPoints;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            std::getline(file, line); // skip the header line
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                DataPoint dp;
                std::getline(ss, dp.projection, ',');
                std::string x_min, x_max, x_avg;
                std::getline(ss, x_min, ',');
                std::getline(ss, x_max, ',');
                std::getline(ss, x_avg, ',');

                dp.x_min = std::stod(x_min);
                dp.x_max = std::stod(x_max);
                dp.x_avg = std::stod(x_avg);

                dataPoints.push_back(dp);
            }
            file.close();
        }
        return dataPoints;
    }

    std::vector<DataPoint> theta_reader(const std::string& filename) {
        std::vector<DataPoint> dataPoints;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            std::getline(file, line); // skip the header line
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                DataPoint dp;
                std::getline(ss, dp.projection, ',');
                std::string theta_min, theta_max, theta_avg;
                std::getline(ss, theta_min, ',');
                std::getline(ss, theta_max, ',');
                std::getline(ss, theta_avg, ',');

                dp.theta_min = std::stod(theta_min);
                dp.theta_max = std::stod(theta_max);
                dp.theta_avg = std::stod(theta_avg);

                dataPoints.push_back(dp);
            }
            file.close();
        }
        return dataPoints;
    }

    std::vector<DataPoint> eff_proj_reader(const std::string& filename) {
        std::vector<DataPoint> dataPoints;
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            std::getline(file, line); // skip the header line
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                DataPoint dp;
                std::getline(ss, dp.projection, ',');
                std::string total, on, eff;
                std::getline(ss, total, ',');
                std::getline(ss, on, ',');
                std::getline(ss, eff, ',');

                dp.total = std::stod(total);
                dp.on = std::stod(on);
                dp.eff = std::stod(eff);

                dataPoints.push_back(dp);
            }
            file.close();
        }
        return dataPoints;
    }

    void matchDataPoints() {
        for (const auto& dp : dataPoints1) {
            auto it1 = std::find_if(dataPoints2.begin(), dataPoints2.end(), [&](const DataPoint& d) {
                return d.projection == dp.projection;
            });

            auto it2 = std::find_if(dataPoints3.begin(), dataPoints3.end(), [&](const DataPoint& d) {
                return d.projection == dp.projection;
            });

            auto it3 = std::find_if(dataPoints4.begin(), dataPoints4.end(), [&](const DataPoint& d) {
                return d.projection == dp.projection;
            });

            if (it1 != dataPoints2.end() && it2 != dataPoints3.end() && it3 != dataPoints4.end()) {
                // Projection exists in all files
                projectionMap.emplace(dp.projection, dp);
                projectionMap.at(dp.projection).x_min = it1->x_min;
                projectionMap.at(dp.projection).x_max = it1->x_max;
                projectionMap.at(dp.projection).x_avg = it1->x_avg;
                projectionMap.at(dp.projection).theta_min = it2->theta_min;
                projectionMap.at(dp.projection).theta_max = it2->theta_max;
                projectionMap.at(dp.projection).theta_avg = it2->theta_avg;
                projectionMap.at(dp.projection).total = it3->total; 
                projectionMap.at(dp.projection).on = it3->on;
                projectionMap.at(dp.projection).eff = it3->eff;
            }
        }
    }
};

// Plot previously measured absolute cross sections from 1967 - calculated with automeris
TGraphErrors* old_diffcross = nullptr;  // Declare gDWBA1 as a global variable
void read_old_diffcross(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> xerr;
    std::vector<double> yerr;

    double xValue, yValue, yerrValue;
    while (file >> xValue >> yValue >> yerrValue) {
        x.push_back(xValue);
        y.push_back(yValue);
        xerr.push_back(0);
        yerr.push_back(yerrValue);
    }
    file.close();

    int numPoints = x.size();

    // Create the TGraphErrors object
    old_diffcross = new TGraphErrors(numPoints, x.data(), y.data(), xerr.data(), yerr.data());

    // Set the graph attributes
    old_diffcross->SetTitle("Differential cross section");
    old_diffcross->GetXaxis()->SetTitle("#theta (deg.)");
    old_diffcross->GetYaxis()->SetTitle("d#sigma / d#Omega (mb/sr)");

    old_diffcross->SetMarkerColor(kBlue);
    old_diffcross->SetMarkerStyle(20);

    // Draw the graph
    //TCanvas canvas("canvas", "1967 Absoulute differential cross section", 800, 600);
    old_diffcross->Draw("p same");

    // Save the plot as a PDF file
    //canvas.SaveAs("Old_diffcross.pdf");
}


// Reads in ptolemy DWBA data 1 
TGraph* gDWBA1 = nullptr;  // Declare gDWBA1 as a global variable
void read_DWBA1(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<double> x;
    std::vector<double> y;
    double xValue, yValue;
    while (file >> xValue >> yValue) {
        x.push_back(xValue);
        y.push_back(yValue);
    }
    file.close();

    int numPoints = x.size();

    // Create the TGraph object
    gDWBA1 = new TGraph(numPoints, x.data(), y.data());
}

// Reads in ptolemy DWBA data 2
TGraph* gDWBA2 = nullptr;  // Declare gDWBA2 as a global variable
void read_DWBA2(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<double> x;
    std::vector<double> y;
    double xValue, yValue;
    while (file >> xValue >> yValue) {
        x.push_back(xValue);
        y.push_back(yValue);
    }
    file.close();

    int numPoints = x.size();

    // Create the TGraph object
    gDWBA2 = new TGraph(numPoints, x.data(), y.data());
}

// Defines the 1 parameter fitting function for a singlet state 
double myDWBA_func1(Double_t* x, Double_t* p) {
    return p[0] * gDWBA1->Eval(x[0]);
}

// Defines the 1 parameter fitting function for a doublet state 
double myDWBA_doublet(Double_t* x, Double_t* p1, Double_t* p2) {
    return p1[0] * gDWBA1->Eval(x[0]) + p2[0] * gDWBA2->Eval(x[0]);
}

// Divides the Yield by the solid angle to give differential cross sec - later needs to be normalised 
double I_to_diffCrossSec(double Yield, double theta_min, double theta_max, double theta_avg, double IN, double Eff){
    double diffCrossSec, d_theta_rad, theta_rad; 
    
    // Normalisation factor - Instantaneous rate * # target nuclei - Can set to 1 for no normalisation
    
    theta_rad = theta_avg;
    theta_rad *= (TMath::Pi()/180);

    d_theta_rad = (theta_max - theta_min);
    d_theta_rad *= (TMath::Pi()/180);

    diffCrossSec = Yield;
    diffCrossSec /= (2.0 * TMath::Pi() * TMath::Sin(theta_rad) * d_theta_rad);
    diffCrossSec /= IN;
    diffCrossSec /= Eff;

    return diffCrossSec;
}

void ptolemy_fit_1_state(const std::string& ptolemy_file, const std::string& hdtv_file, const std::string& zfile, const std::string& theta_file, const std::string& eff_file, const std::string& old_diffcross_file, double IN, int Ex, int l){
    
    // Read in the DWBA data for the signle state from ptolemy 
    read_DWBA1(ptolemy_file);

    // Create a TF1 object using myfunction1
    TF1* myFunc = new TF1("myFunc", myDWBA_func1, 10, 50, 1);

    FileReader Experimental_yields(hdtv_file, zfile, theta_file, eff_file);

    std::map<std::string, DataPoint> projectionMap = Experimental_yields.getProjectionMap();

    int numPoints = projectionMap.size();
    TGraphErrors* EXP_data = new TGraphErrors(numPoints);

    // Initialise variables for the data loop 
    int pointIndex = 0;
    double diffCross = 0.0; 
    double diffCrossErr = 0.0; 
    
    // Iterate over the projectionMap and add the desired data to the TGraphErrors
    for (const auto& entry : projectionMap) {
        const std::string& projection = entry.first;
        const DataPoint& dp = entry.second;

        diffCross = I_to_diffCrossSec(dp.vol, dp.theta_min, dp.theta_max, dp.theta_avg, IN, dp.eff); 
        diffCrossErr = I_to_diffCrossSec(dp.volError, dp.theta_min, dp.theta_max, dp.theta_avg, IN, dp.eff); 

        EXP_data->SetPoint(pointIndex, dp.theta_avg, diffCross);
        EXP_data->SetPointError(pointIndex, 0, diffCrossErr);

        std::cout << "theta_avg: " << dp.theta_avg << std::endl;
        std::cout << "Volume: " << dp.vol << std::endl;


        ++pointIndex;
    }

    TCanvas *canvas = new TCanvas("canvas", "EXP fit", 800, 600);

    EXP_data->Fit(myFunc, "R");

    double chi2_ndf = myFunc->GetChisquare()/myFunc->GetNDF();

    std::cout << "Chi2: " << myFunc->GetChisquare() << " with " << myFunc->GetNDF() << " NDF" << std::endl;
    std::cout << "Chi2/NDF: " << myFunc->GetChisquare()/myFunc->GetNDF() << std::endl;

    EXP_data->Draw("AP");

    EXP_data->SetMarkerStyle(20);
    std::string title = Form("Ex = %d keV - l = %d - Absolute cross section", Ex, l);
    EXP_data->SetTitle(title.c_str());
    EXP_data->GetXaxis()->SetTitle("#theta (deg.)");
    EXP_data->GetYaxis()->SetTitle("d#sigma/d#Omega (mb/sr)");

    myFunc->Draw("same");

    read_old_diffcross(old_diffcross_file);

    //canvas->SetLogy(); // Set the y-axis to logarithmic scale

    TLegend* leg = new TLegend(0.71, 0.74, 0.9, 0.88);
    leg->AddEntry(myFunc, Form("l = %d", l), "l");
    leg->AddEntry(old_diffcross, "Lutz, 1967", "pe");
    leg->AddEntry(EXP_data, "ISS, 2021", "pe");
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04); // Adjust the text size to your preference
    leg->SetFillStyle(0); // Remove the box fill
    leg->Draw();

    // Create a new TLatex object
    TLatex* latex = new TLatex(0.72, 0.70, Form("#chi^{2}/NDF = %.2f", chi2_ndf));
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.038);
    latex->Draw();   

    // Add a second line underneath the first one
    TLatex* latex2 = new TLatex(0.72, 0.70 - 0.038, Form("S = %.2f", myFunc->GetParameter(0)));
    latex2->SetNDC();
    latex2->SetTextFont(42);
    latex2->SetTextSize(0.038);
    latex2->SetTextAlign(12); // Align left and baseline bottom
    latex2->Draw(); 

    std::string Save_img = Form("E%d_l%d_Absdiffcross_Elum_ebis.png", Ex, l);
    canvas->SaveAs(Save_img.c_str());
}

/// Usage - pass the following files for the fit:
/// ptolemy_out_clean, hdtv_fit_clean, z_values, theta_values, normalisation param IN, Ex, L 

void run_fits(){
    // IN normalisation from ELUM analysis
    double IN = 339.2537001638637;

    // Fit the ground state 
    double E0 = 0;
    double L0 = 2;
    ptolemy_fit_1_state("ptolemy_clean/22Ne_2021_E_0.out-clean", "hdtv_fits/hdtv_E0-clean.txt", "theta_z/z_values.dat", "theta_z/theta_values_E0.dat", "projections/Eff_proj_onoff.dat", "Ne_1967_diffcross/abso_0.txt", IN, E0, L0);

    double E1016 = 1016;
    double L1016 = 0;
    ptolemy_fit_1_state("ptolemy_clean/22Ne_2021_E_1016.out-clean", "hdtv_fits/hdtv_E1016-clean.txt", "theta_z/z_values.dat", "theta_z/theta_values_E1016.dat", "projections/Eff_proj_onoff.dat", "Ne_1967_diffcross/abso_1016.txt",IN, E1016, L1016); 

    double E2315 = 2315;
    double L2315 = 2;
    ptolemy_fit_1_state("ptolemy_clean/22Ne_2021_E_2315.out-clean", "hdtv_fits/hdtv_E2315-clean.txt", "theta_z/z_values.dat", "theta_z/theta_values_E2315.dat", "projections/Eff_proj_onoff.dat", "Ne_1967_diffcross/abso_2315.txt",IN, E2315, L2315);  

    double E3220 = 3220;
    double L3220 = 1;
    ptolemy_fit_1_state("ptolemy_clean/22Ne_2021_E_3220.out-clean", "hdtv_fits/hdtv_E3220-clean.txt", "theta_z/z_values.dat", "theta_z/theta_values_E3220.dat", "projections/Eff_proj_onoff.dat", "Ne_1967_diffcross/abso_3220.txt",IN, E3220, L3220); 

    double E3431 = 3431;
    double L3431 = 2;
    ptolemy_fit_1_state("ptolemy_clean/22Ne_2021_E_3431.out-clean", "hdtv_fits/hdtv_E3431-clean.txt", "theta_z/z_values.dat", "theta_z/theta_values_E3431.dat", "projections/Eff_proj_onoff.dat", "Ne_1967_diffcross/abso_3431.txt",IN, E3431, L3431); 
     
}

















// old stuff 

// // Used to print out the mapped data - useful for sanity check/debugging 
// void print_map(){
//     FileReader ground_state_data("hdtv_E0-clean.txt", "z_values.dat", "theta_values.dat");

//     std::map<std::string, DataPoint> projectionMap = ground_state_data.getProjectionMap();

//     // Iterate over the projectionMap and print the desired data for each shared projection
//     for (const auto& entry : projectionMap) {
//         const std::string& projection = entry.first;
//         const DataPoint& dp = entry.second;

//         std::cout << "Projection: " << projection << std::endl;
//         std::cout << "Position: " << dp.pos << std::endl;
//         std::cout << "Volume: " << dp.vol << std::endl;
//         std::cout << "Width: " << dp.width << std::endl;
//         std::cout << "x_min: " << dp.x_min << std::endl;
//         std::cout << "x_max: " << dp.x_max << std::endl;
//         std::cout << "x_avg: " << dp.x_avg << std::endl;
//         std::cout << "theta_min: " << dp.theta_min << std::endl;
//         std::cout << "theta_max: " << dp.theta_max << std::endl;
//         std::cout << "theta_avg: " << dp.theta_avg << std::endl;
//         std::cout << "------------------------" << std::endl;
//     }
// }