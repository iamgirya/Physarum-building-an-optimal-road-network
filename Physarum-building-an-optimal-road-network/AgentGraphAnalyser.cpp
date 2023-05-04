#include "SlimeMoldClass.h"

AgentGraphAnalyser::AgentGraphAnalyser() {}

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

	//односв€зный список рЄбер. „тобы удал€ть рЄбра за 1
	struct EdgeListNode {
		it vextexOne;
		it vextexTwo;
		EdgeListNode* next = NULL;
		EdgeListNode* prev = NULL;
	};
	EdgeListNode* begin = new EdgeListNode();
	EdgeListNode* last = begin;
	int count = 0;

	for (int i = 0; i < positions.size(); i++) {
		for (int j = i+1; j < positions.size(); j++) {
			//благодар€ сортировке из n^2 переходим в n*log(n). ј в случае с нашими графами ускорение практически в 15 раз
			if (abs(positions[i].first.first - positions[j].first.first) > edgesRange) {
				break;
			}
			if (distance(positions[i].first, positions[j].first) <= edgesRange) {
				EdgeListNode* newNode = new EdgeListNode();
				newNode->vextexOne = i;
				newNode->vextexTwo = j;
				newNode->prev = last;
				last->next = newNode;
				last = newNode;
				count++;
			}
		}


	}
	begin = begin->next;

	vector<it> teams(positions.size());
	for (int i = 0; i < positions.size(); i++) {
		teams[i] = i;
	}

	//фиктивный конец и начало
	last->next = new EdgeListNode();
	begin->prev = new EdgeListNode();
	bool hasChange = true;
	while (hasChange) {
		hasChange = false;
		last = begin;
		while (last->next != NULL) {
			EdgeListNode* next = last->next;
			bool isNeedDelete = false;
			if (teams[last->vextexOne] == teams[last->vextexTwo]) {
				isNeedDelete = true;
			} else if (distance(positions[teams[last->vextexOne]].first, positions[teams[last->vextexTwo]].first) <= vertexRange) {
				//если втора€ точка - город, то измен€ем первую под неЄ
				if (positions[teams[last->vextexTwo]].second) {
					positions[teams[last->vextexOne]].first = positions[teams[last->vextexTwo]].first;
				}
				//если перва€ точка - не город, то делаем новую точку - среднее между двум€ точками
				else if (!positions[teams[last->vextexOne]].second) {
					positions[teams[last->vextexOne]].first = average(positions[teams[last->vextexOne]].first, positions[teams[last->vextexTwo]].first);
				}
				teams[last->vextexTwo] = teams[last->vextexOne];
				isNeedDelete = true;
				hasChange = true;
			}
			if (isNeedDelete) {
				last->prev->next = last->next;
				last->next->prev = last->prev;
				if (last == begin) {
					begin = begin->next;
				}
				delete last;
				count--;
			}
			last = next;
		}
	}
	begin->prev = NULL;

	set<it> tmpExitVertex;
	vector<it> exitVertex;
	vector<it> expexitVertex;
	for (int i = 0; i < positions.size(); i++) {
		tmpExitVertex.insert(teams[i]);
	}

	for (auto u: tmpExitVertex) {
		exitVertex.push_back(u);
	}
	sort(exitVertex.begin(), exitVertex.end());

	vector<pair<it, it>> exitPoints;
	for (int i = 0; i < exitVertex.size(); i++) {
		exitPoints.push_back(positions[exitVertex[i]].first);
	}

	vector<vector<it>> exitGraph(exitPoints.size(), vector<it>());
	last = begin;
	while (last->next != NULL) {
		int i = lower_bound(exitVertex.begin(), exitVertex.end(), teams[last->vextexOne]) - exitVertex.begin();
		int j = lower_bound(exitVertex.begin(), exitVertex.end(), teams[last->vextexTwo]) - exitVertex.begin();
		exitGraph[i].push_back(j);
		exitGraph[j].push_back(i);
		last = last->next;
	}

	this->graph = exitGraph;
	return exitPoints;
}