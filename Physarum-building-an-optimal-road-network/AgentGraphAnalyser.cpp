#include "SlimeMoldClass.h"

vector<pair<it, it>> AgentGraphAnalyser::makeMinimizedGraph(vector<SlimeAgent*> particles, vector<Generator*> generators) {
	vector<pair<pair<it,it>, bool>> positions;
	for (auto u : particles) {
		positions.push_back(make_pair(make_pair(u->pixelVector[0], u->pixelVector[1]), false));
	}
	for (auto u : generators) {
		positions.push_back(make_pair(make_pair(u->position[0], u->position[1]), true));
	}
	sort(positions.begin(), positions.end());

	auto distance = [](pair<it, it> i, pair<it, it> j) { return sqrt(pow(i.first - j.first, 2) + pow(i.second - j.second, 2)); };
	auto average = [](pair<it, it> i, pair<it, it> j) {return make_pair((i.first+j.first)/2, (i.second + j.second) / 2); };

	//однонаправленный список. first - Элемент, а second - индекс следующего элемента
	vector<pair<pair<it, it>, it>> edgeList;
	int count = 0;

	for (int i = 0; i < positions.size(); i++) {
		for (int j = i+1; j < positions.size(); j++) {
			//благодаря сортировке из n^2 переходим в n*log(n). А в случае с нашими графами ускорение практически в 15 раз
			if (abs(positions[i].first.first - positions[j].first.first) > edgesRange) {
				break;
			}
			if (distance(positions[i].first, positions[j].first) <= edgesRange) {
				edgeList.push_back(make_pair(make_pair(i, j), -1));
				count++;
			}
		}
	}
	std::random_device rd;
	std::mt19937 g(rd());
	shuffle(edgeList.begin(), edgeList.end(), g);
	for (int i = 0; i < edgeList.size()-1; i++) {
		edgeList[i].second = i + 1;
	}

	vector<it> teams(positions.size());
	for (int i = 0; i < positions.size(); i++) {
		teams[i] = i;
	}

	int startIndex = 0;
	bool hasChange = true;
	vector<ft> tmpDis1, tmpDis2;
	vector<pair<pair<it, it>, pair<it, it>>> tmpP1, tmpP2;
	while (hasChange) {
		tmpDis1.clear();
		tmpDis2.clear();
		tmpP1.clear();
		tmpP2.clear();
		hasChange = false;
		int prevIndex = -1;
		int index = startIndex;
		while (index != -1) {
			bool isNeedDelete = false;
			pair<it, it> edge = edgeList[index].first;
			if (teams[edge.first] == teams[edge.second]) {
				isNeedDelete = true;
			} else if (distance(positions[teams[edge.first]].first, positions[teams[edge.second]].first) <= edgesRange) {
				//если вторая точка - город, то изменяем первую под неё
				if (positions[teams[edge.second]].second) {
					positions[teams[edge.first]].first = positions[teams[edge.second]].first;
				}
				//если первая точка - не город, то делаем новую точку - среднее между двумя точками
				else if (!positions[teams[edge.first]].second) {
					positions[teams[edge.first]].first = average(positions[teams[edge.first]].first, positions[teams[edge.second]].first);
				}
				teams[edge.second] = teams[edge.first];
				isNeedDelete = true;
				hasChange = true;
			}
			if (isNeedDelete) {
				if (index == startIndex) {
					startIndex = edgeList[index].second;
				}
				else {
					edgeList[prevIndex].second = edgeList[index].second;
				}
				count--;
				tmpP2.push_back(make_pair(positions[teams[edge.first]].first, positions[teams[edge.second]].first));
				tmpDis2.push_back(distance(positions[teams[edge.first]].first, positions[teams[edge.second]].first));
			}
			else {
				prevIndex = index;
				tmpP1.push_back(make_pair(positions[teams[edge.first]].first, positions[teams[edge.second]].first));
				tmpDis1.push_back(distance(positions[teams[edge.first]].first, positions[teams[edge.second]].first));
			}
			index = edgeList[index].second;
		}
	}

	set<it> tmpExitVertex;
	vector<it> exitVertex;
	vector<it> expexitVertex;
	for (int i = 0; i < positions.size(); i++) {
		tmpExitVertex.insert(teams[i]);
	}

	for (auto u: tmpExitVertex) {
		exitVertex.push_back(u);
	}

	vector<pair< it, it >> exitPoints;
	for (int i = 0; i < exitVertex.size(); i++) {
		exitPoints.push_back(positions[exitVertex[i]].first);
	}

	sort(exitPoints.begin(), exitPoints.end());
	count = 0;

	vector<vector<it>> exitGraph(exitPoints.size(), vector<it>());
	for (int i = 0; i < exitPoints.size(); i++) {
		for (int j = i + 1; j < exitPoints.size(); j++) {
			//благодаря сортировке из n^2 переходим в n*log(n). А в случае с нашими графами ускорение практически в 15 раз
			if (abs(exitPoints[i].first - exitPoints[j].first) > vertexRange) {
				break;
			}
			if (distance(exitPoints[i], exitPoints[j]) <= vertexRange && distance(exitPoints[i], exitPoints[j]) >= 0.5) {
				exitGraph[i].push_back(j);
				exitGraph[j].push_back(i);
				count++;
			}
		}
	}

	/*vector<vector<it>> exitGraph(exitPoints.size(), vector<it>());
	int index = startIndex;
	while (index != -1) {
		pair<it, it> edge = edgeList[index].first;
		int i = lower_bound(exitVertex.begin(), exitVertex.end(), teams[edge.first]) - exitVertex.begin();
		int j = lower_bound(exitVertex.begin(), exitVertex.end(), teams[edge.second]) - exitVertex.begin();
		exitGraph[i].push_back(j);
		exitGraph[j].push_back(i);
		index = edgeList[index].second;	
	}*/

	this->graph = exitGraph;
	return exitPoints;
}