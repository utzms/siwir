CXX = g++
CXXFLAGS = -O3 -Wall -Winline -Wshadow -ansi -g
TARGET = matmult



$(TARGET): main.o matrix.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o matrix.o

main.o: main.cpp matrix.h
	$(CXX) $(CXXFLAGS) -c main.cpp matrix.h

matrix.o: matrix.cpp matrix.h
	$(CXX) $(CXXFLAGS) -c matrix.cpp matrix.h
.PHONY: clean	

clean:
	rm $(TARGET) *.o 
