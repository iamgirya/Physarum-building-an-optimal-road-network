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
	// находим вектора рёбер
	pair<it, it> main = exitPoints[i];
	pair<ft, ft> firstVector = make_pair(main.first - exitPoints[firstIndex].first, main.second - exitPoints[firstIndex].second);
	pair<ft, ft> secondVector = make_pair(exitPoints[secondIndex].first - main.first, exitPoints[secondIndex].second - main.second);
	// нормируем
	ft tmpLength = length(firstVector);
	firstVector = make_pair(firstVector.first / tmpLength, firstVector.second / tmpLength);
	tmpLength = length(secondVector);
	secondVector = make_pair(secondVector.first / tmpLength, secondVector.second / tmpLength);
	// пример: угл между двух рёбер не превышает 10 градусов в случае, если длина разници нормированных векторов не больше 0.1737. Такие рёбра соединяем
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
			// если есть такие две вершины, что соединены ребром
			if (findEdge(first, second)) {
				for (int third: graph[first]) {
					// и у этих двух вершин есть смежная с ними вершина
					if (third != index && findEdge(second, third)) {
						// то это ромб с вершинами index, i, j, k
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

//преобразует множество точек в нормальный граф
void AgentGraphAnalyser::makeGraph(vector<SlimeAgent*> particles, vector<Generator*> generators) {
	// добавляем все точки и генераторы в однин вектор, различая их с помощью второго параметра
	// TODO можно избавиться от такой вложенности пар с помощью ещё одного вектора
	vector<pair<pair<it,it>, it>> position;
	for (auto u : particles) {
		position.push_back(make_pair(make_pair(u->pixelVector[0], u->pixelVector[1]), 0));
	}
	for (auto u : generators) {
		position.push_back(make_pair(make_pair(u->position[0], u->position[1]), u->agentsPerStep));
	}
	sort(position.begin(), position.end());

	// делаем из вектора односвязный список. В i-том элементе находится индекс вектора следующего активного элемента
	vector<it> next(position.size(), -1);
	for (int i = 0; i < position.size()-1; i++) {
		next[i] = i + 1;
	}
	// количество точек, которые забрала данная точка
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
				//благодаря сортировке из n^2 переходим в n*log(n) в случае с нашими графами
				if (abs(firstPoint.first - secondPoint.first) > vertexRange) {
					break;
				}
				// если точки достаточно близко, стираем точку j
				if (distance(firstPoint, secondPoint) <= vertexRange) {
					//если вторая точка - город, то мы не можем её убрать. Чтобы в итоге получился связный граф, мы просто останавливаем обработку этой точки. Проблема лишь в бОльшем количестве точек, которая решится в некст функции
					if (position[j].second) {
						// в случае, если обе точки стоят в одном месте, то их стоит поменять местами, после чего слить воедино.
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

	// дебаг count
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

// Решает 4 проблемы артефактов:
// одинокие - вершины с 1 ребром или без них;
// ломаные - линия, разбитая на несколько ребёр, что нужно соединить в одно;
// треугольники - вершина, что соединена только с двумя вершинами, что уже соединены;
// ромбы - четыре вершины, которые вместе образуют четырёх угольник.
void AgentGraphAnalyser::minimizeGraph() {
	// если нужно что-то удалить, то запоминаем номер
	vector<it> vertexToDelete;
	// TODO можно переписать на односвязный список, чтобы меньше по памяти было
	vector<bool> vertexWhatDeleted(exitPoints.size(), false);
	// вначале чекаем все вершины, потом те вершины, что были затронуты удалением, потом те вершины, что были затронуты ещё одним удалением...
	queue<it> vertexToCheck;
	for (int i = 0; i < exitPoints.size(); i++) {
		vertexToCheck.push(i);
	}

	while (!vertexToCheck.empty()) {
		int i = vertexToCheck.front(); vertexToCheck.pop();
		if (!vertexWhatDeleted[i]) {
			if (!towns[i]) {
				// одинокая вершина
				if (graph[i].size() <= 1) {
					vertexToDelete.push_back(i);
					vertexWhatDeleted[i] = true;
					if (graph[i].size() == 1) {
						eraseEdge(graph[i][0], i);
						vertexToCheck.push(graph[i][0]);
					}
				}
				// треугольник
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
				// ломаная
				else if (graph[i].size() == 2 && canConnectEdges(i)) { 
					it firstIndex = graph[i][0];
					it secondIndex = graph[i][1];
					// соединяем
					vertexToDelete.push_back(i);
					vertexWhatDeleted[i] = true;
					// удаляем рёбра с i-й вершиной
					eraseEdge(firstIndex, i);
					eraseEdge(secondIndex, i);
					// если вершины не соединены, соединяем
					if (!findEdge(firstIndex, secondIndex)) {
						graph[firstIndex].push_back(secondIndex);
						graph[secondIndex].push_back(firstIndex);
					}
					// проверим эти вершины
					vertexToCheck.push(firstIndex);
					vertexToCheck.push(secondIndex);
				}
			} 
			// ромб, Причём i-я вершина может быть городом
			{
				
				vector<it> rombVertex = checkRomb(i);
				if (!rombVertex.empty()) {
					// если обе диалогнали есть
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
							// если находим хотя бы один город - фиксируем положение вершины
							bool hasTown = towns[i];
							// делаем смежную вершину из четырёх
							for (int k: rombVertex) {
								// образуем вершину по середине или с центром в городе
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
									// соединяем первую вершину со смежными вершинами
									if (i != v && !findEdge(i, v)) {
										graph[i].push_back(v);
										graph[v].push_back(i);
									}
									// стираем рёбра к k-вершине и проверяем все смежные вершины.
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
						// делаем смежную вершину из двух, соединённых диагональю, изменяя первую и удаляя вторую
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
						// если они не города, то образуем вершину по середине
						if (!towns[firstIndex] && !towns[secondIndex]) {
							exitPoints[firstIndex] = average(exitPoints[firstIndex], exitPoints[secondIndex]);
						}
						// для каждой смежной вершины: соединяем её с первой вершиной, удаляем рёбра со второй вершиной и проверяем их в последствии
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


	// нужно из исходного графа удалить все вершины, что были слиты. Для этого пересобираем меньший граф и сохраняем, для какого индекса сколько уже было удалённых вершин
	// чтобы после просто вычесть это числа из номера ребра. sort(vertexToDelete.begin(), vertexToDelete.end());
	// удаление выполняется за O(e+n)
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
		// запоминаем сдвиг
		shiftVector.push_back(lastDeleteIndex);
	}
	// Пример: было ребро 1-3 и 3-1. Удалили вершину 2. Теперь это будет ребро 1-2 2-1. Ребро 2-1 само становится на место, так как мы пропустили прошлую вершину 2 в новом графе.
	// Ребро 1-2 мы должны сделать из ребра 1-3 тем, что из "3" вычтем кол-во удалённых до неё вершин - т.е. 1
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

// строит граф кратчайших путей для каждого из townIndexes. Для каждого i и j хранится кратчайший между ними путь. Для самой себя или до недостижимой вершины он хранит пустой путь
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

		// минимальное расстояние. Массив и очередь вместе нужны для оптимизации алгоритма. В очередь записываем отрицательные значения
		vector<ft> d(SIZE, MAXIT); d[begin_index] = 0;
		priority_queue<pair<ft, it>> q; q.push(make_pair(-0, begin_index));
		// посещенные вершины
		vector<it> visited(SIZE, 0);
		// число - это индекс вершины, откуда идёт путь. Для начальной вершины число равно -1
		vector<it> prevVertex(SIZE, -1);

		it minIndex, min;
		pair<ft, it> tmp;
		// TODO оптимизировать как-то с использованием городов
		// Шаг алгоритма
		do {
			// берём вершину, к которой меньше всего топать и которую мы ещё не смотрели
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
			// для всех смежных
			for (int i : graph[minIndex])
			{

				if (!visited[i] && weigthGraph[minIndex][i] > 0) // если -1, то ребра нет
				{
					// если из этой вершины меньше топать в смежную, то перестраиваем путь
					ft tmp = min + weigthGraph[minIndex][i];
					if (tmp < d[i])
					{
						d[i] = tmp;
						q.push(make_pair(-d[i], i));

						prevVertex[i] = minIndex;
					}
				}
			}
			// притопали в вершину
			visited[minIndex] = 1;
		} while (true); // выход через очередь
	endAlgo:

		for (int i : townIndexes) {

			// путь начинается в самой вершине
			vector<it> tmp = vector<it>(); tmp.push_back(i);
			int nowIndex = prevVertex[i];

			// если пути нет, то предыдущая вершина -1
			if (nowIndex == -1) {
				way.push_back(vector<it>());
				continue;
			}
			while (nowIndex != -1) {
				// если это была не последняя вершина, то добавляем индекс предыдущей
				tmp.push_back(nowIndex);
				nowIndex = prevVertex[nowIndex];
			}
			way.push_back(tmp);
		}

		ways.push_back(way);
	}
	this->waysGraph = ways;
}

// Для каждой пары генераторов находим кратчайший маршрут между ними
// Пересоздаём таблицу смежности, но с параметром потока.
// Для каждого маршрута проходимся по нему и добавляем поток между данными генераторами
void AgentGraphAnalyser::buildFlow() {
	this->flowGraph.clear();
	if (waysGraph.empty()) {
		buildWays();
	}

	// дополнение к weigthGraph, чтобы не делать граф с парами
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
		// проходимся лишь по одной копии пути
		for (int j = i+1; j < waysGraph[i].size(); j++) {
			if (waysGraph[i][j].empty()) {
				continue;
			}

			it first;
			it second = waysGraph[i][j][0];
			
			ft waysFlow = ft(towns[townIndexes[i]] * towns[townIndexes[j]]) / (sumOfPriority - towns[townIndexes[i]]);// поток из i в j
			waysFlow += towns[townIndexes[j]] * towns[townIndexes[i]] / (sumOfPriority - towns[townIndexes[j]]);// поток обратно
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

// Создаём матрицу с весом рёбер, где вес - расстояние между вершинами
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

// Вычисляет значение во сколько раз длина дорог превышает минимально возможную
ft AgentGraphAnalyser::calculateWeigth() {
	if (weigthGraph.empty()) {
		buildWeigth();
	}

	ft minWeigth = 0;
	// первое число - длина ребра, второе - вершина, куда идёт ребро
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
		// фу так делать через for
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

// вычисляет во сколько раз в среднем пути стали длиннее, в сравнении с полным графом
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
		// проходимся лишь по одной копии пути
		for (int j = i + 1; j < waysGraph[i].size(); j++) {
			ft minDistance = distance(exitPoints[townIndexes[i]], exitPoints[townIndexes[j]]);
			ft nowDistance = 0;

			ft waysFlow = ft(towns[townIndexes[i]] * towns[townIndexes[j]]) / (sumOfPriority - towns[townIndexes[i]]);// поток из i в j
			waysFlow += towns[townIndexes[j]] * towns[townIndexes[i]] / (sumOfPriority - towns[townIndexes[j]]);// поток обратно

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

// находит количество мостов и максимально длинный цикл - самые стрессоопасные элементы графа
ft AgentGraphAnalyser::calculateResistance() {
	it bridgeCount = 0;
	it maxCycle = 0;
	vector<int> stressed(graph.size(), 0);
	
	ft sumOfPriority = 0;
	for (int i = 0; i < towns.size(); i++) {
		sumOfPriority += towns[i];
	}

	// выбираем все возможные рёбра. Удаляем это ребро и пытается найти путь из i в j. Если путь можно найти, то получается, что мы имеем цикл, иначе мост
	for (int i = 0; i < graph.size(); i++) {
		for (it j : graph[i]) {
			if (i >= j) {
				continue;
			}
			ft nowSum = sumOfPriority - towns[i];
			vector<int> visited(graph.size(), 0); visited[i] = 1;
			queue<pair<it, it>> vertex;
			// заранее проходимся по всем рёбрам из i, чтобы исключить условие на проверку ребра i j
			for (it k : graph[i]) {
				if (k != j) {
					vertex.push(make_pair(k, 1));
					visited[k] = 1;
					nowSum -= towns[k];
				}
			}
			
			// длину можно вычислить без полного перебора с помощью жадности прохода в ширину
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
			// если смогли прийти в j вершину, значит есть цикл
			if (visited[j]) {
				maxCycle = max(maxCycle, size + 1);
			}
			// иначе граф уже не связный и значит i j - мост
			else {
				// если мы прошли меньшую часть из компонент связностей, то все посещённые вершины считаем в зоне риска, иначе все непосещённые
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

	// добавляем +1 потому, что все метрики больше 1
	return 1+ sumOfStressedPriority/sumOfPriority;
}

// т.к. нам интересует только то, чобы сеть _могла_ работать ещё долгое время без дополнительных путей, то среди потока нас интересует лишь
// одно число - максимальный поток на одном из рёбер. Ибо в случае роста потоков, скорее всего, это ребро будет главной проблемой
// Так что мы вычисляем максимум потока для ребра полного графа и этим числов делим максимум потока нынешнего графа.
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

// Суммируем весь поток и делим на количество рёбер.
// Проходимся по каждому ребру и вычисляем квадрат разницы его потока и значения выше, суммируем и находим корень
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