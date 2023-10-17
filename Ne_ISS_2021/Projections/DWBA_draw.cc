#include <iostream>
#include <fstream>
#include <vector>
#include "TGraph.h"
#include "TF1.h"
#include "TCanvas.h"

#include <iostream>
#include <fstream>
#include <vector>
#include "TGraph.h"
#include "TF1.h"
#include "TCanvas.h"

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

double myfunction1(Double_t* x, Double_t* p) {
    return p[0] * gDWBA->Eval(x[0]);
}

void test() {
    // Read the data and obtain the TGraph object
    read_DWBA("22Ne_2021_E_0.out-clean");

    // Create a TF1 object using myfunction1
    TF1* myFunc = new TF1("myFunc", myfunction1, 10, 50);
    

    // myFunc->SetParameter(0, 1.0); // Set the function parameter

    // Evaluate the TF1 object at a specific x value
    // double result = myFunc->Eval(20.0);

    // Perform other operations with the TF1 or TGraph

    // Clean up
    // delete myFunc;
    // delete gDWBA;
}
