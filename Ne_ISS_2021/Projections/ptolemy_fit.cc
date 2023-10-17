///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// ROOT macro for fitting experimental cross sections to ptolemy data
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

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

/// takes in the x_values and hdtv_out files and group the data together
/// !!! important !!! change numbers in hdtv file so they're not in standard form
void fit_reader() {
    std::string hdtv_file = "hdtv_E0-clean.txt";
    std::string x_value_file = "x_values.dat";

    std::vector<DataPoint> dataPoints1 = hdtv_reader(hdtv_file);
    std::vector<DataPoint> dataPoints2 = x_val_reader(x_value_file);

    // Create a map to match data points based on projection names
    std::unordered_map<std::string, DataPoint> projectionMap;
    for (const auto& dp : dataPoints2) {
        projectionMap[dp.projection] = dp;
    }

    // Print matched data
    for (const auto& dp : dataPoints1) {
        if (projectionMap.find(dp.projection) != projectionMap.end()) {
            const auto& matchedDP = projectionMap[dp.projection];
            std::cout << "Projection: " << dp.projection << std::endl;
            std::cout << "pos: " << dp.pos << " +/- " << dp.posError << std::endl;
            std::cout << "vol: " << dp.vol << " +/- " << dp.volError << std::endl;
            std::cout << "width: " << dp.width << " +/- " << dp.widthError << std::endl;
            std::cout << "x_min: " << matchedDP.x_min << std::endl;
            std::cout << "x_max: " << matchedDP.x_max << std::endl;
            std::cout << "x_avg: " << matchedDP.x_avg << std::endl;
            std::cout << std::endl;
        }
    }

    
    // Create TGraphErrors for plotting
    TGraphErrors graph;
    int pointIndex = 0;

    // Fill the graph with data points and errors
    for (const auto& dp : dataPoints1) {
        if (projectionMap.find(dp.projection) != projectionMap.end()) {
            const auto& matchedDP = projectionMap[dp.projection];
            graph.SetPoint(pointIndex, matchedDP.x_avg, dp.vol);
            graph.SetPointError(pointIndex, 0, dp.volError);
            ++pointIndex;
        }
    }

    // Create a canvas and set the style
    TCanvas canvas("canvas", "Volume vs z position", 800, 600);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    // Draw the graph with error bars
    graph.Draw("AP");
    graph.SetMarkerStyle(20);
    graph.SetMarkerSize(0.8);
    graph.SetLineColor(kBlue);
    graph.SetLineWidth(2);

    // Set the axis labels and title
    graph.GetXaxis()->SetTitle("z (mm)");
    graph.GetYaxis()->SetTitle("Counts");
    graph.SetTitle("Counts vs z position");

    // Adjust the plot range
    Double_t xMin = graph.GetXaxis()->GetXmin();
    Double_t xMax = graph.GetXaxis()->GetXmax();
    Double_t yMin = graph.GetYaxis()->GetXmin();
    Double_t yMax = graph.GetYaxis()->GetXmax();
    graph.GetXaxis()->SetLimits(xMin - 0.05 * (xMax - xMin), xMax + 0.05 * (xMax - xMin));
    graph.GetYaxis()->SetRangeUser(yMin - 0.1 * (yMax - yMin), yMax + 0.1 * (yMax - yMin));

    // Draw the legend
    TLegend legend(0.15, 0.75, 0.35, 0.85);
    legend.SetBorderSize(0);
    legend.SetFillColorAlpha(0, 0);
    legend.SetTextSize(0.04);
    legend.AddEntry(&graph, "Ground state", "lep");
    legend.Draw();

    // Save the plot as a PDF file
    canvas.SaveAs("vol_vs_xavg.pdf");
}



