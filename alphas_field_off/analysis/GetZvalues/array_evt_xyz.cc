#include <fstream>
#include <sstream>
#include <vector>

#include "/home/ben/myPackages/ISS_home/ISSSort/include/Settings.hh"
#include "/home/ben/myPackages/ISS_home/ISSSort/include/Reaction.hh"
#include "/home/ben/myPackages/ISS_home/ISSSort/include/ISSEvts.hh"

#include "TVector3.h"
#include "TGraph2D.h"

#include <iostream>

ISSArrayEvt *myarrayevt;

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
    std::cout << "(x,y,z) : (" << n[0] << "," << n[1] << "," << n[2] << ")" << std::endl;

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
    alpha = alpha*(180/TMath::Pi());

    theta = 90 - alpha; 

    return theta;
}


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

