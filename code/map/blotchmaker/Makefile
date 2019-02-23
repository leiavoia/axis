# makefile for ordered linked list module +


# use "gcc" to compile source files.
CC = g++

# the linker is also "gcc". It might be something else with other compilers.
LD = g++

# Compiler flags go here.
CFLAGS = -g -Wall

# Linker flags go here. Currently there aren't any, but if we'll switch to
# code optimization, we might add "-s" here to strip debug info and symbols.
LDFLAGS =

# use this command to erase files.
RM = /bin/rm -f

# program executable file name.
PROG = astartest

# list of generated object files.
OBJS = astartest.o map.o extra_math.o blotchmaker.o mapsearch.o
#AxisMapMenu.o AxisMiniMapWidget.o




# top-level rule to create the whole program.
all: $(PROG)


$(PROG):  $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG)

# for main
astartest.o: astartest.cc
	$(CC) $(CFLAGS) -c astartest.cc

# for main
#main.o: main.cc
#	$(CC) $(CFLAGS) -c main.cc



#for map
map.o: map.cc map.h
	$(CC) $(CFLAGS) -c map.cc

#for blotchmaker
blotchmaker.o: blotchmaker.cc blotchmaker.h
	$(CC) $(CFLAGS) -c blotchmaker.cc

#for extra_math
extra_math.o: extra_math.cc extra_math.h
	$(CC) $(CFLAGS) -c extra_math.cc

#for mapsearch
mapsearch.o: mapsearch.cc mapsearch.h
	$(CC) $(CFLAGS) -c mapsearch.cc



#for mapsearch
#AxisMapMenu.o: AxisMapMenu.cpp AxisMapMenu.h
#	$(CC) $(CFLAGS) -c AxisMapMenu.cpp

#for mapsearch
#AxisMiniMapWidget.o: AxisMiniMapWidget.cc AxisMiniMapWidget.h
#	$(CC) $(CFLAGS) -c AxisMiniMapWidget.cc









# cleaning everything that can be automatically recreated with "make".
#clean:
#        /bin/rm -f main main.o

