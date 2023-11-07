#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TSystem.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TProfile.h"
#include "ISSEvts.hh"

#include <iostream>
#include <fstream>

using namespace std;

// This script takes the time difference vs energy plots from root files and fits a function of the form E = exp( ax + b ) + cx + d. It then prints these values into an output file which can be added directly to a calibration file.

// // USE THIS TO FIND THE TIMEWALK ON THE P SIDES WITH RESPECT TO THE RECOIL DETECTOR (SILICON)
void p_recoil(){
    
    //Defining the number of bins to use etc. can put this outside of the functions and define them?
    //Changes the number of bins of the spectrum it will loop over (take noise out etc) -
    int numxbins = 1100, numybins = 800, nummods = 3, numrows = 4;
    int bindiffy = 100, bindiffx = 20; // These are parameters I was playing with. Theyre used when I would find the bin with the most number of counts. Everything else was ignored. (This is only for removing noise to make the TProfile work well)
     
    
    double fit_vals[13][nummods][numrows], means[numybins], widths[numybins];

    TFile *infile; //creates a TFile for the input file

    TH2F *tw_recoil_array[nummods][numrows]; //creates an array of TH2Fs for each of the spectra you want to fit. Ones for each module and row.
    
    
    // For loop just initialises each of the TH2Fs
    for ( int j = 1; j < nummods; j++ ){
        for (int k = 0; k < numrows; k++ ){
            tw_recoil_array[j][k] = new TH2F();
        }
    }

    infile = new TFile("22Ne_R8_to_R15_sumhists_20231014_014850_arraydist_135.root" ); //read in input file
    ofstream outfile("p-recoil_timewalk.cal" ); //open outfile

    TProfile *prof = new TProfile();
    
    // MAIN LOOP - loop over the modules and rows; take the recoil-psides timewalk hists; reduce noise; make a TProfile; Fit the profile.
    for ( int j = 1; j < 2; j++ ){
        for (int k = 0; k < 2; k++ ){
            
        //READ IN HISTOGRAM -----------------------------------------------
            tw_recoil_array[j][k] = (TH2F*)infile->Get( Form( "Timing/tw_recoil_array_hit0_mod_%i_row%i", j, k ) );
        // ----------------------------------------------------------------

            
            // This part can be commented out or deleted or improved -- I think I did this because there are few low energy counts in these bins (because it's at the end of the array) and this just made it easier to fit to the TProfile.
            // for mod0row0 and mod2row0 loop over x bins 0-300 and set all y bins = 0
            // if ( j == 0 && k == 0 ){
            //     for ( int x = 0; x < 300; x++ ){
            //         for ( int y = 0; y < numybins; y++ ){
            //             tw_recoil_array[j][k]->SetBinContent( tw_recoil_array[j][k]->GetBin( x, y ), 0);
            //         }
            //     }
            // }else if ( j == 0 && k == 0 ){
            //     for ( int x = 0; x < 300; x++ ){
            //         for ( int y = 0; y < numybins; y++ ){
            //             tw_recoil_array[j][k]->SetBinContent( tw_recoil_array[j][k]->GetBin( x, y ), 0);
            //         }
            //     }
            // }

            
        //REDUCE NOISE ----------------------------------------------------
            // The idea here is to loop over the x bins and find which y bin has the most number of counts and take this as the curve
            // Then ignore all events in a range around this maxmimum.
            // Initially tried to use a gaussian fit to each bin projection but there were'nt enough statistics to get a good fit. -- however i think this is the best idea
            // Instead I loop over all x bins, and for each x bin, find the y bin with the most counts.
            //      I then loop over the rest of the bins outside the range (defined above) of the curve and set all bins to 0 counts.
            
            
            
            //I did this because I found that it worked better to do this part of the plot by first looping over the y bins (because the curve is pretty much vertical here so this method works better.
            //Where the curve tails off at low energy it's better to loop over the x bins because the curve is horizontal(ish)
            for ( int y = 90; y < numybins; y++ ){
                
                //x projection
                TH1D *Xproj = tw_recoil_array[j][k]->ProjectionX( Form( "Xproj_%i_%i", j, k) , y, y);
                
                
                // Set all x bins not in the range of the curve to zero to fit easier without background stuffs
                int entries = Xproj->GetEntries();
                if ( entries == 0 ){ //skips y bins that have 0 entries
                    continue;
                }else if ( entries < 0 ){ //Used this when there are lots of counts (it's basically SetMinimum() before I knew that was a thing)
                   for ( int x = 0; x < numxbins; x++ ){
                       tw_recoil_array[j][k]->SetBinContent( tw_recoil_array[j][k]->GetBin( x, y ), 0);
                   }
                }else{ // These are all of the bins with counts in.
                
                    int maxbin = Xproj->GetMaximumBin(); //finds x bin with the max counts (for each y bin)
                    int maxbinval = Xproj->GetXaxis()->GetBinCenter(maxbin); //finds the centre of this bin
                    means[y] = tw_recoil_array[j][k]->GetXaxis()->FindBin(maxbinval); //
                    
                    //These 2 if statements I just made after seeing there was annoying bins with counts that I couldn't get rid of when I would run the script. (sorry but this script is fulllllll of this)
                    if ( y > 100 && maxbinval < -500 ){
                        for ( int x = 0; x < numxbins; x++ ){
                            tw_recoil_array[j][k]->SetBinContent( tw_recoil_array[j][k]->GetBin( x, y ), 0);
                        }
                    }else if ( y > 170 && maxbinval < -250 ){
                        for ( int x = 0; x < numxbins; x++ ){
                            tw_recoil_array[j][k]->SetBinContent( tw_recoil_array[j][k]->GetBin( x, y ), 0);
                        }
                    }else{
                    
                        for ( int x = 0; x < numxbins; x++ ){
                            
                            if ( maxbinval < -2000 || maxbinval > 30 ){
                                tw_recoil_array[j][k]->SetBinContent( tw_recoil_array[j][k]->GetBin( x, y ), 0);
                                continue;
                            }
                            if ( x < means[y] - 15 || x > means [y] + 10 ){
                                tw_recoil_array[j][k]->SetBinContent( tw_recoil_array[j][k]->GetBin( x, y ), 0);
                            }else{
                                continue;
                            }
                            
                        }
                        
                    }

                }

            }//ybins >= 90


            //loop over the x bins (time difference)
            for ( int x = 0; x < 400; x++ ){
                
                TH1D *Yproj = tw_recoil_array[j][k]->ProjectionY( Form( "Yproj_%i_%i", j, k) , x, x);
                int entries = Yproj->GetEntries();

                //loop over y bins and set all >80 == 0
                for ( int y = 80; y < numybins; y++ ){
                    tw_recoil_array[j][k]->SetBinContent( tw_recoil_array[j][k]->GetBin( x, y ), 0);
                }
                
                if ( entries == 0 ){
                    continue;
                }else{
                    
                    int maxbin = Yproj->GetMaximumBin();
                    int maxbincontent = Yproj->GetBinContent(maxbin);
                    int maxbinval = Yproj->GetXaxis()->GetBinCenter(maxbin);
                    
        
                    TH1D *h1 = Yproj;
                    
                    maxbin = h1->GetMaximumBin(); //finds y bin with the maximum content again
                    maxbincontent = h1->GetBinContent(maxbin); // to test if it is still 1
                    maxbinval = Yproj->GetXaxis()->GetBinCenter(maxbin); //finds the centre of this bin
                    means[x] = tw_recoil_array[j][k]->GetYaxis()->FindBin(maxbinval); //to remove the values aeound the peak that aren't in the range of the curve.
                    
                    for ( int y = 0; y < 80; y++ ){ //for each x bin, loop over y bins and set all out of the range of the curve to 0.
                        
                        if ( y < means[x] - 10 || y > means[x] + 30 ){
                            tw_recoil_array[j][k]->SetBinContent( tw_recoil_array[j][k]->GetBin( x, y ), 0);
                        }else{
                            continue;
                        }
                        
                    }
                        

                }//removing noise -- setting bins to 0

            }//xbins
            
        // ----------------------------------------------------------------

            
            
        //MAKE XPROFILE & FIT ---------------------------------------------

            new TCanvas();
            tw_recoil_array[j][k]->GetYaxis()->SetRangeUser( 500, 8000 );
            tw_recoil_array[j][k]->Draw("colz");
            prof = tw_recoil_array[j][k]->ProfileX();
            
            new TCanvas();
            prof->Draw("");

            int lowfit = -2000, highfit = 200;
            
            //FIT
            TF1 *f = new TF1("name", "pol1(0) + expo(2)", lowfit, highfit );
            prof->Fit( f, "R" );
            
            prof->Draw();

            for ( int parnum = 0; parnum < 4; parnum++ ){
                fit_vals[parnum][j][k] = f->GetParameter(parnum);
            }
        
        // ----------------------------------------------------------------

            
       } // k - rows
   } // j - modules
    

    
    //READ OUT PARAMATERS FOR CALIBRATION FILE
    int rownum;

    //Prints the fit parameters to output file in the format needed for the calibration file so you can just copy them in
    for ( int j = 0; j < nummods; j++ ){
        for ( int asicnum = 0; asicnum < 6; asicnum++ ){
            if ( asicnum == 1 || asicnum == 4 ){continue;}
            else if ( asicnum == 0 ){
                rownum = 0;
            }else if ( asicnum == 2 ){
                rownum = 1;
            }else if ( asicnum == 3 ){
                rownum = 2;
            }else if ( asicnum == 5){
                rownum = 3;
            }
            
            for ( int parnum = 0; parnum < 4; parnum++ ){
                outfile << "asic_" << j << "_" << asicnum << ".Walk" << parnum;
                outfile << ": " << fit_vals[parnum][j][rownum] << endl;
            }
            
        }
    }
    
    outfile.close();
    
    return;
    
}

// USE THIS TO FIND THE TIMEWALK ON THE N SIDES WITH RESPECT TO THE P SIDES
void nside_pside(){
        
    int numxbins = 600, numybins = 2000;
    int bindiffy = 100, bindiffx = 50;
    
    int numruns = 8, nummods = 3, numrows = 4, asicnum;
    
    double fit_vals[13][nummods][numrows], means[numybins];
    
    TH1F *pn_td[nummods][numrows];
    TH2F *pn_td_Ep[nummods][numrows], *pn_td_En[nummods][numrows];
    TH2F *pn_td_Ep0[nummods][numrows], *pn_td_En0[nummods][numrows];

    
    //read in file

    TFile *infile = new TFile( "nside/R8_to_R15_sumevents_for_tw_141023.root" ); //read in file
    ofstream outfile( "pn_timewalk.cal" ); //open outfile
    
    
    
    for ( int j = 0; j < nummods; j++ ){
        for (int k = 0; k < numrows; k++ ){
            
            bindiffy = 50;
            bindiffx = 10;
            
            //take histogram
            pn_td[j][k] = (TH1F*)infile->Get( Form( "array/module_%i/pn_td_uncorrected_mod%i_row%i", j, j , k ) );
            pn_td_Ep[j][k] = (TH2F*)infile->Get( Form( "array/module_%i/pn_td_Ep_uncorrected_mod%i_row%i", j, j , k ) );
            pn_td_En[j][k] = (TH2F*)infile->Get( Form( "array/module_%i/pn_td_En_uncorrected_mod%i_row%i", j, j , k ) );

            //make profile of it
            TProfile *prof = new TProfile();
                        
            //loop over y bins. take projection. max value of each projection is taken as the curve.
            //then loop over x bins in each y bin and set any that are out of a yet undecided range to 0 to get the curve only.
            //may have to mix this method and the previous method for low energies as at low energies the curve is relatively flat.
            for ( int biny = 0; biny < numybins; biny++ ){
//                for each ybin getting the x bin with the max value.
                TH1D *h2 = pn_td_En[j][k]->ProjectionX("projX", biny,biny);
                int maxxbin = h2->GetMaximumBin() + bindiffx;
                int minxbin = h2->GetMaximumBin() - bindiffx;


                //This is taking all the points above alpha energies where there are a lot of counts, and setting the pixels with fewer than 100 counts to 0 to try to remove noise.
                if ( biny > 280 ){
                    //loop over x bins but this time set all with entries < 100 to zero??? not sure this will give the exact effect i want, but it may help.
                    //no no no , instead: for all bins above 280, loop over the xbins in the x projection of the ybin, get the content of each bin (so going pixel by pixel) and if that has less than (not sure of the number yet) entries, and > 0, set it to zero. if zero, continue. if greater, this is the stuff we want (not noise) so continue.

                    for ( int binx = 0; binx < numxbins; binx++ ){
                        int bincontent = h2->GetBinContent( binx );

//                        if ( bincontent > 0 ){
//                            cout << bincontent << endl;
//                        }

                        if ( bincontent == 0 || bincontent > 10 ){
                            continue;
                        }else{
                            pn_td_En[j][k]->SetBinContent( pn_td_En[j][k]->GetBin( binx, biny ), 0);
                        }

                    }

                    //now loop over the xbins in each y bin and set all the bins ~20 above the max to 0 to remove the second tw curve.
                    for ( int binx = 0; binx < numxbins; binx++ ){
                        if ( binx > maxxbin ){
                            pn_td_En[j][k]->SetBinContent( pn_td_En[j][k]->GetBin( binx, biny ), 0);
                        }else if ( /*binx < minxbin ||*/ binx < minxbin ){
                            pn_td_En[j][k]->SetBinContent( pn_td_En[j][k]->GetBin( binx, biny ), 0);
                        }else{
                            continue;
                        }
                    }

                }else{

                    //now loop over the xbins in each y bin and set all the bins ~20 above the max to 0 to remove the second tw curve.
                    for ( int binx = 0; binx < numxbins; binx++ ){
                        if ( binx > maxxbin ){
                            pn_td_En[j][k]->SetBinContent( pn_td_En[j][k]->GetBin( binx, biny ), 0);
                        }else if ( /*binx < minxbin ||*/ binx < minxbin ){
                            pn_td_En[j][k]->SetBinContent( pn_td_En[j][k]->GetBin( binx, biny ), 0);
                        }else{
                            continue;
                        }
                    }
                }
            }
        

            prof = pn_td_En[j][k]->ProfileX();

            //draw it to see how it looks.

            new TCanvas();
            pn_td_En[j][k]->Draw("colz");
            new TCanvas();
            //01, 20
            int fithigh;
            
            if ( j == 0 ){
                fithigh = 50;
            }else{
                fithigh  = 50;
            }
            
            TF1 *f = new TF1("name", "pol1(0) + expo(2)", -3000, fithigh );
            prof->Fit(f, "QR");
            
            prof->GetYaxis()->SetRangeUser( 500, 8000 );
            prof->Draw("same");
            


            for ( int parnum = 0; parnum < 4; parnum++ ){
//                int par2 = parnum + 3;
                fit_vals[parnum][j][k] = f->GetParameter(parnum);
//                fit_vals[par2][j][k] = f2->GetParameter(parnum);
                
            }
//            }else{
//                continue;
//            }
        } // k - rows
    } // j - modules


    //row 0 = asic 0 and 1
    //row 1 = asic 2
    //row 2 = asic 3 and 4
    //row 3 = asic 5
    int rownum;
    for ( int j = 0; j < nummods; j++ ){
        int k;
        
//        if ( j == 1 ){
//            k = j - 1;
//        }else{
            k = j;
//        }
        
//        for (int k = 0; k < numrows; k++ ){
            for ( int asicnum = 0; asicnum < 6; asicnum++ ){

                if ( asicnum == 0 || asicnum == 1  ){
                    rownum = 0;
                }else if ( asicnum == 2 /*|| asicnum == 1*/ ){
                    rownum = 1;
                }else if ( asicnum == 3 || asicnum == 4  ){
                    rownum = 2;
                }else if ( asicnum == 5 /*|| asicnum == 4 */) {
                    rownum = 3;
                }

                
//                if ( j == 1 && asicnum == 1 ){
//                    continue;
//                }else
                if ( asicnum == 1 || asicnum == 4 ){
                    for ( int parnum = 0; parnum < 4; parnum++ ){
                        outfile << "asic_" << j << "_" << asicnum << ".Walk" << parnum;
                        outfile << ": " << fit_vals[parnum][k][rownum] << endl;
                    }
                }else{
                    continue;
                }

            }
    }


    outfile.close();
    
    return;

    
}

int main() {
    p_recoil();
    nside_pside();
    return 0;
}