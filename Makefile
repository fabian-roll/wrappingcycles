build: infiltrator delaunay_3d_filtration


infiltrator: infiltrator.cpp
	c++ -std=gnu++11 -Wall infiltrator.cpp -o infiltrator -O3 -D NDEBUG

delaunay_3d_filtration: delaunay_3d_filtration.cpp
	c++ -std=c++17 -Wall delaunay_3d_filtration.cpp -o delaunay_3d_filtration -lgmp -lmpfr -O3 -D NDEBUG


clean:
	rm -f infiltrator delaunay_3d_filtration
