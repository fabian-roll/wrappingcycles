/*

This file grew out of many examples that are part of the Gudhi Library - https://gudhi.inria.fr/ - which is released under MIT.
 
MIT License

Copyright (c) 2014-2019 The GUDHI developers.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <iostream>
#include <string>
#include <gudhi/Alpha_complex_3d.h>
#include <gudhi/Simplex_tree.h>

using Alpha_complex_3d = Gudhi::alpha_complex::Alpha_complex_3d<Gudhi::alpha_complex::complexity::SAFE, false, false>;

void usage(int nbArgs, char * const progName) {
	std::cerr << "Error: Number of arguments (" << nbArgs << ") is not correct\n";
	std::cerr << "Usage: " << progName << " input_file ouput_file\n";
	exit(-1);
}

int main(int argc, char **argv) {
		if (argc != 3) usage(argc, argv[0]);

	std::string off_file_name(argv[1]);
	//double max_square_radius = atof(argv[2]);
	
	std::vector<Alpha_complex_3d::Point_3> points;
	std::ifstream stream(off_file_name);
	if (!stream.is_open()) {
		std::cerr << "could not open file \n";
		exit(-1);
	} else {
		std::string line;
	  	while (std::getline(stream, line)) {
			std::stringstream stream(line);
			std::vector<double> point;
			point.assign(std::istream_iterator<double>(stream), std::istream_iterator<double>());
			if (point.size() != 3) {
				std::cerr << "dimension of point != 3" << std::endl;
				exit(-1);
			}
			points.push_back(Alpha_complex_3d::Point_3(point[0], point[1], point[2]));
		}
	}
	
	std::cerr << "read " << points.size() << " points" << std::endl << std::flush;

	Alpha_complex_3d alpha_complex(points);
	
	std::cerr << "constructed alpha complex on " << points.size()  << " vertices" << std::endl << std::flush;

	Gudhi::Simplex_tree<> simplex_tree;
	alpha_complex.create_complex(simplex_tree);

	std::cerr << "constructed simplex tree on " << simplex_tree.num_vertices() << " vertices" << std::endl << std::flush;

	std::ofstream output_stream_points("point_cloud_sorted.txt", std::ofstream::out);

	std::ofstream output_stream(std::string(argv[2]), std::ofstream::out);

	output_stream << simplex_tree.num_vertices() << " " << simplex_tree.dimension() << std::endl;

	for (auto f_simplex : simplex_tree.filtration_simplex_range()) {
	    if(simplex_tree.dimension(f_simplex)==0){
                auto vertices = simplex_tree.simplex_vertex_range(f_simplex);
			    output_stream_points << alpha_complex.get_point(*vertices.begin());
			    output_stream_points << std::endl;
        }
		output_stream << "[";
		
		auto vertices = simplex_tree.simplex_vertex_range(f_simplex);
		auto it = vertices.begin();
		if (it != vertices.end()) {
			output_stream << *it++;
			while (it != vertices.end()) output_stream << " " << *it++;
		}
		
		output_stream << "] " << simplex_tree.filtration(f_simplex);
		output_stream << std::endl;
	}
	output_stream_points.close();
	output_stream.close();
	return 0;
}