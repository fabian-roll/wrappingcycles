build: ripser delaunay_3d_filtration


ripser: ripser.cpp
	c++ -std=gnu++11 -Wall ripser.cpp -o ripser -O3 -D NDEBUG

delaunay_3d_filtration: delaunay_3d_filtration.cpp
	c++ -std=c++17 -Wall delaunay_3d_filtration.cpp -o delaunay_3d_filtration -lgmp -lmpfr -O3 -D NDEBUG


clean:
	rm -f ripser delaunay_3d_filtration
