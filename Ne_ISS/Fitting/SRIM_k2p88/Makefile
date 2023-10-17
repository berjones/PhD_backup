#Target is a space separated list of executables 
Target  = peak_fitter_visable.exe peak_fitter_knownpeaks.exe

#Objects are external objects needed to compile the target, blank for now
Objects = 

#these lines link the external root flags and libraries - try typing root-config --libs into your shell
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

#This gets ran when no Target is specified on input (e.g. we type 'make' into the shell) 
#it will compile everything specified in Target above
all:$(Target)

#our individual executable, $@ is the name of the target (in this case simple)
peak_fitter_visable.exe: peak_fitter_visable.cc
	g++ -std=c++0x -o $@ peak_fitter_visable.cc$(Objects) $(ROOTFLAGS) $(ROOTLIBS)

peak_fitter_knownpeaks.exe: peak_fitter_knownpeaks.cc
	g++ -std=c++0x -o $@ peak_fitter_knownpeaks.cc$(Objects) $(ROOTFLAGS) $(ROOTLIBS)

#add more excutables in here
#...

#type make clean to remove all .exe files
clean:
	rm peak_fitter_visable.exe
	rm peak_fitter_knownpeaks.exe