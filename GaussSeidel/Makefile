CX = g++
CXXFLAGS = -O3 -Wall -Winline -Wshadow -ansi -g -pthread -msse3
TARGET =  rbgs
LDFLAGS = -L/usr/loca/
INC = 




$(TARGET): main.o grid.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o grid.o $(INC) $(LDFLAGS) $(LIBS)

main.o: main.cpp grid.h
	$(CXX) $(CXXFLAGS) -c main.cpp grid.h  $(INC) $(LDFLAGS) $(LIBS)

grid.o: grid.cpp grid.h
	$(CXX) $(CXXFLAGS) -c grid.cpp grid.h $(INC) $(LDFLAGS) $(LIBS)
 

.PHONY: clean	

clean:
	rm $(TARGET) *.o *.h.gch 
