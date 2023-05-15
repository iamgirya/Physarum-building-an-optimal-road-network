#include "SlimeMoldClass.h"

int main()
{
	//ввод настроек теперь мерез файл settings в директории проекта
	SlimeMoldSimulation sim = SlimeMoldSimulation(200, 200);

	auto tmp = AgentGraphAnalyser();
	tmp.exitPoints = {
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
		make_pair(153, 190)
	};

	tmp.graph = vector<vector<it>>(tmp.exitPoints.size());
	tmp.graph[0].push_back(1);
	tmp.graph[1].push_back(2);
	tmp.graph[2].push_back(3);
	tmp.graph[3].push_back(4);
	tmp.graph[3].push_back(5);
	tmp.graph[5].push_back(6);
	tmp.graph[6].push_back(7);
	tmp.graph[7].push_back(8);
	tmp.graph[8].push_back(9);
	tmp.graph[1].push_back(9);
	tmp.graph[3].push_back(9);
	tmp.graph[9].push_back(10);
	tmp.graph[10].push_back(11);
	tmp.graph[7].push_back(11);
	tmp.graph[11].push_back(12);
	tmp.graph[12].push_back(13);
	tmp.graph[11].push_back(14);
	tmp.graph[14].push_back(15);
	tmp.graph[14].push_back(16);
	tmp.graph[16].push_back(17);
	tmp.graph[16].push_back(18);
	tmp.graph[18].push_back(19);
	tmp.graph[18].push_back(20);
	tmp.graph[20].push_back(21);
	tmp.graph[9].push_back(21);
	tmp.graph[20].push_back(22);
	tmp.graph[22].push_back(23);
	for (int i = 0; i < tmp.graph.size(); i++) {
		for (it j : tmp.graph[i]) {
			if (i < j) {
				tmp.graph[j].push_back(i);
			}
		}
		tmp.towns.push_back(3);
		tmp.townIndexes.push_back(i);
	}
	tmp.towns[9] = 6;
	tmp.towns[23] = 1;
	tmp.towns[22] = 1;
	tmp.towns[19] = 1;
	tmp.towns[17] = 1;
	tmp.towns[15] = 1;
	tmp.towns[13] = 1;
	tmp.towns[4] = 1;
	tmp.towns[0] = 1;

	cout << tmp.calculateWeigth() << endl;
	cout << tmp.calculateDeltaFlow() << endl;
	cout << tmp.calculateOverDistance() << endl;
	cout << tmp.calculateResistance() << endl;
	cout << endl;
	sim.outputInBmpGraph(tmp.exitPoints, tmp.graph, true, -1);

	sim.startSimulation({ 45.5, 45.5 });
}

// 100 6 1 45 45 1 5 0 3 0.5 0 1 - крест с усиками
// 100 6 1 45 45 1 5 0 3 0.5 0 1 - крест с более длинными усиками
// 100 9 1 45 45 1 5 0 3 0.5 0 1 - крест, но вроде бы усов меньше
// 100 9 1 45 45 1 5 0 3 0.2 0 1 - знак деления
// 100 9 1 45 45 1 5 0 3 0.35 0 1 - same
// 100 9 1 45 22.5 1 5 0 3 0.5 0 1 - клевер переходящий в восьмёрку
// 100 9 1 45 60 1 5 0 3 0.5 0 1 - жёсткий крест
// 100 9 1 45 35 1 5 0 3 0.5 0 1 - крест с усами
// 100 9 1 22.5 22.5 1 5 0 3 0.5 0 1 - крест с усами
// 100 9 1 45 22.5 2 5 0 3 0.5 0 1 - клевер, переходящий в восьмёрку. В целом, достаточно оптимально, но не очень статично
// 150 9 1 45 22.5 2 5 0 3 0.5 0 1 - огромные восьмёрки, что не помещаются
// 60 9 1 45 22.5 1 5 0 3 0.5 0 1 - кое-какие линии
// 80 9 1 45 22.5 1 5 0 3 0.5 0 1 - то же
// 80 6 1 45 22.5 1 5 0 3 0.5 0 1 -  то же
// 80 6 1 45 10 1 5 0 3 0.5 0 1 - то же
// 100 9 1 45 60 3 5 0 3 0.5 0 1 - 
//
//


/*
tmp.exitPoints = {
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
		make_pair(153, 190)
	};

	tmp.graph = vector<vector<it>>(tmp.exitPoints.size());
	tmp.graph[0].push_back(1);
	tmp.graph[1].push_back(2);
	tmp.graph[2].push_back(3);
	tmp.graph[3].push_back(4);
	tmp.graph[3].push_back(5);
	tmp.graph[5].push_back(6);
	tmp.graph[6].push_back(7);
	tmp.graph[7].push_back(8);
	tmp.graph[8].push_back(9);
	tmp.graph[9].push_back(1);
	tmp.graph[9].push_back(3);
	tmp.graph[9].push_back(10);
	tmp.graph[10].push_back(11);
	tmp.graph[7].push_back(11);
	tmp.graph[11].push_back(12);
	tmp.graph[12].push_back(13);
	tmp.graph[11].push_back(14);
	tmp.graph[14].push_back(15);
	tmp.graph[14].push_back(16);
	tmp.graph[16].push_back(17);
	tmp.graph[16].push_back(18);
	tmp.graph[18].push_back(19);
	tmp.graph[18].push_back(20);
	tmp.graph[20].push_back(21);
	tmp.graph[9].push_back(21);
	tmp.graph[20].push_back(22);
	tmp.graph[22].push_back(23);
	for (int i = 0; i < tmp.graph.size(); i++) {
		for (it j : tmp.graph[i]) {
			tmp.graph[j].push_back(i);
		}
		tmp.towns.push_back(2);
		tmp.townIndexes.push_back(i);
	}
	tmp.towns[9] = 5;
	tmp.towns[23] = 1;
	tmp.towns[22] = 1;
	tmp.towns[19] = 1;
	tmp.towns[17] = 1;
	tmp.towns[15] = 1;
	tmp.towns[13] = 1;
	tmp.towns[4] = 1;
	tmp.towns[0] = 1;
*/