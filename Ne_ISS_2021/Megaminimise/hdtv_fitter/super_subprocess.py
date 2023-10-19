import subprocess 

subprocess.run('./hdtv_run.sh', shell = True)

subprocess.run('root -l -b -q run_Ex_vs_z.C', shell = True)
