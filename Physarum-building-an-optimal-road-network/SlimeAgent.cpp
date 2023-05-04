#include "SlimeMoldClass.h"

SlimeAgent::SlimeAgent() {}

void SlimeAgent::setUp(ft sa, it ti, it ttl, vector<ft> pv, vector<ft> mv, vector<ft> lsv, vector<ft> csv, vector<ft> rsv, SlimeMoldSimulation* set) {
	positionVector = pv;
	moveVector = mv;
	leftSensorVector = lsv;
	centerSensorVector = csv;
	rigthSensorVector = rsv;
	pixelVector = { it(pv[0]), it(pv[1]) };
	settings = set;
	timeToLife = ttl;
	teamIndex = ti;
	startAngle = make_pair(sa, mv);
}
//движение и старение
void SlimeAgent::moveTurn() {
	// если движение успешно
	if (move()) {
		makeDeposit();
	}
	else {
		rotate(rand() % 2);
	}
}
//старение
it SlimeAgent::checkGeneratorsTurn() {
	const it range = 2;
	for (it i = 0; i < settings->generators.size(); i++) {
		if (i != teamIndex && abs(pixelVector[0] - it(settings->generators[i].first[0])) <= range && abs(pixelVector[1] - it(settings->generators[i].first[1])) <= range) {
			return i;
		}
	}
	return -1;
}

void SlimeAgent::deadTurn(it indexOfGenerator = -1) {
	const ft someAngle = 10;
	if (indexOfGenerator != -1) {
		ft newAngle = startAngle.first + (rand() % 2 == 0 ? -1 : 1) * someAngle;
		settings->generatorsQueue[teamIndex].push(newAngle);
		for (int i = 0; i < pathVector.size(); i++) {
			settings->location.trailMap[pathVector[i][0]][pathVector[i][1]] += settings->depositPerStep * 3;
		}
		return;
	}
	const it delta = 2;
	it i, j;
	for (i = -delta; i <= delta; i++) {
		for (j = -delta; j <= delta; j++) {
			// кринжово
			if (settings->location.checkMatrix(pixelVector[0] + i, pixelVector[1] + j)) {
				settings->location.trailMap[pixelVector[0] + i][pixelVector[1] + j] -= settings->depositPerStep * 5;
			}
		}
	}
}
//сканирование
void SlimeAgent::skanTurn() {
	//шанс рандомного поворота
	if (settings->chanceOfRandomChangeDirection != 0 && settings->chanceOfRandomChangeDirection > (rand() % 100 + 1)) {
		rotate(rand() % 2);
	}
	else {
		// -1 - поворот налево, 0 - сохранение направления, 1 - поворот направо
		it rotateSide = activateSensors();
		if (rotateSide != 0) {
			rotate(rotateSide == 1);
		}
	}
}
// функция хода
void SlimeAgent::makeFullTurn() {
	moveTurn();
	skanTurn();
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
it SlimeAgent::activateSensors() {
	it lw, cw, rw;
	vector<it> pixelSensorPosition;
	vector<ft> tmp;

	tmp = vSum(positionVector, leftSensorVector);
	pixelSensorPosition = settings->location.getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		lw = settings->location.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
	}
	else {
		lw = 0;
	}

	tmp = vSum(positionVector, centerSensorVector);
	pixelSensorPosition = settings->location.getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		cw = settings->location.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
	}
	else {
		cw = 0;
	}

	tmp = vSum(positionVector, rigthSensorVector);
	pixelSensorPosition = settings->location.getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		rw = settings->location.trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]];
	}
	else {
		rw = 0;
	}
	/*if (cw < 0 && rw < 0 && lw < 0) {
		moveVector = { -moveVector[0], -moveVector[1] };
		centerSensorVector = { -centerSensorVector[0], -centerSensorVector[1] };
		rigthSensorVector = { -rigthSensorVector[0], -rigthSensorVector[1] };
		leftSensorVector = { -leftSensorVector[0], -leftSensorVector[1] };
		return 0;
	}*/

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
		return 0;
	}
}
// поворот векторов скорости и сенсоров
void SlimeAgent::rotate(bool isRigth) {
	if (isRigth) {
		moveVector = vMult(moveVector, settings->rightRotationMatrix);
		leftSensorVector = vMult(leftSensorVector, settings->rightRotationMatrix);
		centerSensorVector = vMult(centerSensorVector, settings->rightRotationMatrix);
		rigthSensorVector = vMult(rigthSensorVector, settings->rightRotationMatrix);
	}
	else {
		moveVector = vMult(moveVector, settings->leftRotationMatrix);
		leftSensorVector = vMult(leftSensorVector, settings->leftRotationMatrix);
		centerSensorVector = vMult(centerSensorVector, settings->leftRotationMatrix);
		rigthSensorVector = vMult(rigthSensorVector, settings->leftRotationMatrix);
	}
}
//сдвиг и установка пикселя, если это возможно
bool SlimeAgent::move() {
	vector<ft> newPosition = vSum(positionVector, moveVector);
	vector<it> newPixel = settings->location.getPixelOnCoord(newPosition);

	if (newPixel.empty() || !settings->location.canMakeMove(newPixel, pixelVector)) {
		return false;
	}
	else {
		positionVector = newPosition;
		pixelVector = newPixel;
		pathVector.push_back(newPixel);
		return true;
	}
}
// нанесение следа на карту
void SlimeAgent::makeDeposit() {
	settings->location.trailMap[pixelVector[0]][pixelVector[1]] += settings->depositPerStep;
}