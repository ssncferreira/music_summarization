# makefile
CXX = g++
RM = rm -f

# compiler flags
# -g adds debugging information to the executable file
# -Wall turns on most, but not all, compiler warnings
CFLAGS = -g -Wall
MARFLAG = -l marsyas

# the build target executable
TARGET = prog

all: $(TARGET)

$(TARGET): 
	$(CXX) $(CFLAGS) -o $(TARGET) segmenter.cpp outputPrinter.cpp matrix.cpp $(MARFLAG)

clean:
	 $(RM) $(TARGET) *.pgm *.ppm *.dat
