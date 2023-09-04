#include <cassert>

#include "../include/geometry.hpp"
#include "../include/volume.hpp"
#include "../include/cpu.hpp"
#include "../include/gpu.hpp"

/* cSpell:disable */
/*
• Точка O(0, 0, 0) всегда лежит внутри фигуры;
• Фигура всегда является выпуклым многогранником;
• В коде программы явно указаны входные точки и плоскости;
• При изменении координат точек программа должна вести себя корректно(при соблюдении предыдущих условий);
• Для вычислений использовать тип данных как минимум float. Для доп. задания можно использовать half;
• Разница в точности для CPU и GPU реализации не больше 0.001. Разница между ММК и аналитическим - 0.1;
• Для GPU должна использоваться разделяемая память;
• Для CPU измерение должно проводиться с помощью high − precision clock, для GPU - с помощью событий;
• Реализация на GPU должна использовать размер блока в пределах от 32 до максимально
допустимого вашей GPU. Программа должна поддерживать смену размера блока и по возможности вести себя валидно.
*/
/* cSpell:enable */
int main(const int argc, const char *const *argv) {
	bool noGpuInfo = true, defaultValues = true;
	string pointNames = "ABCDEF";
	if (argc == 1) {
		cout << "Lab 4: MMK cpu and gpu" << endl
		     << "Options: default - use default values (if set ignores setted points)" << endl
		     << "		  noGpuInfo - does not output info about GPU" << endl
		     << "		  <A,B,C,D,E,F> [<num>,<num>,<num>] - uses given values for mentioned point (if does not mentioned uses defaults)" << endl
		     << "examples: ./build/run A[-1, 0, 0.5] noGpuInfo" << endl;
			 return 0;
	}

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "default") == 0) {
			defaultValues = false;
		} else if (strcmp(argv[i], "noGpuInfo") == 0) {
			noGpuInfo = false;
		}
	}
	if (noGpuInfo)
		GPU::outputCudaInfo();
	// Working with points
	struct Geometry3D::Point points[6];
	// Setting default values (given by variant)
	points[0] = {-1, -1, -1};
	points[1] = {-1, 1, -1};
	points[2] = {1, -1, -1};
	points[3] = {1, 1, 1};
	points[4] = {-1, 1, 1};
	points[5] = {1, -1, 1};

	if (defaultValues)
		// Setting values from command line
		for (int i = 1; i < argc; ++i) {
			string line = argv[i];
			if (pointNames.find(line[0]) != string::npos) {
				int pointIndex       = pointNames.find(line[0]);
				line                 = line.substr(2, line.length() - 3);
				int commaIndex       = line.find(',');
				points[pointIndex].x = stof(line.substr(0, commaIndex));
				line                 = line.substr(commaIndex + 1, line.length() - commaIndex - 1);
				commaIndex           = line.find(',');
				points[pointIndex].y = stof(line.substr(0, commaIndex));
				line                 = line.substr(commaIndex + 1, line.length() - commaIndex - 1);
				points[pointIndex].z = stof(line);
			}
		}

	// Printing points
	cout << "Points:" << endl;
	for (int i = 0; i < 6; ++i) {
		cout << "\t" << pointNames[i] << ": " << points[i]() << endl;
	}
	// ended working with points

	//! \note Create hexahedron
	const Geometry3D::Polyhedron hexahedron = MonteCarlo::Hexahedron(points[0], points[1], points[2], points[3], points[4], points[5]);
	//! \note Extract vertices
	const std::vector<Geometry3D::Point> vertices = hexahedron.vertices();
	const size_t n_vertices = vertices.size();
	if (n_vertices != 8) {
		cerr << "Oops... Seems like number of vertices (" << n_vertices << ") doesn't equal to 8. Do better." << endl;
		return 1;
	}
	//! \note Find the most distant vertex
	const Geometry3D::Point most_distant_vertex = hexahedron.most_distant();
	//! \note Get the most distant from ORIGIN coordinate
	const xyz_type max_distance = std::max(abs(most_distant_vertex.x), std::max(abs(most_distant_vertex.y), abs(most_distant_vertex.z)));
	//! \note Get the PRNG limit by multiplying max_distance by MULTIPLIER_COEFFICIENT
	const xyz_type prng_limit = max_distance * MULTIPLIER_COEFFICIENT;
	const xyz_type cube_volume = pow((2 * prng_limit), 3);

	assert(hexahedron.planes.size() == 6);
	const Geometry3D::Plane planes[6] = {hexahedron.planes[0], hexahedron.planes[1], hexahedron.planes[2],
										 hexahedron.planes[3], hexahedron.planes[4], hexahedron.planes[5]};
	
	// doing tasks

	const xyz_type ratio_CPU = CPU::MonteCarlo(planes, prng_limit, N_OF_ITERATIONS);
	cout << "CPU ratio: " << ratio_CPU << endl;
	const xyz_type volume_CPU = cube_volume * ratio_CPU;
	cout << "CPU Volume: " << volume_CPU << endl;
	// not const since might be changed using cli (must be lower than 1024)
	size_t AmountOfThreads = 1024;
	const xyz_type ratio_GPU = GPU::initCudaCalc(planes, prng_limit, AmountOfThreads);
	cout << "GPU ratio: " << ratio_GPU << endl;
	const xyz_type volume_GPU = cube_volume * ratio_GPU;
	cout << "GPU Volume: " << volume_GPU << endl;

	return 0;
}