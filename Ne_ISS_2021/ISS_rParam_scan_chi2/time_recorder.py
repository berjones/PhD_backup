import datetime

# Record start time
start_time = datetime.datetime.now()
start_time_str = start_time.strftime("%Y-%m-%d %H:%M:%S")

with open('iteration_times.txt', 'a') as time_file:
    time_file.write(f"Iteration start time: {start_time_str}\n")