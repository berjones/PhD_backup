#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "/home/ben/myPackages/ISS_home/ISSSort/include/Settings.hh"
#include "/home/ben/myPackages/ISS_home/ISSSort/include/Reaction.hh"

#include "TVector3.h"
#include "TGraph2D.h"

#include <iostream>

const double radius = 27.5; // mm

ISSSettings *myset;
ISSReaction *myreact;


struct DataPoint {
    std::string projection;
    double pos, vol, width;
    double posError, volError, widthError;
    double x_min, x_max, x_avg;
};

class FileReader {
public:
    FileReader(const std::string& hdtv_file, const std::string& x_value_file) {
        dataPoints1 = hdtv_reader(hdtv_file);
        dataPoints2 = x_val_reader(x_value_file);
        matchDataPoints();
    }

    std::vector<DataPoint> getDataPoints1() const {
        return dataPoints1;
    }

    std::vector<DataPoint> getDataPoints2() const {
        return dataPoints2;
    }

    const DataPoint& getDataPoint(const std::string& projection) const {
        return projectionMap.at(projection);
    }

    const std::unordered_map<std::string, DataPoint>& getProjectionMap() const {
        return projectionMap;
    }

private:
    std::vector<DataPoint> dataPoints1;
    std::vector<DataPoint> dataPoints2;
    std::unordered_map<std::string, DataPoint> projectionMap;

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

    void matchDataPoints() {
        for (const auto& dp : dataPoints2) {
            auto it = std::find_if(dataPoints1.begin(), dataPoints1.end(), [&](const DataPoint& d) {
                return d.projection == dp.projection;
            });

            if (it != dataPoints1.end()) {
                // Projection exists in both files
                projectionMap.emplace(dp.projection, dp);
            }
        }
    }
};


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
    TCanvas canvas("canvas", "Volume vs x_avg", 800, 600);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    // Draw the graph
    graph.Draw("AP");
    graph.SetMarkerStyle(20);
    graph.SetMarkerSize(0.8);
    graph.SetLineColor(kBlue);
    graph.SetLineWidth(2);

    // Set the axis labels and title
    graph.GetXaxis()->SetTitle("x_avg");
    graph.GetYaxis()->SetTitle("Volume");
    graph.SetTitle("Volume vs x_avg");

    // Save the plot as a PDF file
    canvas.SaveAs("vol_vs_xavg.pdf");
}

void test() {
    FileReader ground_state_data("hdtv_E0-clean.txt", "x_values.dat");

    const std::vector<DataPoint>& dataPoints1 = ground_state_data.getDataPoints1();
    const std::unordered_map<std::string, DataPoint>& projectionMap = ground_state_data.getProjectionMap();

    plotVolVsXAvg(dataPoints1, projectionMap);
}

