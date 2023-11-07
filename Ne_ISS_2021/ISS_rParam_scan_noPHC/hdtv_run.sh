# Ex = 0 fit 
#-----------------------------------------------------------------
# Run hdtv and save the output
hdtv -b batchfiles/batch_E0.txt > hdtv_fits/hdtv_E0.txt &

# Wait for a moment to ensure output is captured
sleep 5 

# Kill the hdtv interactive session
killall hdtv 

# Clean the hdtv output 
python3 hd_clean.py hdtv_fits/hdtv_E0.txt
#-----------------------------------------------------------------

sleep 1 # small pause between the different fits

# Ex = 1016.9 fit 
#-----------------------------------------------------------------
# Run hdtv and save the output
hdtv -b batchfiles/batch_E1016.txt > hdtv_fits/hdtv_E1016.txt &

# Wait for a moment to ensure output is captured
sleep 5 

# Kill the hdtv interactive session
killall hdtv 

# Clean the hdtv output 
python3 hd_clean.py hdtv_fits/hdtv_E1016.txt
#-----------------------------------------------------------------

sleep 1 # small pause between the different fits

# Ex = 2315.1 fit 
#-----------------------------------------------------------------
# Run hdtv and save the output
hdtv -b batchfiles/batch_E2315.txt > hdtv_fits/hdtv_E2315.txt &

# Wait for a moment to ensure output is captured
sleep 5 

# Kill the hdtv interactive session
killall hdtv 

# Clean the hdtv output 
python3 hd_clean.py hdtv_fits/hdtv_E2315.txt
#-----------------------------------------------------------------