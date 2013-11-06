CXX = g++
CXXFLAGS = -O3 -Wall -Winline -Wshadow -ansi
TARGET = matmult


$(TARGET): main.cpp matrix.h	 
	$(CXX) -c  $(CXXFLAGS) -o $(TARGET) main.cpp matrix.h	 
