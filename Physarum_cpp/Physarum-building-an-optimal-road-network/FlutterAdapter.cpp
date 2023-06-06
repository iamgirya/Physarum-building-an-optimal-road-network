#include "SlimeMoldClass.h"

// TODO добавить ввод параметров через флаттер. В том числе и параметров анализатора
vector<vector<it>> FlutterAdapter::execute() {
	SlimeMoldSimulation sim = SlimeMoldSimulation(200, 200);
	sim.setUp(80, 0, 6, 45, 45, 1, 3, 1.5, 0, 1);
	sim.placeGenerators({ 
		make_pair(8,121),
		make_pair(35, 126),
		make_pair(35, 94),
		make_pair(59, 65),
		make_pair(20, 49),
		make_pair(45, 6),
		make_pair(82, 19),
		make_pair(98, 31),
		make_pair(82, 67),
		make_pair(72, 101),
		make_pair(95, 77),
		make_pair(123, 49),
		make_pair(146, 36),
		make_pair(182, 44),
		make_pair(153, 69),
		make_pair(191, 78),
		make_pair(135, 83),
		make_pair(158, 123),
		make_pair(105, 114),
		make_pair(122, 111),
		make_pair(96, 153),
		make_pair(83, 126),
		make_pair(127, 183),
		make_pair(153, 190) }, {
		1,
		3,
		3,
		3,
		1,
		3,
		3,
		3,
		3,
		6,
		3,
		3,
		3,
		1,
		3,
		1,
		3,
		1,
		3,
		1,
		3,
		3,
		1,
		1
		});
		

	sim.startSimulation(1000);
	return sim.analyser.bestGraph;
}