#Use this makefile in future when dealing with ISS as its the one liam helped me write

#Target is a space separated list of executables 
Target  = timewalk

#Objects are external objects needed to compile the target, blank for now
Objects = 

#these lines link the external root flags and libraries - try typing root-config --libs into your shell
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)
CC = $(shell root-config --cxx)

#This gets ran when no Target is specified on input (e.g. we type 'make' into the shell) 
#it will compile everything specified in Target above
all:$(Target)

#our individual executable, $@ is the name of the target (in this case simple)
timewalk: timewalk.cc
	$(CC) -o $@ timewalk.cc $(Objects) $(ROOTFLAGS) $(ROOTLIBS) -I//home/ben/myPackages/ISS_home/ISSSort/include -L/home/ben/myPackages/ISS_home/ISSSort/lib -liss_sort

#add more excutables in here
#...

#type make clean to remove all .exe files
clean:
	rm timewalk