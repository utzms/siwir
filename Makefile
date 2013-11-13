CXX = g++
CXXFLAGS = -O3 -Wall -Winline -Wshadow -ansi -g -pthread -msse3
TARGET = matmult
INC =  -I/usr/local/likwid-3.0/include/
LDFLAGS = -L/usr/local/likwid-3.0/lib/
LIBS = -llikwid




$(TARGET): main.o matrix.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o matrix.o  $(INC) $(LDFLAGS) $(LIBS)

main.o: main.cpp matrix.h
	$(CXX) $(CXXFLAGS) -c main.cpp matrix.h  $(INC) $(LDFLAGS) $(LIBS)

matrix.o: matrix.cpp matrix.h
	$(CXX) $(CXXFLAGS) -c matrix.cpp matrix.h  $(INC) $(LDFLAGS) $(LIBS)
.PHONY: clean	

clean:
	rm $(TARGET) *.o *.h.gch 
