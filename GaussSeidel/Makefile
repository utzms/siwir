CX = g++
CXXFLAGS = -O3 -Wall -Winline -Wshadow -ansi -g -pthread -msse3
TARGET =  rbgs
LDFLAGS = -L/usr/loca/
INC = 




$(TARGET): main.o grid.o matrix.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o grid.o  $(INC) $(LDFLAGS) $(LIBS)

main.o: main.cpp grid.h matrix.h
	$(CXX) $(CXXFLAGS) -c main.cpp grid.h matrix.h  $(INC) $(LDFLAGS) $(LIBS)

grid.o: grid.cpp grid.h matrix.h
	$(CXX) $(CXXFLAGS) -c grid.cpp grid.h matrix.h  $(INC) $(LDFLAGS) $(LIBS)
 
matrix.o: matrix.cpp matrix.h
	$(CXX) $(CXXFLAGS) -c matrix.cpp matrix.h  $(INC) $(LDFLAGS) $(LIBS)

.PHONY: clean	

clean:
	rm $(TARGET) *.o *.h.gch 
