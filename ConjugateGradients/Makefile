CXX = mpiCC
CXXFLAGS = -O3 -Wall -Winline -Wshadow -ansi -g
TARGET =  cg
LDFLAGS = -L/usr/loca/
INC = 
INCPATH=./include/
SRCPATH=./src/



$(TARGET): main.o grid.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o grid.o $(INC) $(LDFLAGS) $(LIBS)

main.o: $(SRCPATH)main.cpp $(INCPATH)grid.h
	$(CXX) $(CXXFLAGS) -c $(SRCPATH)main.cpp $(INCPATH)grid.h  $(INC) $(LDFLAGS) $(LIBS)

grid.o: $(SRCPATH)grid.cpp $(INCPATH)grid.h
	$(CXX) $(CXXFLAGS) -c $(SRCPATH)grid.cpp $(INCPATH)grid.h $(INC) $(LDFLAGS) $(LIBS)
 

.PHONY: clean	

clean:
	rm $(TARGET) *.o *.h.gch 
