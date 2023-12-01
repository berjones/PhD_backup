#include <fstream>
#include <sstream>
#include <vector>

#include "/home/ben/myPackages/ISS_home/ISSSort/include/Settings.hh"
#include "/home/ben/myPackages/ISS_home/ISSSort/include/Reaction.hh"
#include "/home/ben/myPackages/ISS_home/ISSSort/include/ISSEvts.hh"

#include "TVector3.h"
#include "TGraph2D.h"
#include <TMath.h>

#include <iostream>

ISSArrayEvt *myarrayevt;


// The geometry calculations used to determine theta
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Gets the XYZ coordinate of a unique pixel on the array
TVector3 GetArrayEvtXYZ(const double array_distance, const int pid, const int nid, int module = 1, int row = 3) {
    // Make a reaction instance
    myarrayevt = new ISSArrayEvt();

    myarrayevt->SetEvent( 5000, // P energy
					   5000, // N energy 
					   pid, // pid 
					   nid, // nid
					   1, // ptime
					   1, // ntime 
					   0, // p-hit bit
					   0, // n-hit bit 
					   module, // module
					   row ); // row
    

    TVector3 myXYZ = myarrayevt->GetPosition();

	myXYZ[0] = -myXYZ[0] ;
	myXYZ[1] = -myXYZ[1] ;
	myXYZ[2] = myXYZ[2] ;
	myXYZ[2] += array_distance;

	return myXYZ;
}

// Gets all XYZ(pid) values for a particular nid, mod and row 
void Get_XYZ_for_nid(double array_distance, int nid, int module = 1, int row = 3){
	
	int pid_max = 127 ;

	std::string title = Form("XYZfor_mod%d_row%d_nid%d_arraydist%f", module, row, nid, array_distance);

	// Create a vector to store TVector3 objects
    std::vector<TVector3> coordinates;

	std::ofstream XYZcoordinateFile(title);
    for (int pid = 0; pid <= pid_max; ++pid) {
        TVector3 currentXYZ = GetArrayEvtXYZ(array_distance, pid, nid, module, row);
        coordinates.push_back(currentXYZ);

        // Optionally, you can write the coordinates to the file directly
        XYZcoordinateFile << currentXYZ[0] << " " << currentXYZ[1] << " " << currentXYZ[2] << "\n";
    }

	// Make this function return a vector of TVectors.

}

// Gets the vector normal to the plane of the strip
// Need to give three points on one strip to define this
TVector3 Get_normal_vector(int module = 1, char side = 'A') {
    // Define our three points on the strip (plane) of interest
    TVector3 P, Q, R;

    // Choose points based on the side
    switch (side) {
        case 'A':
            P = GetArrayEvtXYZ(100, 127, 10, module, 3);
            Q = GetArrayEvtXYZ(100, 0, 10, module, 3);
            R = GetArrayEvtXYZ(100, 127, 0, module, 3);
            break;

        case 'B':
            P = GetArrayEvtXYZ(100, 127, 21, module, 3);
            Q = GetArrayEvtXYZ(100, 0, 21, module, 3);
            R = GetArrayEvtXYZ(100, 127, 11, module, 3);
            break;

        default:
            // Handle invalid side
            std::cerr << "Invalid side. Please use 'A' or 'B'." << std::endl;
            // Return a default TVector3 or throw an exception as needed
            return TVector3();
    }

    // Define our two vectors parallel to the plane
    TVector3 PQ = Q - P;
    TVector3 PR = R - P;

    // Get our normal vector
    TVector3 n = PQ.Cross(PR);

    // print normal vector
    //std::cout << "(x,y,z) : (" << n[0] << "," << n[1] << "," << n[2] << ")" << std::endl;

    return n;
}

// Get the vector from the alpha source to the pixel of interest
// Need to define the coordinates of the alpha source and the pixel of interest
TVector3 Get_alphaPixel_vector(const double array_distance, const int pid, const int nid, int module = 1, int row = 3){
    
    // The position of the alpha source in our coordinate system
    TVector3 alphaXYZ(0, 131.47, 0);

    // Position of the pixel of interest
    TVector3 PixelXYZ = GetArrayEvtXYZ(array_distance, pid, nid, module, row);

    TVector3 alphaPixelXYZ = PixelXYZ - alphaXYZ; 

    //std::cout << "(x,y,z) : (" << alphaPixelXYZ[0] << "," << alphaPixelXYZ[1] << "," << alphaPixelXYZ[2] << ")" << std::endl;

    return alphaPixelXYZ;
}

// Calculates the angle between the line and the plane
Double_t Get_Theta(const double array_distance, const int pid, const int nid, int module = 1, int row = 3, char side = 'A'){
    TVector3 n, alphaPixel;
    Double_t cos_alpha, alpha, theta;

    // Calculate the normal vector
    n = Get_normal_vector(module, side);

    // Calculate the alpha-pixel vector
    alphaPixel = Get_alphaPixel_vector(array_distance, pid, nid, module, row);

    // Calculate the accute angle (alpha) between the normal and the alphaPixel vector
    cos_alpha = alphaPixel * n;
    cos_alpha /= alphaPixel.Mag() * n.Mag();

    alpha = TMath::ACos(cos_alpha);
    //alpha = alpha*(180/TMath::Pi());

    //theta = 90 - alpha; 
    theta = TMath::Pi()/2 - alpha;

    return alpha;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// Do the automatic fit of the alpha peaks
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


Double_t CrystalBall4(Double_t *x, Double_t *par) {
    // 4 peak crystall ball  
    Double_t result = 0.0;

    for (int i = 0; i < 4; i++) {
        result += par[5 * i + 4] * ROOT::Math::crystalball_function(x[0], par[5 * i], par[5 * i + 1], par[5 * i + 2], par[5 * i + 3]);
    }

    return result;
}

Double_t Gauss4(Double_t *x, Double_t *par) {
    // 4 peak Gaussian
    Double_t result = 0.0;

    for (int i = 0; i < 4; i++) {
        result += par[3 * i + 2] * TMath::Gaus(x[0], par[3*i], par[3*i + 1]);
    }

    return result;
}

std::vector<std::vector<Double_t>> FitAlphaPeaks(const std::string& rootFileName, const std::string& En_pid_DistanceN) {
    // Root in batch mode 
    gROOT->SetBatch(kTRUE);
    gErrorIgnoreLevel = kError;

    // Open the ROOT file
    TFile *file = new TFile(rootFileName.c_str());

    // Proj Output file
    //std::string outputFileName = "proj_" + rootFileName;
    //TFile *outputFile = new TFile(outputFileName.c_str(), "RECREATE");

    // Get the 2D histogram
    TH2F *hist = (TH2F*)file->Get(En_pid_DistanceN.c_str());

    // Create a directory for saving plots
    std::string plotDirectory = En_pid_DistanceN + "_plots";
    system(("mkdir -p " + plotDirectory).c_str());

    // Stores the peak positions for each projection 
    std::vector<std::vector<Double_t>> allPeaksTSpec;
    std::vector<std::vector<Double_t>> allPeaksFit;

    // Loop over each pid channel
    for (int pid = 0; pid < 128; ++pid) {
        // Create a projection for each pid channel
        TH1D *projection = hist->ProjectionY(Form("Projection_pid%d", pid), pid + 1, pid + 1);

        // Create a canvas to draw the projection
        TCanvas *canvas = new TCanvas("canvas", "canvas");

        // Find peaks using TSpectrum
        TSpectrum *s = new TSpectrum(4);
        Int_t nfound = s->Search(projection, 2.5, "", 0.1);

        //projection->Write(Form("Projection_pid%d", pid));

        std::vector<Double_t> xpValues;

        Double_t *xpeaks = s->GetPositionX();
        for (int p = 0; p < nfound; ++p) {
            Double_t xp = xpeaks[p];
            xpValues.push_back(xp);
        }

        // Sort the centroid values to be in order
        std::sort(xpValues.begin(), xpValues.end());

        allPeaksTSpec.push_back(xpValues);

        // Fit our 4 crystal ball functions
        // Constrain the fit parameters
        auto CBFit4 = new TF1("CBFit4", CrystalBall4, 3500, 6500, 20);
        for (Int_t p = 0; p < 4; p++) {
            //Set initial fit parameters
            CBFit4->SetParameter(5 * p, 1); // A
            CBFit4->SetParameter(5 * p + 1, 1); // N
            CBFit4->SetParameter(5 * p + 2, 15); // S
            CBFit4->SetParameter(5 * p + 3, xpValues[p]); // M
            CBFit4->SetParameter(5 * p + 1, 100);// C

            // Set parameter limits
            CBFit4->SetParLimits(5 * p, 0, 100);
            CBFit4->SetParLimits(5 * p + 1, 0, 100);
            CBFit4->SetParLimits(5 * p + 2, 10, 30);
            CBFit4->SetParLimits(5 * p + 3, xpValues[p] - 20, xpValues[p] + 20);
        }

        projection->Fit("CBFit4", "Q");

        // Store the centroid values and errors into a vector
        std::vector<Double_t> CentroidValuesCB;
        for (Int_t p = 0; p < 4; p++) {
            Double_t CentroidValCB = CBFit4->GetParameter(5 * p + 3); // M parameter is at index 5 * p + 3
            Double_t CentroidValErrCB = CBFit4->GetParError(5 * p + 3);
            CentroidValuesCB.push_back(CentroidValCB);
            CentroidValuesCB.push_back(CentroidValErrCB);
        }

        auto GaussFit4 = new TF1("GaussFit4", Gauss4, 3500, 6500, 12);
        for (Int_t p = 0; p < 4; p++) {
            // Set initial fit parameters
            GaussFit4->SetParameter(3 * p, xpValues[p]); // M
            GaussFit4->SetParameter(3 * p + 1, 20); // S
            GaussFit4->SetParameter(3 * p + 2, 100); // N

            // Set parameter limits
            GaussFit4->SetParLimits(3 * p, xpValues[p] - 20, xpValues[p] + 20);
            GaussFit4->SetParLimits(3 * p + 1, 10,30.);
        }

        projection->Fit("GaussFit4", "Q");

        // Store the centroid values and errors into a vector for Gaussian fit
        std::vector<Double_t> GaussCentroidValues;
        for (Int_t p = 0; p < 4; p++) {
            Double_t GaussCentroidVal = GaussFit4->GetParameter(3 * p ); // M parameter is at index 3 * p + 1
            Double_t GaussCentroidValErr = GaussFit4->GetParError(3 * p );
            GaussCentroidValues.push_back(GaussCentroidVal);
            GaussCentroidValues.push_back(GaussCentroidValErr);
        }

        allPeaksFit.push_back(GaussCentroidValues);

        // Draw the projection
        projection->Draw();

        // Plot the Crystal Ball fit in red
        CBFit4->SetLineColor(kRed);
        CBFit4->Draw("SAME");

        // Plot the Gaussian fit in blue
        GaussFit4->SetLineColor(kBlue);
        GaussFit4->Draw("SAME");

        TLegend* leg = new TLegend(0.2, 0.74, 0.5, 0.88);
        leg->AddEntry(CBFit4, "CrystalBall");
        leg->AddEntry(GaussFit4, "Gaussian");
        leg->AddEntry(projection, "data");
        leg->SetBorderSize(0);
        leg->SetTextSize(0.04); // Adjust the text size to your preference
        leg->SetFillStyle(0); // Remove the box fill
        leg->Draw();

        // Save the canvas as a PNG file
        const char* fileName = Form("%s/projection_pid%d.png", plotDirectory.c_str(), pid);
        canvas->SaveAs(fileName);

        // Clean up
        delete projection;
        delete canvas;
        delete s;
    }

    // Close the output ROOT file
    //outputFile->Close();
    //delete outputFile;

    // Clean up
    delete hist;
    file->Close();
    delete file;

    return allPeaksFit;
}



void DeadLayerPlot(const std::string& rootFileName, const std::string& En_pid_Distance1, const std::string& En_pid_Distance2) {
    // Read in the peak positions
    std::vector<std::vector<Double_t>> alphaPeaks_d1 = FitAlphaPeaks(rootFileName, En_pid_Distance1);
    std::vector<std::vector<Double_t>> alphaPeaks_d2 = FitAlphaPeaks(rootFileName, En_pid_Distance2);

    // Loop over each peak
    for (size_t peakIndex = 0; peakIndex < 4; ++peakIndex) {
        // Create a canvas for each peak
        TCanvas *canvas = new TCanvas("canvas", "Dead Layer Plots", 800, 600);

        // Create a graph for each peak
        TGraphErrors *EvsInvCosTheta_1 = new TGraphErrors();
        TGraphErrors *EvsInvCosTheta_2 = new TGraphErrors();

        TMultiGraph *mg = new TMultiGraph();

        int plotIndex1 = 0;
        // Loop over each PID value
        for (int pid = 0; pid < 128; ++pid) {
            // Calculate 1/cos(theta) using Get_Theta function
            double theta = Get_Theta(80.02, pid, 5);
            double invCosTheta = 1.0 / TMath::Cos(theta);

            // Check if the error is within a reasonable range
            if (std::abs(alphaPeaks_d1[pid][2 * peakIndex + 1]) >= 10) {
                // Skip points with large errors
                continue;
            }

            // Check if mean is within a reasonable range
            if (std::abs(alphaPeaks_d1[pid][2 * peakIndex]) >= 6500) {
                // Skip points with large errors
                continue;
            }

            // Add a point to the graph
            EvsInvCosTheta_1->SetPoint(plotIndex1, invCosTheta, alphaPeaks_d1[pid][2 * peakIndex]);
            EvsInvCosTheta_1->SetPointError(plotIndex1, 0, alphaPeaks_d1[pid][2 * peakIndex + 1]);
            plotIndex1++;
        }

        int plotIndex2 = 0;
        for (int pid = 0; pid < 128; ++pid) {
            // Calculate 1/cos(theta) using Get_Theta function
            double theta = Get_Theta(205.02, pid, 5);
            double invCosTheta = 1.0 / TMath::Cos(theta);

            // Check if the error is within a reasonable range
            if (std::abs(alphaPeaks_d2[pid][2 * peakIndex + 1]) >= 10) {
                // Skip points with large errors
                continue;
            }

            // Check if mean is within a reasonable range
            if (std::abs(alphaPeaks_d2[pid][2 * peakIndex]) >= 6500) {
                // Skip points with large errors
                continue;
            }

            // Add a point to the graph
            EvsInvCosTheta_2->SetPoint(plotIndex2, invCosTheta, alphaPeaks_d2[pid][2 * peakIndex]);
            EvsInvCosTheta_2->SetPointError(plotIndex2, 0, alphaPeaks_d2[pid][2 * peakIndex + 1]);
            plotIndex2++;
        }


        // Create a new plot for each peak
        EvsInvCosTheta_1->SetMarkerStyle(4);
        EvsInvCosTheta_1->SetMarkerColorAlpha(kBlue, 1);
        EvsInvCosTheta_2->SetMarkerStyle(4);
        EvsInvCosTheta_2->SetMarkerColorAlpha(kGreen, 1);
        mg->Add(EvsInvCosTheta_1);
        mg->Add(EvsInvCosTheta_2);       

        mg->SetTitle(Form("Alpha Peak %zu vs 1/cos(theta);1/cos(theta);Energy", peakIndex + 1));
        mg->Draw("AP");

        // Fit the first distance
        TF1 *Fit_d1 = new TF1("Fit_d1", "pol1");
        Fit_d1->SetParameter(0, 6000);  // p0
        Fit_d1->SetParameter(1, -80); // p1
        Fit_d1->SetRange(1.1, 1.64);
        Fit_d1->SetLineColorAlpha(kBlue, 1);
        EvsInvCosTheta_1->Fit("Fit_d1", "R");
        
        // Fit the second distance
        TF1 *Fit_d2 = new TF1("Fit_d2", "pol1");
        Fit_d2->SetParameter(0, 6000);  // p0
        Fit_d2->SetParameter(1, -80); // p1
        Fit_d2->SetRange(1.61, 2.28);
        Fit_d2->SetLineColorAlpha(kGreen, 1);
        EvsInvCosTheta_2->Fit("Fit_d2", "R");

        // Fit the combined graph
        TF1 *Fit_full_d = new TF1("Fit_full_d", "pol1");
        Fit_full_d->SetParameter(0, 6000);  // p0
        Fit_full_d->SetParameter(1, -80); // p1
        Fit_full_d->SetRange(1.1, 2.28);
        Fit_full_d->SetLineColorAlpha(kRed, 1);
        mg->Fit("Fit_full_d", "R");

        TLegend* leg = new TLegend(0.5, 0.74, 0.7, 0.88);
        leg->AddEntry(EvsInvCosTheta_1, "Array Distance = 80.02 mm", "pe");
        leg->AddEntry(EvsInvCosTheta_2, "Array Distance = 205.02 mm", "pe");
        leg->AddEntry(Fit_d1, "Array Distance = 80.02 mm, fit ", "l");
        leg->AddEntry(Fit_d2, "Array Distance = 205.02 mm, fit ", "l");
        leg->AddEntry(Fit_full_d, "Combined fit", "l");
        leg->SetBorderSize(0);
        leg->SetTextSize(0.03); // Adjust the text size to your preference
        leg->SetFillStyle(0); // Remove the box fill
        leg->Draw();

        // Save the plot for each peak
        canvas->SaveAs(Form("Peak_%zu_vs_InvCosTheta.png", peakIndex + 1));

        // Delete the canvas and graph to avoid memory leaks
        delete canvas;
        delete EvsInvCosTheta_1;
    }
}




// Some extra functions of use 
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Get_XYZ_for_pid(double array_distance, int pid, int module = 1, int row = 3){
	
	int nid_max = 22 ;

	std::string title = Form("XYZfor_mod%d_row%d_pid%d_arraydist%f", module, row, pid, array_distance);

	// Create a vector to store TVector3 objects
    std::vector<TVector3> coordinates;

	std::ofstream XYZcoordinateFile(title);
    for (int nid = 0; nid < nid_max; ++nid) {
        TVector3 currentXYZ = GetArrayEvtXYZ(array_distance, pid, nid, module, row);
        coordinates.push_back(currentXYZ);

        // Optionally, you can write the coordinates to the file directly
        XYZcoordinateFile << currentXYZ[0] << " " << currentXYZ[1] << " " << currentXYZ[2] << "\n";
    }

	 // Create a TGraph for the X and Y coordinates
    TGraph *graph = new TGraph(nid_max);
    graph->SetTitle(title.c_str());
    graph->GetXaxis()->SetTitle("Y Coordinate");
    graph->GetYaxis()->SetTitle("X Coordinate");
	graph->SetMarkerColor(kBlue);
	graph->SetMarkerStyle(210);

    // Fill the TGraph with the X and Y coordinates
    for (int nid = 0; nid < nid_max; ++nid) {
        graph->SetPoint(nid, coordinates[nid].Y(), coordinates[nid].X());
    }
    // Optionally, you can also draw the TGraph on a canvas
    TCanvas *canvas = new TCanvas("canvas", "X vs Y for nid =", 800, 600);
    graph->Draw("AP");  // "AP" option for drawing points

	TLatex latex;
    for (int nid = 0; nid < nid_max; ++nid) {
        latex.SetTextSize(0.02);
        latex.DrawLatex(coordinates[nid].Y(), coordinates[nid].X(), Form("%d", nid));
    }

    // Save the canvas as an image file
    canvas->SaveAs("nidXYcoordinates.png");
}


void print_XYZ_for_nid(double array_distance, int pid, int nid){
	TVector3 XYZ = GetArrayEvtXYZ(array_distance, pid, nid);

	std::cout << "x = " <<  XYZ[0] << std::endl;
	std::cout << "y = " <<  XYZ[1] << std::endl;
	std::cout << "z = " <<  XYZ[2] << std::endl;

}

