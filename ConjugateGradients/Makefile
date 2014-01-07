CXX = mpiCC
CXXFLAGS = -O3 -Wall -Winline -Wshadow -ansi -g
TARGET =  cg
INC = 
INCPATH=./include/
SRCPATH=./src/



$(TARGET): main.o grid.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o grid.o $(INC) $(LIBS)

main.o: $(SRCPATH)main.cpp $(INCPATH)grid.h
	$(CXX) $(CXXFLAGS) -c $(SRCPATH)main.cpp $(INCPATH)grid.h  $(INC) $(LIBS)

grid.o: $(SRCPATH)grid.cpp $(INCPATH)grid.h
	$(CXX) $(CXXFLAGS) -c $(SRCPATH)grid.cpp $(INCPATH)grid.h $(INC) $(LIBS)
 

.PHONY: clean	

clean:
	rm $(TARGET) *.o *.h.gch 
