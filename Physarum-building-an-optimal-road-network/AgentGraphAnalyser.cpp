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
	vector<it> rezult;
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
						rezult.push_back(first);
						rezult.push_back(second);
						rezult.push_back(third);
						return rezult;
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

// �������� ��� begin_index ������� ���������� �������� ���������� ���� �� ������� �� townIndexes. ��� ����� ���� ��� �� ������������ ������� �� ���������� ������ ����
vector<vector<it>> AgentGraphAnalyser::diikstra(it begin_index) {
	if (weigthGraph.empty()) {
		buildWeigth();
	}

	const it MAXIT = INT_MAX;
	it SIZE = graph.size();

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

	vector<vector<it>> reversePath;
	for (int i : townIndexes) {

		// ���� ���������� � ����� �������
		vector<it> tmp = vector<it>(); tmp.push_back(i);
		int nowIndex = prevVertex[i];

		// ���� ���� ���, �� ���������� ������� -1
		if (nowIndex == -1) {
			reversePath.push_back(vector<it>());
			continue;
		}
		while (nowIndex != -1) {
			// ���� ��� ���� �� ��������� �������, �� ��������� ������ ����������
			tmp.push_back(nowIndex);
			nowIndex = prevVertex[nowIndex];
		}
		reversePath.push_back(tmp);
	}

	return reversePath;
}

// ��� ������ ���� ����������� ������� ���������� ������� ����� ����
// ���������� ������� ���������, �� � ���������� ������.
// ��� ������� �������� ���������� �� ���� � ��������� ����� ����� ������� ������������
void AgentGraphAnalyser::buildFlow() {
	this->flowGraph.clear();
	
	// ������, � ������� i,j ������ - ��� �������� ���������� ���� �� ������ i � j
	vector<vector<vector<it>>> ways;
	for (it i : townIndexes) {
		ways.push_back(diikstra(i));
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

	for (int i = 0; i < ways.size(); i++) {
		// ���������� ���� �� ����� ����� ����
		for (int j = i+1; j < ways[i].size(); j++) {
			if (ways[i][j].empty()) {
				continue;
			}

			it first;
			it second = ways[i][j][0];
			
			ft waysFlow = ft(towns[townIndexes[i]] * towns[townIndexes[j]]) / (sumOfPriority - towns[townIndexes[i]]);// ����� �� i � j
			waysFlow += towns[townIndexes[j]] * towns[townIndexes[i]] / (sumOfPriority - towns[townIndexes[j]]);// ����� �������
			for (int k = 1; k < ways[i][j].size(); k++) {
				first = second;
				second = ways[i][j][k];
				
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

// ����� ���� ���� ����
ft AgentGraphAnalyser::calculateWeigth() {
	if (weigthGraph.empty()) {
		buildWeigth();
	}

	ft rezult = 0;
	for (int i = 0; i < graph.size(); i++) {
		for (int j : graph[i]) {
			if (i < j) {
				rezult += weigthGraph[i][j];
			}
		}
	}
	return rezult;
}

// ������� ���� ������ � �����������
ft AgentGraphAnalyser::calculateDeltaFlow() {
	if (flowGraph.empty()) {
		buildFlow();
	}

	ft sumOfPriority = 0;
	for (int i = 0; i < towns.size(); i++) {
		sumOfPriority += towns[i];
	}

	ft sumOfFlow = 0;
	for (int i = 0; i < graph.size(); i++) {
		for (int j : graph[i]) {
			if (i < j) {
				sumOfFlow += flowGraph[i][j];
			}
		}
	}

	ft rezult = sumOfFlow - sumOfPriority;
	return rezult;
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

	ft rezult = 0;
	for (int i = 0; i < graph.size(); i++) {
		for (int j : graph[i]) {
			if (i < j) {
				rezult += pow(flowGraph[i][j] - averageFlow, 2);
			}
		}
	}
	rezult = sqrt(rezult);

	return rezult;
}

bool AgentGraphAnalyser::checkConnected() {
	int countVertex = graph.size();
	vector<bool> visited(graph.size(), false);

	queue<it> q;
	q.push(0); visited[0] = true;
	while (q.size()) {
		it i = q.front(); q.pop();
		for (it j : graph[i]) {
			visited[j] = true;
			q.push(j);
			countVertex--;
		}
	}

	return countVertex == 0;
}