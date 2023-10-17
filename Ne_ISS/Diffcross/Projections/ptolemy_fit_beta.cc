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
};

// Reads the files and stores them in a data structure. Maps data to projections that exist 
class FileReader {
public:
    FileReader(const std::string& hdtv_file, const std::string& x_value_file, const std::string& theta_file) {
        dataPoints1 = hdtv_reader(hdtv_file);
        dataPoints2 = x_val_reader(x_value_file);
        dataPoints3 = theta_reader(theta_file);
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

    void matchDataPoints() {
        for (const auto& dp : dataPoints1) {
            auto it1 = std::find_if(dataPoints2.begin(), dataPoints2.end(), [&](const DataPoint& d) {
                return d.projection == dp.projection;
            });

            auto it2 = std::find_if(dataPoints3.begin(), dataPoints3.end(), [&](const DataPoint& d) {
                return d.projection == dp.projection;
            });

            if (it1 != dataPoints2.end() && it2 != dataPoints3.end()) {
                // Projection exists in both files
                projectionMap.emplace(dp.projection, dp);
                projectionMap.at(dp.projection).x_min = it1->x_min;
                projectionMap.at(dp.projection).x_max = it1->x_max;
                projectionMap.at(dp.projection).x_avg = it1->x_avg;
                projectionMap.at(dp.projection).theta_min = it2->theta_min;
                projectionMap.at(dp.projection).theta_max = it2->theta_max;
                projectionMap.at(dp.projection).theta_avg = it2->theta_avg;
            }
        }
    }
};

// Used to print out the mapped data - useful for sanity check/debugging 
void print_map(){
    FileReader ground_state_data("hdtv_E0-clean.txt", "z_values.dat", "theta_values.dat");

    std::map<std::string, DataPoint> projectionMap = ground_state_data.getProjectionMap();

    // Iterate over the projectionMap and print the desired data for each shared projection
    for (const auto& entry : projectionMap) {
        const std::string& projection = entry.first;
        const DataPoint& dp = entry.second;

        std::cout << "Projection: " << projection << std::endl;
        std::cout << "Position: " << dp.pos << std::endl;
        std::cout << "Volume: " << dp.vol << std::endl;
        std::cout << "Width: " << dp.width << std::endl;
        std::cout << "x_min: " << dp.x_min << std::endl;
        std::cout << "x_max: " << dp.x_max << std::endl;
        std::cout << "x_avg: " << dp.x_avg << std::endl;
        std::cout << "theta_min: " << dp.theta_min << std::endl;
        std::cout << "theta_max: " << dp.theta_max << std::endl;
        std::cout << "theta_avg: " << dp.theta_avg << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}

// Reads in ptolemy DWBA data 
TGraph* gDWBA = nullptr;  // Declare gDWBA as a global variable
void read_DWBA(const std::string& filename) {
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
    gDWBA = new TGraph(numPoints, x.data(), y.data());

    // Set the graph attributes
    gDWBA->SetTitle("Differential cross section");
    gDWBA->GetXaxis()->SetTitle("#theta (deg.)");
    gDWBA->GetYaxis()->SetTitle("d#sigma / d#Omega (mb/sr)");

    gDWBA->SetLineColor(kRed);
    gDWBA->SetLineWidth(2);

    // Draw the graph
    TCanvas canvas("canvas", "DWBA Graph", 800, 600);
    gDWBA->Draw("AL");

    // Save the plot as a PDF file
    canvas.SaveAs("DWBA_theory.pdf");
}

// Defines the 1 parameter fitting function for a singlet state 
double myfunction1(Double_t* x, Double_t* p) {
    return p[0] * gDWBA->Eval(x[0]);
}

// Divides the Yield by the solid angle to give differential cross sec - later needs to be normalised 
double I_to_diffCrossSec(double Yield, double theta_min, double theta_max, double theta_avg){
    double diffCrossSec, d_theta_rad, theta_rad; 
    
    // Normalisation factor - Instantaneous rate * # target nuclei - Can set to 1 for no normalisation
    double IN_t = 2700.46283256561662;

    theta_rad = theta_avg;
    theta_rad *= (TMath::Pi()/180);

    d_theta_rad = (theta_max - theta_min);
    d_theta_rad *= (TMath::Pi()/180);

    diffCrossSec = Yield;
    diffCrossSec /= (2.0 * TMath::Pi() * TMath::Sin(theta_rad) * d_theta_rad);
    diffCrossSec /= IN_t;

    return diffCrossSec;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// Fitting the states in the excitation spectrum:


void fitting_gs() {
    read_DWBA("22Ne_2022_E_0.out-clean");

    // Create a TF1 object using myfunction1
    TF1* myFunc = new TF1("myFunc", myfunction1, 10, 50, 1);

    FileReader ground_state_data("hdtv_E0-clean.txt", "z_values.dat", "theta_values_E0.dat");

    std::map<std::string, DataPoint> projectionMap = ground_state_data.getProjectionMap();

    int numPoints = projectionMap.size();
    TGraphErrors* GS_data = new TGraphErrors(numPoints);

    int pointIndex = 0;
    double diffCross = 0.0; 
    double diffCrossErr = 0.0; 
    // Iterate over the projectionMap and add the desired data to the TGraphErrors
    for (const auto& entry : projectionMap) {
        const std::string& projection = entry.first;
        const DataPoint& dp = entry.second;

        diffCross = I_to_diffCrossSec(dp.vol, dp.theta_min, dp.theta_max, dp.theta_avg); 
        diffCrossErr = I_to_diffCrossSec(dp.volError, dp.theta_min, dp.theta_max, dp.theta_avg); 

        GS_data->SetPoint(pointIndex, dp.theta_avg, diffCross);
        GS_data->SetPointError(pointIndex, 0, diffCrossErr);

        std::cout << "theta_avg: " << dp.theta_avg << std::endl;
        std::cout << "Volume: " << dp.vol << std::endl;


        ++pointIndex;
    }

    TCanvas *canvas = new TCanvas("canvas", "GS fit", 800, 600);

    GS_data->Fit(myFunc, "R");

    double chi2_ndf = myFunc->GetChisquare()/myFunc->GetNDF();

    std::cout << "Chi2: " << myFunc->GetChisquare() << " with " << myFunc->GetNDF() << " NDF" << std::endl;
    std::cout << "Chi2/NDF: " << myFunc->GetChisquare()/myFunc->GetNDF() << std::endl;

    GS_data->Draw("AP");

    GS_data->SetMarkerStyle(20);
    GS_data->SetTitle("Ex = 0 keV - Absolute cross section");
    GS_data->GetXaxis()->SetTitle("#theta (deg.)");
    GS_data->GetYaxis()->SetTitle("d#sigma/d#Omega (mb/sr)");

    myFunc->Draw("same");

    //canvas->SetLogy(); // Set the y-axis to logarithmic scale

    TLegend* leg = new TLegend(0.71, 0.75, 0.9, 0.92);
    leg->AddEntry(myFunc, "l = 2", "l");
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04); // Adjust the text size to your preference
    leg->SetFillStyle(0); // Remove the box fill
    leg->Draw();

    // Create a new TLatex object
    TLatex* latex = new TLatex(0.72, 0.75, Form("#chi^{2}/NDF = %.2f", chi2_ndf));
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.038);
    latex->Draw();   

    // Add a second line underneath the first one
    TLatex* latex2 = new TLatex(0.72, 0.75 - 0.038, Form("S = %.2f", myFunc->GetParameter(0)));
    latex2->SetNDC();
    latex2->SetTextFont(42);
    latex2->SetTextSize(0.038);
    latex2->SetTextAlign(12); // Align left and baseline bottom
    latex2->Draw(); 

    canvas->SaveAs("GS_ptolemy_fit_Absolute_diffcross.png");
}

void fitting_1016() {
    read_DWBA("22Ne_2022_E_1016.out-clean");

    // Create a TF1 object using myfunction1
    TF1* myFunc = new TF1("myFunc", myfunction1, 10, 50, 1);

    FileReader state_1016_data("hdtv_E1016-clean.txt", "z_values.dat", "theta_values_E1016.dat");

    std::map<std::string, DataPoint> projectionMap = state_1016_data.getProjectionMap();

    int numPoints = projectionMap.size();
    TGraphErrors* data_1016 = new TGraphErrors(numPoints-1);


    int pointIndex = 0;
    double diffCross = 0.0; 
    double diffCrossErr = 0.0; 
    bool isFirstDataPoint = true;
    // Iterate over the projectionMap and add the desired data to the TGraphErrors
    for (const auto& entry : projectionMap) {
        const std::string& projection = entry.first;
        const DataPoint& dp = entry.second;

        if (isFirstDataPoint) {
            isFirstDataPoint = false;
            continue;  // Skip the first data point
        }

        diffCross = I_to_diffCrossSec(dp.vol, dp.theta_min, dp.theta_max, dp.theta_avg); 
        diffCrossErr = I_to_diffCrossSec(dp.volError, dp.theta_min, dp.theta_max, dp.theta_avg); 

        data_1016->SetPoint(pointIndex, dp.theta_avg, diffCross);
        data_1016->SetPointError(pointIndex, 0, diffCrossErr);

        std::cout << "theta_avg: " << dp.theta_avg << std::endl;
        std::cout << "Volume: " << dp.vol << std::endl;


        ++pointIndex;
    }

    TCanvas *canvas = new TCanvas("canvas", "1016 fit", 800, 600);

    data_1016->Fit(myFunc, "R");

    double chi2_ndf = myFunc->GetChisquare()/myFunc->GetNDF();

    std::cout << "Chi2: " << myFunc->GetChisquare() << " with " << myFunc->GetNDF() << " NDF" << std::endl;
    std::cout << "Chi2/NDF: " << myFunc->GetChisquare()/myFunc->GetNDF() << std::endl;

    data_1016->SetMarkerStyle(20);
    data_1016->SetTitle(0);
    data_1016->GetXaxis()->SetTitle("#theta (deg.)");
    data_1016->GetYaxis()->SetTitle("d#sigma/d#Omega (mb/sr)");
    // data_1016->GetXaxis()->SetLimits(15., 45.);
    // data_1016->GetHistogram()->SetMinimum(500);
    data_1016->SetTitle("Ex = 1016.9 keV - Absolute cross section");

    data_1016->Draw("AP");
    myFunc->Draw("same");

    //canvas->SetLogy(); // Set the y-axis to logarithmic scale
    
    TLegend* leg = new TLegend(0.71, 0.75, 0.9, 0.92);
    leg->AddEntry(myFunc, "l = 0", "l");
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04); // Adjust the text size to your preference
    leg->SetFillStyle(0); // Remove the box fill
    leg->Draw();

    // Add a second line underneath the first one
    TLatex* latex2 = new TLatex(0.72, 0.75 - 0.038, Form("S = %.2f", myFunc->GetParameter(0)));
    latex2->SetNDC();
    latex2->SetTextFont(42);
    latex2->SetTextSize(0.038);
    latex2->SetTextAlign(12); // Align left and baseline bottom
    latex2->Draw();     

    // Create a new TLatex object
    TLatex* latex = new TLatex(0.722, 0.75, Form("#chi^{2}/NDF = %.2f", chi2_ndf));
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.038);
    latex->Draw();   
   

    canvas->SaveAs("ptolemy_1016_fit_Absolutediffcross.png");
}


void fitting_2315() {
    read_DWBA("22Ne_2022_E_2315.out-clean");

    // Create a TF1 object using myfunction1
    TF1* myFunc = new TF1("myFunc", myfunction1, 10, 50, 1);

    FileReader state_2315_data("hdtv_E2315-clean.txt", "z_values.dat", "theta_values_E2315.dat");

    std::map<std::string, DataPoint> projectionMap = state_2315_data.getProjectionMap();

    int numPoints = projectionMap.size();
    TGraphErrors* data_2315 = new TGraphErrors(numPoints-1);

    int pointIndex = 0;
    double diffCross = 0.0; 
    double diffCrossErr = 0.0; 
    bool isFirstDataPoint = true;
    // Iterate over the projectionMap and add the desired data to the TGraphErrors
    for (const auto& entry : projectionMap) {
        const std::string& projection = entry.first;
        const DataPoint& dp = entry.second;

        if (isFirstDataPoint) {
            isFirstDataPoint = false;
            continue;  // Skip the first data point
        }

        diffCross = I_to_diffCrossSec(dp.vol, dp.theta_min, dp.theta_max, dp.theta_avg); 
        diffCrossErr = I_to_diffCrossSec(dp.volError, dp.theta_min, dp.theta_max, dp.theta_avg); 

        data_2315->SetPoint(pointIndex, dp.theta_avg, diffCross);
        data_2315->SetPointError(pointIndex, 0, diffCrossErr);

        std::cout << "theta_avg: " << dp.theta_avg << std::endl;
        std::cout << "Volume: " << dp.vol << std::endl;


        ++pointIndex;
    }
    

    TCanvas *canvas = new TCanvas("canvas", "2315 fit", 800, 600);

    data_2315->Fit(myFunc, "R");

    double chi2_ndf = myFunc->GetChisquare()/myFunc->GetNDF();

    std::cout << "Chi2: " << myFunc->GetChisquare() << " with " << myFunc->GetNDF() << " NDF" << std::endl;
    std::cout << "Chi2/NDF: " << myFunc->GetChisquare()/myFunc->GetNDF() << std::endl;

    data_2315->Draw("AP");

    data_2315->SetMarkerStyle(20);
    data_2315->SetTitle(0);
    data_2315->GetXaxis()->SetTitle("#theta (deg.)");
    data_2315->GetYaxis()->SetTitle("d#sigma/d#Omega (mb/sr)");
    //data_2315->GetHistogram()->SetMaximum(1000);
    data_2315->SetTitle("Ex = 2315.1 keV - Absolute cross section");
    //data_2315->GetYaxis()->SetRangeUser(0, 5);

    myFunc->Draw("same");

    //canvas->SetLogy(); // Set the y-axis to logarithmic scale
    
    TLegend* leg = new TLegend(0.71, 0.75, 0.9, 0.92);
    leg->AddEntry(myFunc, "l = 2", "l");
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04); // Adjust the text size to your preference
    leg->SetFillStyle(0); // Remove the box fill
    leg->Draw();


    // Create a new TLatex object
    TLatex* latex = new TLatex(0.722, 0.75, Form("#chi^{2}/NDF = %.2f", chi2_ndf));
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.038);
    latex->Draw();   
    
    // Add a second line underneath the first one
    TLatex* latex2 = new TLatex(0.72, 0.75 - 0.038, Form("S = %.2f", myFunc->GetParameter(0)));
    latex2->SetNDC();
    latex2->SetTextFont(42);
    latex2->SetTextSize(0.038);
    latex2->SetTextAlign(12); // Align left and baseline bottom
    latex2->Draw(); 

    canvas->SaveAs("ptolemy_2315_fit_Absolute_diffcross.png");
}


void fitting_3220() {
    read_DWBA("22Ne_2021_E_3220.out-clean");

    // Create a TF1 object using myfunction1
    TF1* myFunc = new TF1("myFunc", myfunction1, 10, 50, 1);

    FileReader state_3220_data("hdtv_E3220-clean.txt", "z_values.dat", "theta_values_E3220.dat");

    std::map<std::string, DataPoint> projectionMap = state_3220_data.getProjectionMap();

    int numPoints = projectionMap.size();
    TGraphErrors* data_3220 = new TGraphErrors(numPoints);


    int pointIndex = 0;
    double diffCross = 0.0; 
    double diffCrossErr = 0.0; 
    bool isFirstDataPoint = false;
    // Iterate over the projectionMap and add the desired data to the TGraphErrors
    for (const auto& entry : projectionMap) {
        const std::string& projection = entry.first;
        const DataPoint& dp = entry.second;

        if (isFirstDataPoint) {
            isFirstDataPoint = false;
            continue;  // Skip the first data point
        }

        diffCross = I_to_diffCrossSec(dp.vol, dp.theta_min, dp.theta_max, dp.theta_avg); 
        diffCrossErr = I_to_diffCrossSec(dp.volError, dp.theta_min, dp.theta_max, dp.theta_avg); 

        data_3220->SetPoint(pointIndex, dp.theta_avg, diffCross);
        data_3220->SetPointError(pointIndex, 0, diffCrossErr);

        std::cout << "theta_avg: " << dp.theta_avg << std::endl;
        std::cout << "Volume: " << dp.vol << std::endl;


        ++pointIndex;
    }

    TCanvas *canvas = new TCanvas("canvas", "3220 fit", 800, 600);

    data_3220->Fit(myFunc, "R");

    double chi2_ndf = myFunc->GetChisquare()/myFunc->GetNDF();

    std::cout << "Chi2: " << myFunc->GetChisquare() << " with " << myFunc->GetNDF() << " NDF" << std::endl;
    std::cout << "Chi2/NDF: " << myFunc->GetChisquare()/myFunc->GetNDF() << std::endl;

    data_3220->SetMarkerStyle(20);
    data_3220->SetTitle(0);
    data_3220->GetXaxis()->SetTitle("#theta (deg.)");
    data_3220->GetYaxis()->SetTitle("d#sigma/d#Omega (mb/sr)");
    // data_1016->GetXaxis()->SetLimits(15., 45.);
    // data_1016->GetHistogram()->SetMinimum(500);
    data_3220->SetTitle("Ex = 3220.69 keV - Absolute cross section");

    data_3220->Draw("AP");
    myFunc->Draw("same");

    //canvas->SetLogy(); // Set the y-axis to logarithmic scale
    
    TLegend* leg = new TLegend(0.71, 0.75, 0.9, 0.92);
    leg->AddEntry(myFunc, "l = 2", "l");
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04); // Adjust the text size to your preference
    leg->SetFillStyle(0); // Remove the box fill
    leg->Draw();

    // Add a second line underneath the first one
    TLatex* latex2 = new TLatex(0.72, 0.75 - 0.038, Form("S = %.2f", myFunc->GetParameter(0)));
    latex2->SetNDC();
    latex2->SetTextFont(42);
    latex2->SetTextSize(0.038);
    latex2->SetTextAlign(12); // Align left and baseline bottom
    latex2->Draw();     

    // Create a new TLatex object
    TLatex* latex = new TLatex(0.722, 0.75, Form("#chi^{2}/NDF = %.2f", chi2_ndf));
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.038);
    latex->Draw();   
   

    canvas->SaveAs("ptolemy_3220_fit_Absolutediffcross.png");
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Module 2 only 











































///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// old stuff: 

void plotVolVsXAvg(const std::vector<DataPoint>& dataPoints1, const std::unordered_map<std::string, DataPoint>& projectionMap) {
    // Create TGraph for plotting
    TGraph graph(dataPoints1.size());

    // Fill the graph with data points
    int pointIndex = 0;
    for (const auto& dp : dataPoints1) {
        if (projectionMap.find(dp.projection) != projectionMap.end()) {
            const auto& matchedDP = projectionMap.at(dp.projection);
            graph.SetPoint(pointIndex, matchedDP.x_avg, dp.vol);
            ++pointIndex;
        }
    }

    // Create a canvas and set the style
    TCanvas canvas("canvas", "I vs theta_avg", 800, 600);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    // Draw the graph
    graph.Draw("AP");
    graph.SetMarkerStyle(20);
    graph.SetMarkerSize(0.8);
    graph.SetLineColor(kBlue);
    graph.SetLineWidth(2);

    // Set the axis labels and title
    graph.GetXaxis()->SetTitle("#theta");
    graph.GetYaxis()->SetTitle("Intensity (arb.)");
    graph.SetTitle("I vs #theta");

    // Save the plot as a PDF file
    canvas.SaveAs("I_vs_theta.pdf");
}

TGraphErrors plot_I_Vs_theta(const std::vector<DataPoint>& dataPoints1, const std::unordered_map<std::string, DataPoint>& projectionMap) {
    // Create TGraph for plotting
    TGraphErrors graph(dataPoints1.size());

    // Fill the graph with data points
    int pointIndex = 0;
    for (const auto& dp : dataPoints1) {
        if (projectionMap.find(dp.projection) != projectionMap.end()) {
            const auto& matchedDP = projectionMap.at(dp.projection);
            graph.SetPoint(pointIndex, matchedDP.theta_avg, dp.vol);
            graph.SetPointError(pointIndex, 0, dp.volError);
            ++pointIndex;
        }
    }

    // Create a canvas and set the style
    TCanvas canvas("canvas", "I vs theta_avg", 800, 600);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    // Draw the graph
    graph.Draw("AP");
    graph.SetMarkerStyle(20);
    graph.SetMarkerSize(0.8);
    graph.SetLineColor(kBlue);
    graph.SetLineWidth(2);

    // Set the axis labels and title
    graph.GetXaxis()->SetTitle("#theta (deg.)");
    graph.GetYaxis()->SetTitle("Intensity (arb.)");
    graph.SetTitle("I vs #theta");

    // Save the plot as a PDF file
    canvas.SaveAs("I_vs_theta.pdf");

    return graph;
}



// void test() {
//     // Read the data and obtain the TGraph object
//     read_DWBA("22Ne_2021_E_0.out-clean");

//     // Create a TF1 object using myfunction1
//     TF1* myFunc = new TF1("myFunc", myfunction1, 10, 50);
    
//     FileReader ground_state_data("hdtv_E0-clean.txt", "z_values.dat", "theta_values.dat");

//     //cout << ground_state_data.getDataPoints3(); 

//     // myFunc->SetParameter(0, 1.0); // Set the function parameter

//     // Evaluate the TF1 object at a specific x value
//     // double result = myFunc->Eval(20.0);

//     // Perform other operations with the TF1 or TGraph

//     // Clean up
//     // delete myFunc;
//     // delete gDWBA;
// }





// void fit_DWBA_test() {
//     FileReader ground_state_data("hdtv_E0-clean.txt", "z_values.dat", "theta_values.dat");
//     std::string filename = "22Ne_2021_E_0.out-clean";
//     read_DWBA(filename);
//     TF1* f1 = new TF1("f1", myfunction1, 10, 50, 2);

//     const std::vector<DataPoint>& dataPoints1 = ground_state_data.getDataPoints1();
//     const std::unordered_map<std::string, DataPoint>& projectionMap = ground_state_data.getProjectionMap();

//     //plotVolVsXAvg(dataPoints1, projectionMap);
//     TGraphErrors graph = plot_I_Vs_theta(dataPoints1, projectionMap);
//     graph.Fit(f1);

// }

// void test() {
//     FileReader ground_state_data("hdtv_E0-clean.txt", "z_values.dat", "theta_values.dat");

//     const std::vector<DataPoint>& dataPoints1 = ground_state_data.getDataPoints1();
//     const std::unordered_map<std::string, DataPoint>& projectionMap = ground_state_data.getProjectionMap();

//     //plotVolVsXAvg(dataPoints1, projectionMap);
//     plot_I_Vs_theta(dataPoints1, projectionMap);
// }

