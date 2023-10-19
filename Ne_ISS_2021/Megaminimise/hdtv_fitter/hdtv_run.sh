# Run hdtv and save the output
hdtv -b batch_E0.txt > hdtv_E0.txt &

# Wait for a moment to ensure output is captured
sleep 5 

# Kill the hdtv interactive session
killall hdtv 

# Clean the hdtv output 
python3 hd_clean.py hdtv_E0.txt