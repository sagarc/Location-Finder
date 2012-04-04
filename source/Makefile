
PROGRAMS=Locfinder

.SUFFIXES: .c .cpp .h .hpp

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)
	rm -f *.o
	rm -f *~

Locfinder: kdtree.h region.h prim.h main.cpp
	g++ main.cpp -o Locfinder
