# Run hdtv and save the output
hdtv -b batchtest.txt > hdtvout.txt &

# Wait for a moment to ensure output is captured
inotifywait -e modify hdtvout.txt -t 5 

# Kill the hdtv interactive session
killall hdtv 

