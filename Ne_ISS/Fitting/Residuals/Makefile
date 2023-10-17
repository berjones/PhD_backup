#Target is a space separated list of executables 
Target  = visablePeaksResiduals.exe 

#Objects are external objects needed to compile the target, blank for now
Objects = 

#these lines link the external root flags and libraries - try typing root-config --libs into your shell
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

#This gets ran when no Target is specified on input (e.g. we type 'make' into the shell) 
#it will compile everything specified in Target above
all:$(Target)

#our individual executable, $@ is the name of the target (in this case simple)
visablePeaksResiduals.exe: visablePeaksResiduals.cc
	g++ -std=c++0x -o $@ visablePeaksResiduals.cc$(Objects) $(ROOTFLAGS) $(ROOTLIBS)


#add more excutables in here
#...

#type make clean to remove all .exe files
clean:
	rm visablePeaksResiduals.exe
