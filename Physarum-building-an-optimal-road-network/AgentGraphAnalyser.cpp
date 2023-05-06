#include "SlimeMoldClass.h"

vector<pair<it, it>> AgentGraphAnalyser::makeMinimizedGraph(vector<SlimeAgent*> particles, vector<Generator*> generators) {
	// ��������� ��� ����� � ���������� � ����� ������, �������� �� � ������� ������� ���������
	vector<pair<pair<it,it>, bool>> position;
	for (auto u : particles) {
		position.push_back(make_pair(make_pair(u->pixelVector[0], u->pixelVector[1]), false));
	}
	for (auto u : generators) {
		position.push_back(make_pair(make_pair(u->position[0], u->position[1]), true));
	}
	sort(position.begin(), position.end());

	//������ ������� ���������� ������� ����� � ���������
	auto distance = [](pair<it, it> i, pair<it, it> j) { return sqrt(pow(i.first - j.first, 2) + pow(i.second - j.second, 2)); };
	auto average = [](pair<it, it> i, pair<it, it> j) {return make_pair((i.first+j.first)/2, (i.second + j.second) / 2); };

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
	for (int i = 0; i != -1; i = next[i]) {
		if (pointMass[i] >= minVertexMass || position[i].second) {
			exitPoints.push_back(position[i].first);
		}
	}


	int count = 0;
	vector<vector<it>> exitGraph(exitPoints.size(), vector<it>());
	for (int i = 0; i < exitPoints.size(); i++) {
		for (int j = i + 1; j < exitPoints.size(); j++) {
			if (distance(exitPoints[i], exitPoints[j]) <= edgesRange) {
				exitGraph[i].push_back(j);
				count++;
			}
		}
	}

	this->graph = exitGraph;
	return exitPoints;
}