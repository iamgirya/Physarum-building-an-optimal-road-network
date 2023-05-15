#include "SlimeMoldClass.h"

void AgentGraphAnalyser::eraseEdge(int index, int vertex) {
	graph[index].erase(find(graph[index].begin(), graph[index].end(), vertex));
}

bool AgentGraphAnalyser::findEdge(int index, int vertex) {
	return find(graph[index].begin(), graph[index].end(), vertex) != graph[index].end();
}

bool AgentGraphAnalyser::canConnectEdges(int i) {
	static ft minRezultVectorLength = -1;
	if (minRezultVectorLength == -1) {
		minRezultVectorLength = sqrt(pow(1 - cos(minEdgeAngle / 180 * PI), 2) + pow(sin(minEdgeAngle / 180 * PI), 2));
	}

	it firstIndex = graph[i][0];
	it secondIndex = graph[i][1];
	// ������� ������� ����
	pair<it, it> main = exitPoints[i];
	pair<ft, ft> firstVector = make_pair(main.first - exitPoints[firstIndex].first, main.second - exitPoints[firstIndex].second);
	pair<ft, ft> secondVector = make_pair(exitPoints[secondIndex].first - main.first, exitPoints[secondIndex].second - main.second);
	// ���������
	ft tmpLength = length(firstVector);
	firstVector = make_pair(firstVector.first / tmpLength, firstVector.second / tmpLength);
	tmpLength = length(secondVector);
	secondVector = make_pair(secondVector.first / tmpLength, secondVector.second / tmpLength);
	// ������: ��� ����� ���� ���� �� ��������� 10 �������� � ������, ���� ����� ������� ������������� �������� �� ������ 0.1737. ����� ���� ���������
	pair<ft, ft> rezultVector = make_pair(firstVector.first - secondVector.first, firstVector.second - secondVector.second);
	tmpLength = length(rezultVector);
	return tmpLength <= minRezultVectorLength;
}

vector<it> AgentGraphAnalyser::checkRomb(int index) {
	if (graph[index].size() <= 2) {
		return vector<it>();
	}
	vector<it> result;
	for (int i = 0; i < graph[index].size(); i++) {
		for (int j = i+1; j < graph[index].size(); j++) {
			it first = graph[index][i];
			it second = graph[index][j];
			// ���� ���� ����� ��� �������, ��� ��������� ������
			if (findEdge(first, second)) {
				for (int third: graph[first]) {
					// � � ���� ���� ������ ���� ������� � ���� �������
					if (third != index && findEdge(second, third)) {
						// �� ��� ���� � ��������� index, i, j, k
						result.push_back(first);
						result.push_back(second);
						result.push_back(third);
						return result;
					}
				}
			}
		}
	}
	return vector<it>();
}

//����������� ��������� ����� � ���������� ����
void AgentGraphAnalyser::makeGraph(vector<SlimeAgent*> particles, vector<Generator*> generators) {
	// ��������� ��� ����� � ���������� � ����� ������, �������� �� � ������� ������� ���������
	// TODO ����� ���������� �� ����� ����������� ��� � ������� ��� ������ �������
	vector<pair<pair<it,it>, it>> position;
	for (auto u : particles) {
		position.push_back(make_pair(make_pair(u->pixelVector[0], u->pixelVector[1]), 0));
	}
	for (auto u : generators) {
		position.push_back(make_pair(make_pair(u->position[0], u->position[1]), u->agentsPerStep));
	}
	sort(position.begin(), position.end());

	// ������ �� ������� ����������� ������. � i-��� �������� ��������� ������ ������� ���������� ��������� ��������
	vector<it> next(position.size(), -1);
	for (int i = 0; i < position.size()-1; i++) {
		next[i] = i + 1;
	}
	// ���������� �����, ������� ������� ������ �����
	vector<it> pointMass(position.size(), 1);

	bool hasChange = true;
	while (hasChange) {
		hasChange = false;
		for (int i = 0; i != -1; i = next[i]) {
			int prevIndex = i;
			for (int j = next[i]; j != -1; j = next[j]) {
				pair<it, it> firstPoint = position[i].first;
				pair<it, it> secondPoint = position[j].first;
				bool isNeedDelete = false;
				//��������� ���������� �� n^2 ��������� � n*log(n) � ������ � ������ �������
				if (abs(firstPoint.first - secondPoint.first) > vertexRange) {
					break;
				}
				// ���� ����� ���������� ������, ������� ����� j
				if (distance(firstPoint, secondPoint) <= vertexRange) {
					//���� ������ ����� - �����, �� �� �� ����� � ������. ����� � ����� ��������� ������� ����, �� ������ ������������� ��������� ���� �����. �������� ���� � ������� ���������� �����, ������� ������� � ����� �������
					if (position[j].second) {
						// � ������, ���� ��� ����� ����� � ����� �����, �� �� ����� �������� �������, ����� ���� ����� �������.
						if (firstPoint.first == secondPoint.first && firstPoint.second == secondPoint.second) {
							it tmp = position[j].second;
							position[j].second = position[i].second;
							position[i].second = tmp;
						}
						else {
							break;
						}
					}
					isNeedDelete = true;
					hasChange = true;
				}
				
				if (isNeedDelete) {
					next[prevIndex] = next[j];
					pointMass[i] += pointMass[j];
				}
				else {
					prevIndex = j;
				}
			}
		}
	} 

	vector<pair<it,it>> exitPoints;
	vector<it> towns;
	for (int i = 0; i != -1; i = next[i]) {
		if (pointMass[i] >= minVertexMass || position[i].second) {
			exitPoints.push_back(position[i].first);
			towns.push_back(position[i].second);
		}
	}

	// ����� count
	int count = 0;
	vector<vector<it>> exitGraph(exitPoints.size(), vector<it>());
	for (int i = 0; i < exitPoints.size(); i++) {
		for (int j = i + 1; j < exitPoints.size(); j++) {
			if (abs(exitPoints[i].first - exitPoints[j].first) > edgesRange) {
				break;
			}
			ft tmpDistance = distance(exitPoints[i], exitPoints[j]);
			if (tmpDistance <= edgesRange) {
				exitGraph[i].push_back(j);
				exitGraph[j].push_back(i);
				count++;
			}
		}
	}

	this->towns = towns;
	this->graph = exitGraph;
	this->exitPoints = exitPoints;
}

// ������ 4 �������� ����������:
// �������� - ������� � 1 ������ ��� ��� ���;
// ������� - �����, �������� �� ��������� ����, ��� ����� ��������� � ����;
// ������������ - �������, ��� ��������� ������ � ����� ���������, ��� ��� ���������;
// ����� - ������ �������, ������� ������ �������� ������ ��������.
void AgentGraphAnalyser::minimizeGraph() {
	// ���� ����� ���-�� �������, �� ���������� �����
	vector<it> vertexToDelete;
	// TODO ����� ���������� �� ����������� ������, ����� ������ �� ������ ����
	vector<bool> vertexWhatDeleted(exitPoints.size(), false);
	// ������� ������ ��� �������, ����� �� �������, ��� ���� ��������� ���������, ����� �� �������, ��� ���� ��������� ��� ����� ���������...
	queue<it> vertexToCheck;
	for (int i = 0; i < exitPoints.size(); i++) {
		vertexToCheck.push(i);
	}

	while (!vertexToCheck.empty()) {
		int i = vertexToCheck.front(); vertexToCheck.pop();
		if (!vertexWhatDeleted[i]) {
			if (!towns[i]) {
				// �������� �������
				if (graph[i].size() <= 1) {
					vertexToDelete.push_back(i);
					vertexWhatDeleted[i] = true;
					if (graph[i].size() == 1) {
						eraseEdge(graph[i][0], i);
						vertexToCheck.push(graph[i][0]);
					}
				}
				// �����������
				else if (graph[i].size() == 2 && findEdge(graph[i][0], graph[i][1])) {
					it firstIndex = graph[i][0];
					it secondIndex = graph[i][1];

					vertexToDelete.push_back(i);
					vertexWhatDeleted[i] = true;

					eraseEdge(firstIndex, i);
					vertexToCheck.push(firstIndex);
					eraseEdge(secondIndex, i);
					vertexToCheck.push(secondIndex);
				}
				// �������
				else if (graph[i].size() == 2 && canConnectEdges(i)) { 
					it firstIndex = graph[i][0];
					it secondIndex = graph[i][1];
					// ���������
					vertexToDelete.push_back(i);
					vertexWhatDeleted[i] = true;
					// ������� ���� � i-� ��������
					eraseEdge(firstIndex, i);
					eraseEdge(secondIndex, i);
					// ���� ������� �� ���������, ���������
					if (!findEdge(firstIndex, secondIndex)) {
						graph[firstIndex].push_back(secondIndex);
						graph[secondIndex].push_back(firstIndex);
					}
					// �������� ��� �������
					vertexToCheck.push(firstIndex);
					vertexToCheck.push(secondIndex);
				}
			} 
			// ����, ������ i-� ������� ����� ���� �������
			{
				
				vector<it> rombVertex = checkRomb(i);
				if (!rombVertex.empty()) {
					// ���� ��� ���������� ����
					if (findEdge(i, rombVertex[2])) {
						int townCount = 0;
						for (auto u : rombVertex) {
							if (towns[u]) {
								townCount++;
							}
						}
						townCount += towns[i] != 0 ? 1 : 0;
						if (townCount >= 2) {
							continue;
						}
						else {
							// ���� ������� ���� �� ���� ����� - ��������� ��������� �������
							bool hasTown = towns[i];
							// ������ ������� ������� �� ������
							for (int k: rombVertex) {
								// �������� ������� �� �������� ��� � ������� � ������
								if (!hasTown) {
									if (towns[k]) {
										exitPoints[i] = exitPoints[k];
										towns[i] = towns[k];
										towns[k] = 0;
										hasTown = true;
									}
									else {
										exitPoints[i] = average(exitPoints[i], exitPoints[k]);
									}
								}
								
								for (int v: graph[k]) {
									// ��������� ������ ������� �� �������� ���������
									if (i != v && !findEdge(i, v)) {
										graph[i].push_back(v);
										graph[v].push_back(i);
									}
									// ������� ���� � k-������� � ��������� ��� ������� �������.
									eraseEdge(v, k);
									vertexToCheck.push(v);
								}

								vertexToDelete.push_back(k);
								vertexWhatDeleted[k] = true;
							}
						}
					}
					else {
						if (towns[rombVertex[0]] && towns[rombVertex[1]]) {
							continue;
						}
						// ������ ������� ������� �� ����, ���������� ����������, ������� ������ � ������ ������
						it firstIndex = -1;
						it secondIndex = -1;
						if (towns[rombVertex[1]]) {
							firstIndex = rombVertex[1];
							secondIndex = rombVertex[0];
						}
						else {
							firstIndex = rombVertex[0];
							secondIndex = rombVertex[1];
						}
						// ���� ��� �� ������, �� �������� ������� �� ��������
						if (!towns[firstIndex] && !towns[secondIndex]) {
							exitPoints[firstIndex] = average(exitPoints[firstIndex], exitPoints[secondIndex]);
						}
						// ��� ������ ������� �������: ��������� � � ������ ��������, ������� ���� �� ������ �������� � ��������� �� � �����������
						for (int j: graph[secondIndex]) {
							if (firstIndex != j && !findEdge(firstIndex, j)) {
								graph[firstIndex].push_back(j);
								graph[j].push_back(firstIndex);
							}
							eraseEdge(j, secondIndex);
							vertexToCheck.push(j);
						}
						vertexToDelete.push_back(secondIndex);
						vertexWhatDeleted[secondIndex] = true;
					}
				}

				
			}
		}
	}


	// ����� �� ��������� ����� ������� ��� �������, ��� ���� �����. ��� ����� ������������ ������� ���� � ���������, ��� ������ ������� ������� ��� ���� �������� ������
	// ����� ����� ������ ������� ��� ����� �� ������ �����. sort(vertexToDelete.begin(), vertexToDelete.end());
	// �������� ����������� �� O(e+n)
	vector<it> shiftVector;
	vector<vector<it>> newGraph;
	vector<pair<it, it>> newExitPoints;
	vector<it> newTowns;
	vector<it> townIndexes;
	int lastDeleteIndex = 0;
	sort(vertexToDelete.begin(), vertexToDelete.end());
	for (int i = 0; i < exitPoints.size(); i++) {
		if (lastDeleteIndex != vertexToDelete.size() && i == vertexToDelete[lastDeleteIndex]) {
			lastDeleteIndex++;
		}
		else {
			newExitPoints.push_back(exitPoints[i]);
			newGraph.push_back(graph[i]);
			newTowns.push_back(towns[i]);
			if (towns[i]) {
				townIndexes.push_back(newTowns.size()-1);
			}
		}
		// ���������� �����
		shiftVector.push_back(lastDeleteIndex);
	}
	// ������: ���� ����� 1-3 � 3-1. ������� ������� 2. ������ ��� ����� ����� 1-2 2-1. ����� 2-1 ���� ���������� �� �����, ��� ��� �� ���������� ������� ������� 2 � ����� �����.
	// ����� 1-2 �� ������ ������� �� ����� 1-3 ���, ��� �� "3" ������ ���-�� �������� �� �� ������ - �.�. 1
	for (int i = 0; i < newGraph.size(); i++) {
		for (int j = 0; j < newGraph[i].size(); j++) {
			newGraph[i][j] -= shiftVector[newGraph[i][j]];
			if (newGraph[i][j] == i) {
				cout << 1;
			}
		}
	}

	this->towns = newTowns;
	this->graph = newGraph;
	this->townIndexes = townIndexes;
	this->exitPoints = newExitPoints;
}

// ������ ���� ���������� ����� ��� ������� �� townIndexes. ��� ������� i � j �������� ���������� ����� ���� ����. ��� ����� ���� ��� �� ������������ ������� �� ������ ������ ����
void AgentGraphAnalyser::buildWays() {
	waysGraph.clear();
	if (weigthGraph.empty()) {
		buildWeigth();
	}

	vector<vector<vector<it>>> ways;
	const it MAXIT = INT_MAX;
	it SIZE = graph.size();
	for (it begin_index : townIndexes) {
		vector<vector<it>> way;

		// ����������� ����������. ������ � ������� ������ ����� ��� ����������� ���������. � ������� ���������� ������������� ��������
		vector<ft> d(SIZE, MAXIT); d[begin_index] = 0;
		priority_queue<pair<ft, it>> q; q.push(make_pair(-0, begin_index));
		// ���������� �������
		vector<it> visited(SIZE, 0);
		// ����� - ��� ������ �������, ������ ��� ����. ��� ��������� ������� ����� ����� -1
		vector<it> prevVertex(SIZE, -1);

		it minIndex, min;
		pair<ft, it> tmp;
		// TODO �������������� ���-�� � �������������� �������
		// ��� ���������
		do {
			// ���� �������, � ������� ������ ����� ������ � ������� �� ��� �� ��������
			do {
				if (!q.empty()) {
					tmp = q.top(); q.pop();
				}
				else {
					goto endAlgo;
				}
			} while (visited[tmp.second]);
			min = -tmp.first;
			minIndex = tmp.second;
			// ��� ���� �������
			for (int i : graph[minIndex])
			{

				if (!visited[i] && weigthGraph[minIndex][i] > 0) // ���� -1, �� ����� ���
				{
					// ���� �� ���� ������� ������ ������ � �������, �� ������������� ����
					ft tmp = min + weigthGraph[minIndex][i];
					if (tmp < d[i])
					{
						d[i] = tmp;
						q.push(make_pair(-d[i], i));

						prevVertex[i] = minIndex;
					}
				}
			}
			// ��������� � �������
			visited[minIndex] = 1;
		} while (true); // ����� ����� �������
	endAlgo:

		for (int i : townIndexes) {

			// ���� ���������� � ����� �������
			vector<it> tmp = vector<it>(); tmp.push_back(i);
			int nowIndex = prevVertex[i];

			// ���� ���� ���, �� ���������� ������� -1
			if (nowIndex == -1) {
				way.push_back(vector<it>());
				continue;
			}
			while (nowIndex != -1) {
				// ���� ��� ���� �� ��������� �������, �� ��������� ������ ����������
				tmp.push_back(nowIndex);
				nowIndex = prevVertex[nowIndex];
			}
			way.push_back(tmp);
		}

		ways.push_back(way);
	}
	this->waysGraph = ways;
}

// ��� ������ ���� ����������� ������� ���������� ������� ����� ����
// ���������� ������� ���������, �� � ���������� ������.
// ��� ������� �������� ���������� �� ���� � ��������� ����� ����� ������� ������������
void AgentGraphAnalyser::buildFlow() {
	this->flowGraph.clear();
	if (waysGraph.empty()) {
		buildWays();
	}

	// ���������� � weigthGraph, ����� �� ������ ���� � ������
	vector<vector<ft>> flowGraph(graph.size(), vector<ft>(graph.size(), -1));
	for (int i = 0; i < graph.size(); i++) {
		for (int j: graph[i]) {
			flowGraph[i][j] = 0;
		}
	}

	ft sumOfPriority = 0;
	for (int i = 0; i < towns.size(); i++) {
		sumOfPriority += towns[i];
	}

	for (int i = 0; i < waysGraph.size(); i++) {
		// ���������� ���� �� ����� ����� ����
		for (int j = i+1; j < waysGraph[i].size(); j++) {
			if (waysGraph[i][j].empty()) {
				continue;
			}

			it first;
			it second = waysGraph[i][j][0];
			
			ft waysFlow = ft(towns[townIndexes[i]] * towns[townIndexes[j]]) / (sumOfPriority - towns[townIndexes[i]]);// ����� �� i � j
			waysFlow += towns[townIndexes[j]] * towns[townIndexes[i]] / (sumOfPriority - towns[townIndexes[j]]);// ����� �������
			for (int k = 1; k < waysGraph[i][j].size(); k++) {
				first = second;
				second = waysGraph[i][j][k];
				
				flowGraph[first][second] += waysFlow;
				flowGraph[second][first] += waysFlow;
			}
		}
	}

	this->flowGraph = flowGraph;
}

// ������ ������� � ����� ����, ��� ��� - ���������� ����� ���������
void AgentGraphAnalyser::buildWeigth() {
	weigthGraph.clear();
	weigthGraph = vector<vector<ft>>(graph.size(), vector<ft>(graph.size(), -1));

	for (int i = 0; i < graph.size(); i++) {
		for (int j : graph[i]) {
			weigthGraph[i][j] = distance(exitPoints[i], exitPoints[j]);
		}
	}

	this->weigthGraph = weigthGraph;
}

// ��������� �������� �� ������� ��� ����� ����� ��������� ���������� ���������
ft AgentGraphAnalyser::calculateWeigth() {
	if (weigthGraph.empty()) {
		buildWeigth();
	}

	ft minWeigth = 0;
	// ������ ����� - ����� �����, ������ - �������, ���� ��� �����
	priority_queue<pair<ft, it>> edges;
	vector<int> visited(townIndexes.size(), 0); visited[0] = 1;
	for (it i = 1; i < townIndexes.size(); i++) {
		edges.push(make_pair(-distance(exitPoints[townIndexes[0]], exitPoints[townIndexes[i]]), i));
	}
	int countOfVertex = townIndexes.size()-1;
	while (countOfVertex) {
		auto edge = edges.top(); edges.pop();
		if (visited[edge.second]) {
			continue;
		}
		minWeigth -= edge.first;
		countOfVertex--;
		visited[edge.second] = 1;
		// �� ��� ������ ����� for
		for (it i = 1; i < townIndexes.size(); i++) {
			if (!visited[i]) {
				edges.push(make_pair(-distance(exitPoints[townIndexes[edge.second]], exitPoints[townIndexes[i]]), i));
			}
		}
	}

	ft result = 0;
	for (int i = 0; i < graph.size(); i++) {
		for (int j : graph[i]) {
			if (i < j) {
				result += weigthGraph[i][j];
			}
		}
	}
	result /= minWeigth;


	return result;;
}

// ��������� �� ������� ��� � ������� ���� ����� �������, � ��������� � ������ ������
ft AgentGraphAnalyser::calculateOverDistance() {
	if (waysGraph.empty()) {
		buildWays();
	}

	ft sumOfPriority = 0;
	for (int i = 0; i < towns.size(); i++) {
		sumOfPriority += towns[i];
	}

	ft waysCount = 0; ft overMult = 0;
	for (int i = 0; i < waysGraph.size(); i++) {
		// ���������� ���� �� ����� ����� ����
		for (int j = i + 1; j < waysGraph[i].size(); j++) {
			ft minDistance = distance(exitPoints[townIndexes[i]], exitPoints[townIndexes[j]]);
			ft nowDistance = 0;

			ft waysFlow = ft(towns[townIndexes[i]] * towns[townIndexes[j]]) / (sumOfPriority - towns[townIndexes[i]]);// ����� �� i � j
			waysFlow += towns[townIndexes[j]] * towns[townIndexes[i]] / (sumOfPriority - towns[townIndexes[j]]);// ����� �������

			it first;
			it second = waysGraph[i][j][0];
			for (int k = 1; k < waysGraph[i][j].size(); k++) {
				first = second;
				second = waysGraph[i][j][k];

				nowDistance += weigthGraph[first][second];
			}

			waysCount++;
			overMult += nowDistance * waysFlow / minDistance;
		}
	}
	overMult /= sumOfPriority;
	
	return overMult;
}

// ������� ���������� ������ � ����������� ������� ���� - ����� �������������� �������� �����
ft AgentGraphAnalyser::calculateResistance() {
	it bridgeCount = 0;
	it maxCycle = 0;
	vector<int> stressed(graph.size(), 0);
	
	ft sumOfPriority = 0;
	for (int i = 0; i < towns.size(); i++) {
		sumOfPriority += towns[i];
	}

	// �������� ��� ��������� ����. ������� ��� ����� � �������� ����� ���� �� i � j. ���� ���� ����� �����, �� ����������, ��� �� ����� ����, ����� ����
	for (int i = 0; i < graph.size(); i++) {
		for (it j : graph[i]) {
			if (i >= j) {
				continue;
			}
			ft nowSum = sumOfPriority - towns[i];
			vector<int> visited(graph.size(), 0); visited[i] = 1;
			queue<pair<it, it>> vertex;
			// ������� ���������� �� ���� ����� �� i, ����� ��������� ������� �� �������� ����� i j
			for (it k : graph[i]) {
				if (k != j) {
					vertex.push(make_pair(k, 1));
					visited[k] = 1;
					nowSum -= towns[k];
				}
			}
			
			// ����� ����� ��������� ��� ������� �������� � ������� �������� ������� � ������
			it index;
			it size;
			while (vertex.size()) {
				index = vertex.front().first;
				size = vertex.front().second; vertex.pop();
				
				for (it k : graph[index]) {
					if (k == j) {
						visited[j] = 1;
						goto endWhile;
					}
					if (!visited[k]) {
						vertex.push(make_pair(k, size + 1));
						visited[k] = 1;
						nowSum -= towns[k];
					}
				}
			}
			endWhile:
			// ���� ������ ������ � j �������, ������ ���� ����
			if (visited[j]) {
				maxCycle = max(maxCycle, size + 1);
			}
			// ����� ���� ��� �� ������� � ������ i j - ����
			else {
				// ���� �� ������ ������� ����� �� ��������� ����������, �� ��� ���������� ������� ������� � ���� �����, ����� ��� ������������
				bool smallerPart = nowSum > sumOfPriority / 2;
				for (int k = 0; k < visited.size(); k++) {
					if (smallerPart == visited[k]) {
						stressed[k] = 1;
					}
				}
				
			}
		}
	}

	ft sumOfStressedPriority = 0;
	for (int i = 0; i < towns.size(); i++) {
		if (stressed[i]) {
			sumOfStressedPriority += towns[i];
		}
	}

	// ��������� +1 ������, ��� ��� ������� ������ 1
	return 1+ sumOfStressedPriority/sumOfPriority;
}

// �.�. ��� ���������� ������ ��, ���� ���� _�����_ �������� ��� ������ ����� ��� �������������� �����, �� ����� ������ ��� ���������� ����
// ���� ����� - ������������ ����� �� ����� �� ����. ��� � ������ ����� �������, ������ �����, ��� ����� ����� ������� ���������
// ��� ��� �� ��������� �������� ������ ��� ����� ������� ����� � ���� ������ ����� �������� ������ ��������� �����.
ft AgentGraphAnalyser::calculateDeltaFlow() {
	if (flowGraph.empty()) {
		buildFlow();
	}

	ft maxPriority = max(towns[0], towns[1]);
	ft secondMaxPriority = min(towns[0], towns[1]);
	for (int i = 0; i < towns.size(); i++) {
		if (towns[i] > maxPriority) {
			secondMaxPriority = maxPriority;
			maxPriority = towns[i];
		}
		else if (towns[i] > secondMaxPriority) {
			secondMaxPriority = towns[i];
		}
	}
	ft minMaxEdgeFlow = maxPriority + secondMaxPriority;

	ft maxEdgeFlow = 0;
	for (int i = 0; i < graph.size(); i++) {
		for (int j : graph[i]) {
			if (i < j) {
				if (flowGraph[i][j] > maxEdgeFlow) {
					maxEdgeFlow = flowGraph[i][j];
				}
			}
		}
	}

	ft result = maxEdgeFlow / minMaxEdgeFlow;
	return result;
}

// ��������� ���� ����� � ����� �� ���������� ����.
// ���������� �� ������� ����� � ��������� ������� ������� ��� ������ � �������� ����, ��������� � ������� ������
ft AgentGraphAnalyser::calculateOmega() {
	if (flowGraph.empty()) {
		buildFlow();
	}

	ft countOfEdge = 0;
	ft sumOfFlow = 0;
	for (int i = 0; i < graph.size(); i++) {
		for (int j : graph[i]) {
			if (i < j) {
				sumOfFlow += flowGraph[i][j];
				countOfEdge++;
			}
		}
	}

	ft averageFlow = sumOfFlow / countOfEdge;

	ft result = 0;
	for (int i = 0; i < graph.size(); i++) {
		for (int j : graph[i]) {
			if (i < j) {
				result += pow(flowGraph[i][j] - averageFlow, 2);
			}
		}
	}
	result = sqrt(result);

	return result;
}

bool AgentGraphAnalyser::checkConnected() {
	int countVertex = graph.size() -1;
	vector<bool> visited(graph.size(), false);

	queue<it> q;
	q.push(0); visited[0] = true;
	while (q.size()) {
		it i = q.front(); q.pop();
		for (it j : graph[i]) {
			if (!visited[j]) {
				visited[j] = true;
				q.push(j);
				countVertex--;
			}
		}
	}

	return countVertex == 0;
}