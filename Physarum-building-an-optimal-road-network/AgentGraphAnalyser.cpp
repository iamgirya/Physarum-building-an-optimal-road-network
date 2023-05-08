#include "SlimeMoldClass.h"

void AgentGraphAnalyser::eraseEdge(int index, int vertex) {
	graph[index].erase(find(graph[index].begin(), graph[index].end(), vertex));
}

bool AgentGraphAnalyser::findEdge(int index, int vertex) {
	return find(graph[index].begin(), graph[index].end(), vertex) != graph[index].end();
}

bool AgentGraphAnalyser::canConnectEdges(int i) {
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
vector<pair<it, it>> AgentGraphAnalyser::makeGraph(vector<SlimeAgent*> particles, vector<Generator*> generators) {
	// ��������� ��� ����� � ���������� � ����� ������, �������� �� � ������� ������� ���������
	// TODO ����� ���������� �� ����� ����������� ��� � ������� ��� ������ �������
	vector<pair<pair<it,it>, bool>> position;
	for (auto u : particles) {
		position.push_back(make_pair(make_pair(u->pixelVector[0], u->pixelVector[1]), false));
	}
	for (auto u : generators) {
		position.push_back(make_pair(make_pair(u->position[0], u->position[1]), true));
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
				bool isNeedDelete = false;
				//��������� ���������� �� n^2 ��������� � n*log(n). � � ������ � ������ ������� ��������� ����������� � 15 ���
				if (abs(position[i].first.first - position[j].first.first) > vertexRange) {
					break;
				}
				// ���� ����� ���������� ������, ������� ����� j
				if (distance(position[i].first, position[j].first) <= vertexRange) {
					//���� ������ ����� - �����, �� �� �� ����� � ������. ����� � ����� ��������� ������� ����, �� ������ ������������� ��������� ���� �����. �������� ���� � ������� ���������� �����, ������� ������� � ����� �������
					if (position[j].second) {
						break;
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
	vector<bool> towns;
	for (int i = 0; i != -1; i = next[i]) {
		if (pointMass[i] >= minVertexMass || position[i].second) {
			exitPoints.push_back(position[i].first);
			towns.push_back(position[i].second);
		}
	}


	int count = 0;
	vector<vector<it>> exitGraph(exitPoints.size(), vector<it>());
	for (int i = 0; i < exitPoints.size(); i++) {
		for (int j = i + 1; j < exitPoints.size(); j++) {
			if (distance(exitPoints[i], exitPoints[j]) <= edgesRange) {
				exitGraph[i].push_back(j);
				exitGraph[j].push_back(i);
				count++;
			}
		}
	}

	this->towns = towns;
	this->graph = exitGraph;
	this->exitPoints = exitPoints;
	//minimizeLongEndge();
	return exitPoints;
}

// ������ 4 ��������:
// �������� - ������� � 1 ������ ��� ��� ���
// ������� - �����, �������� �� ��������� ����, ��� ����� ��������� � ����
// ������������ - �������, ��� ��������� ������ � ����� ���������, ��� ��� ���������
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

		// ����� ���
		for (int k = 0; k < graph.size(); k++) {
			for (int j = 0; j < graph[k].size(); j++) {
				if (!vertexWhatDeleted[k] && !vertexWhatDeleted[graph[k][j]] && !findEdge(graph[k][j], k)) {
					cout << 1;
				}
			}
		}
		int thfd = 0;

		int i = vertexToCheck.front(); vertexToCheck.pop();
		if (!vertexWhatDeleted[i]) {
			if (!towns[i]) {
				thfd = 1;
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
					thfd = 2;
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
					thfd = 3;
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
						thfd = 4;
						int townCount = 0;
						for (auto u : rombVertex) {
							if (towns[u]) {
								townCount++;
							}
						}
						townCount += towns[i];
						if (townCount >= 2) {
							continue;
						}
						else {
							// ���� ������� ���� �� ���� ����� - ��������� ��������� �������
							bool hasTown = false;
							// ������ ������� ������� �� ������
							for (int k: rombVertex) {
								// �������� ������� �� ��������
								if (!hasTown) {
									if (towns[k]) {
										exitPoints[i] = exitPoints[k];
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
						thfd = 5;
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
						if (!towns[rombVertex[0]] && !towns[rombVertex[1]]) {
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
	

		// ����� ���
		for (int k = 0; k < graph.size(); k++) {
			for (int j = 0; j < graph[k].size(); j++) {
				if (!vertexWhatDeleted[k] && !vertexWhatDeleted[graph[k][j]] && !findEdge(graph[k][j], k)) {
					cout << 1;
				}
			}
		}
	}

	// ����� �� ��������� ����� ������� ��� �������, ��� ���� �����. ��� ����� ������������ ������� ���� � ���������, ��� ������ ������� ������� ��� ���� �������� ������
	// ����� ����� ������ ������� ��� ����� �� ������ �����. sort(vertexToDelete.begin(), vertexToDelete.end());
	vector<it> shiftVector;
	vector<vector<it>> newGraph;
	vector<pair<it, it>> newExitPoints;
	vector<bool> newTowns;
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
	// �������� ����������� �� O(e+n)
	this->towns = newTowns;
	this->graph = newGraph;
	this->exitPoints = newExitPoints;
	return;
	//�� �����
	//for (int i = 0; i < towns.size(); i++) {
	//	// ���� ����� �� ������� �� � ���
	//	if (graph[towns[i]].empty()) {
	//		ft minDistance = INT16_MAX;
	//		//������� ������������ �������
	//		for (int j = 0; j < exitPoints.size(); j++) {
	//			ft nowDistance = distance(exitPoints[towns[i]], exitPoints[j]);
	//			if (nowDistance < minDistance && towns[i] != j) {

	//			}
	//		}
	//	}
	//}
}