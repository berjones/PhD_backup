#include <fstream>
#include <sstream>
#include <vector>

#include "/home/ben/myPackages/ISS_home/ISSSort/include/Settings.hh"
#include "/home/ben/myPackages/ISS_home/ISSSort/include/Reaction.hh"

#include "TVector3.h"
#include "TGraph2D.h"

#include <iostream>

const double radius = 27.5; // mm

ISSSettings *myset;
ISSReaction *myreact;

void z_to_angle_conveter(const std::string& reaction_file) {
    // Make a reaction instance
    myset = new ISSSettings("dummy");
    myreact = new ISSReaction(reaction_file, myset, false);
}

double get_theta_cm(double z_det, double E_x) {
    // Make a vector for the interaction position
    TVector3 pos(radius, 0.0, z_det);

    // Set excitation energy
    myreact->GetRecoil()->SetEx(E_x);

    // Get theta_cm
    myreact->SimulateReaction(pos);
    double theta_cm = myreact->GetThetaCM();

    // Print the result
    // std::cout << "z_det = " << z_det << ", E_x = " << E_x;
    // std::cout << ", θ_cm = " << theta_cm << std::endl;

    return theta_cm* TMath::RadToDeg();
}


struct DataPoint {
    std::string projection;
    double pos, vol, width;
    double posError, volError, widthError;
    double x_min, x_max, x_avg;
};

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


void z_to_theta_run() {
    z_to_angle_conveter("reaction_22Ne_R8_R15_arraydist_corr_129p1.dat");

    std::vector<DataPoint> z_values = x_val_reader("z_values.dat");

    std::ofstream theta_outfile("theta_values_E3220.dat");
    theta_outfile << "projection, theta_min, theta_max, theta_avg" << endl; 

    double E_x = 3220.7; // Set the desired value of E_x here
    // Pass each x_min value to the function
    for (const DataPoint& dp : z_values) {
        theta_outfile << dp.projection << ", " << get_theta_cm(dp.x_min, E_x) << ", " << get_theta_cm(dp.x_max, E_x) << ", " << get_theta_cm(dp.x_avg, E_x)  << endl;
    }

    // Clean up resources
    delete myreact;
    delete myset;
}