#include <iostream>
#include <fstream>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

using namespace std;

int strip_threshold( TH1F *h ) {
	
	int thres;
	int maxbin = h->GetMaximumBin();
	double maxval = h->GetBinContent(maxbin);
	double curval;
	
	int binlimit = 20;
	double peakfraction = 0.005;

	for( int i = maxbin+1; i < maxbin + binlimit; ++i ){
		
		curval = h->GetBinContent(i);
		thres = h->GetBinCenter(i);
		if( curval < maxval*peakfraction ) break;
					
	}
		
	return thres;

}

int run_thresholds( string filename) {
	
	TFile *infile = new TFile( filename.data() );
	
	string hname;
	TH2F* h2;
	TH1F* h1;

	ofstream outfile( "thresholds_R1_all.dat" );
	
	int thres;

	// Loop over all modules
	for( int mod = 0; mod < 3; ++mod ) {
		
		// Loop over all asics
		for( int asic = 0; asic < 6; ++asic ) {
			
			// Loop over all channels
			for( int i = 0; i < 128; ++i ){
			
				if( asic == 1 || asic == 4 ) {
					
					if( i < 11 ) continue;
					if( i > 21 && i < 28 ) continue;
					if( i > 38 && i < 89 ) continue;
					if( i > 99 && i < 106 ) continue;
					if( i > 116 ) continue;

				}
				
				// Get histogram from file
				hname  = "asic_hists/module_" + to_string(mod);
				hname += "/asic_" + to_string(mod) + "_";
				hname += to_string(asic);
				h2 = (TH2F*)infile->Get( hname.data() );

				h1 = (TH1F*)h2->ProjectionY( "tmp", i+1, i+1 );

				thres = strip_threshold( h1 );

				outfile << "asic_" << mod << "_" << asic << "_" << i;
				outfile << ".Threshold:\t" << 250 << endl;

			}

		}
		
	}
	
	outfile.close();
	infile->Close();

	return 0;
	
}

// int main()
// {
// 	//run_thresholds("../../calibrations/Autocals/R1_alpha_autocal_CB/autocal.root");
// 	run_thresholds("../../calibrations_PHD_corrected/R1_alpha_autocal_CB/autocal.root");
// 	return 0;
// }

