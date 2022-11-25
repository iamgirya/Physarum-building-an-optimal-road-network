#include <SlimeMoldClass.h>

SlimeAgentsSettings::SlimeAgentsSettings() {}

SlimeAgentsSettings::SlimeAgentsSettings(it sp, ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd) {
	setUp(sp, sod, sw, sa, ra, ss, dps, corcd);
}

void SlimeAgentsSettings::setUp(it sp, ft sod, it sw, ft sa, ft ra, ft ss, ft dps, ft corcd = 0) {
	ra = it(ra) % 360;
	//матрицы нужные для поворота мув вектора и сенсоров
	ra = ra * PI / 180;
	rightRotationMatrix = { {cos(ra), -sin(ra)}, { sin(ra), cos(ra)} };
	leftRotationMatrix = { {cos(-ra), -sin(-ra)}, { sin(-ra), cos(-ra)} };

	sa = it(sa) % 360;
	sensorAngle = sa * PI / 180;
	startPopulation = sp;
	sensorOffsetDistance = sod;
	sensorWidth = 1;
	stepSize = ss;
	depositPerStep = dps;
	chanceOfRandomChangeDirection = corcd;

	srand(it(time(NULL)));
}

SlimeAgent* SlimeAgentsSettings::generateAgent(vector<ft> startPosition, ft startAngle) {
	startAngle = startAngle / 180 * PI;
	vector<ft> moveVector = { stepSize * cos(startAngle), stepSize * sin(startAngle) };
	vector<ft> lsVector = { sensorOffsetDistance * cos(startAngle + sensorAngle), sensorOffsetDistance * sin(startAngle + sensorAngle) };
	vector<ft> csVector = { sensorOffsetDistance * cos(startAngle), sensorOffsetDistance * sin(startAngle) };
	vector<ft> rsVector = { sensorOffsetDistance * cos(startAngle - sensorAngle), sensorOffsetDistance * sin(startAngle - sensorAngle) };
	SlimeAgent* tmp = new SlimeAgent();
	tmp->setUp(startPosition, moveVector, lsVector, csVector, rsVector);
	return tmp;
}

vector<SlimeAgent*> SlimeAgentsSettings::generatePopulationInPixel(vector<ft>& startPosition) {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < startPopulation; i++) {
		rezult.push_back(generateAgent(startPosition, (ft(rand() % 360))));
	}
	return rezult;
}

vector<SlimeAgent*> SlimeAgentsSettings::generatePopulationRandomPositions(vector<it> sizes) {
	vector<SlimeAgent*> rezult;

	for (int i = 0; i < startPopulation; i++) {
		rezult.push_back(generateAgent({ ft(rand() % sizes[0]) , ft(rand() % sizes[1]) }, (ft(rand() % 360))));
	}
	return rezult;
}


Location::Location(){}

Location::Location(it x, it y, it dif, ft dec, bool ipb, bool icma) {
	xSize = x;
	ySize = y;
	diffusionSize = dif;
	decayFactor = dec;
	isPeriodicBoundary = ipb;
	isCanMultiAgent = icma;

	trailMap = vector<vector<ft>>(xSize, vector<ft>(ySize, 0.0));
	if (!isCanMultiAgent) {
		agentMap = vector<vector<bool>>(xSize, vector<bool>(ySize, false));
	}
}

void Location::castDecay() {
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			trailMap[i][j] *= decayFactor;
		}
	}
}

void Location::castDiffusion() {
	vector<vector<ft>> newMap = vector<vector<ft>>(xSize, vector<ft>(ySize, 0.0));
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			if (trailMap[i][j]) {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						if (checkMatrix(i + k, j + l)) {
							newMap[i + k][j + l] += 0.1 * trailMap[i][j];
						}
					}
				}
				newMap[i][j] += 0.1 * trailMap[i][j];
			}
		}
	}
	trailMap = newMap;
}

bool Location::canMakeMove(vector <it>& xy, vector <it>& oldxy) {
	if (!isCanMultiAgent) {
		if (!agentMap[xy[0]][xy[1]]) {
			agentMap[oldxy[0]][oldxy[1]] = false;
			agentMap[xy[0]][xy[1]] = true;
			return true;
		}
		return false;
	}

	return true;
}

vector<it> Location::getPixelOnCoord(vector <ft>& xy) {
	vector<it> rezult = { it(xy[0]), it(xy[1]) };
	if (isPeriodicBoundary) {
		if (rezult[0] < 0) {
			rezult[0] += xSize;
			xy[0] += xSize;
		}
		else if (rezult[0] >= xSize) {
			rezult[0] -= xSize;
			xy[0] -= xSize;
		}
		if (rezult[1] < 0) {
			rezult[1] += ySize;
			xy[1] += ySize;
		}
		else if (rezult[1] >= ySize) {
			rezult[1] -= ySize;
			xy[1] -= ySize;
		}
		return rezult;
	}
	else {
		if (rezult[0] < 0 || rezult[0] >= xSize || rezult[1] < 0 || rezult[1] >= ySize) {
			return vector<it>();
		}
		else {
			return rezult;
		}
	}
}

bool Location::checkMatrix(it i, it j) {
	return (i >= 0 && j >= 0 && i < xSize&& j < ySize);
}

vector<it> Location::getSizes() {
	return { xSize, ySize };
}



void SlimeAgent::setUp(vector<ft> pv, vector<ft> mv, vector<ft> lsv, vector<ft> csv, vector<ft> rsv) {
	positionVector = pv;
	moveVector = mv;
	leftSensorVector = lsv;
	centerSensorVector = csv;
	rigthSensorVector = rsv;
	pixelVector = { it(pv[0]), it(pv[1]) };
}
//движение
void SlimeAgent::moveTurn(SlimeAgentsSettings& set, Location& loc) {
	// если движение успешно
	if (move(loc)) {
		makeDeposit(set, loc);
	}
	else {
		rotate(set, rand() % 2);
	}
}
//сканирование
void SlimeAgent::skanTurn(SlimeAgentsSettings& set, Location& loc) {
	//шанс рандомного поворота
	if (set.chanceOfRandomChangeDirection > (rand() % 100 + 1)) {
		rotate(set, rand() % 2);
	}
	else {
		// -1 - поворот налево, 0 - сохранение направления, 1 - поворот направо
		it rotateSide = activateSensors(loc);
		if (rotateSide != 0) {
			rotate(set, rotateSide == 1);
		}
	}
}
// функция хода
void SlimeAgent::makeFullTurn(SlimeAgentsSettings& set, Location& loc) {
	moveTurn(set, loc);
	skanTurn(set, loc);
}

template<typename T>
vector<T> SlimeAgent::vSum(vector<T>& x, vector<T>& y) {
	vector<T> rezult(x.size());
	for (int i = 0; i < x.size(); i++) {
		rezult[i] = x[i] + y[i];
	}
	return rezult;
}

template<typename T>
vector<T> SlimeAgent::vMult(vector<T>& x, vector<vector<T>>& r) {
	vector<T> rezult(x.size(), 0);
	for (int i = 0; i < x.size(); i++) {
		for (int j = 0; j < x.size(); j++) {
			rezult[i] += x[j] * r[j][i];
		}
	}
	return rezult;
}
// определяет направление поворота
it SlimeAgent::activateSensors(Location& loc) {
	it lw, cw, rw;
	vector<it> pixelSensorPosition;
	vector<ft> tmp;

	tmp = vSum(positionVector, leftSensorVector);
	pixelSensorPosition = loc.getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		lw = loc.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
	}
	else {
		lw = 0;
	}

	tmp = vSum(positionVector, centerSensorVector);
	pixelSensorPosition = loc.getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		cw = loc.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
	}
	else {
		cw = 0;
	}

	tmp = vSum(positionVector, rigthSensorVector);
	pixelSensorPosition = loc.getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		rw = loc.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
	}
	else {
		rw = 0;
	}

	if ((cw >= rw && cw > lw) || (cw > rw && cw >= lw)) {
		return 0;
	}
	else if (rw > cw && rw > lw) {
		return 1;
	}
	else if (lw > cw && lw > rw) {
		return -1;
	}
	else if (cw == lw && cw == rw) {
		return (rand() % 3) - 1;
	}
}
// поворот векторов скорости и сенсоров
void SlimeAgent::rotate(SlimeAgentsSettings& set, bool isRigth) {
	if (isRigth) {
		moveVector = vMult(moveVector, set.rightRotationMatrix);
		leftSensorVector = vMult(leftSensorVector, set.rightRotationMatrix);
		centerSensorVector = vMult(centerSensorVector, set.rightRotationMatrix);
		rigthSensorVector = vMult(rigthSensorVector, set.rightRotationMatrix);
	}
	else {
		moveVector = vMult(moveVector, set.leftRotationMatrix);
		leftSensorVector = vMult(leftSensorVector, set.leftRotationMatrix);
		centerSensorVector = vMult(centerSensorVector, set.leftRotationMatrix);
		rigthSensorVector = vMult(rigthSensorVector, set.leftRotationMatrix);
	}
}
//сдвиг и установка пикселя, если это возможно
bool SlimeAgent::move(Location& loc) {
	vector<ft> newPosition = vSum(positionVector, moveVector);
	vector<it> newPixel = loc.getPixelOnCoord(newPosition);
	if (newPixel.empty() || !loc.canMakeMove(newPixel, pixelVector)) {
		return false;
	}
	else {
		positionVector = newPosition;
		pixelVector = newPixel;
		return true;
	}
}
// нанесение следа на карту
void SlimeAgent::makeDeposit(SlimeAgentsSettings& set, Location& loc) {
	loc.trailMap[pixelVector[0]][pixelVector[1]] += set.depositPerStep;
}



SlimeMoldSimulation::SlimeMoldSimulation(Location& l, SlimeAgentsSettings& s) {
	location = l;
	settings = s;
}

void SlimeMoldSimulation::startSimulation(vector<ft> startPosition) {
	particles = settings.generatePopulationRandomPositions(location.getSizes());

	int count = 0;
	while (true) {
		// что-то с rotate неправильно
		makeStep();
		random_shuffle(particles.begin(), particles.end());
		if (count %30 == 0)
			outputInFileAgentMap();
		count++;
	}
}

void SlimeMoldSimulation::makeStep() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->moveTurn(settings, location);
	}
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->skanTurn(settings, location);
	}

	location.castDiffusion();
	location.castDecay();
}

void SlimeMoldSimulation::outputInConsoleTrayMap() {
	for (int i = 0; i < location.trailMap.size(); i++) {
		for (int j = 0; j < location.trailMap[0].size(); j++) {
			cout << " "[location.trailMap[i][j] < 10] << it(location.trailMap[i][j]) << ' ';
		}
		cout << endl;
	}
}

void SlimeMoldSimulation::outputInConsoleAgentMap() {
	vector<vector<bool>> tmp = vector<vector<bool>>(location.trailMap.size(), vector<bool>(location.trailMap[0].size(), false));
	for (int i = 0; i < particles.size(); i++) {
		tmp[particles[i]->pixelVector[0]][particles[i]->pixelVector[1]] = true;
	}

	for (int i = 0; i < location.trailMap.size(); i++) {
		for (int j = 0; j < location.trailMap[0].size(); j++) {
			if (tmp[i][j]) {
				cout << "1" << " ";
			}
			else {
				cout << "." << " ";
			}

		}
		cout << endl;
	}

	cout << endl;
}

void SlimeMoldSimulation::outputInFileAgentMap() {
	static int ii = 0;
	ii++;
	ii %= 1000;
	ofstream output("out" + to_string(ii)+ ".txt");

	vector<vector<bool>> tmp = vector<vector<bool>>(location.trailMap.size(), vector<bool>(location.trailMap[0].size(), false));
	for (int i = 0; i < particles.size(); i++) {
		tmp[particles[i]->pixelVector[0]][particles[i]->pixelVector[1]] = true;
	}

	for (int i = 0; i < location.trailMap.size(); i++) {
		for (int j = 0; j < location.trailMap[0].size(); j++) {
			if (tmp[i][j]) {
				output << "1" << " ";
			}
			else {
				output << "." << " ";
			}

		}
		output << endl;
	}

	output.close();
}