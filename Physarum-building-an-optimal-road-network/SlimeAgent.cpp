#include "SlimeMoldClass.h"

SlimeAgent::SlimeAgent() {}

void SlimeAgent::setUp(ft startAngle, it teamIndex, it timeToLife, vector<ft> positionVector, vector<ft> moveVector, vector<ft> leftSensorVector, vector<ft> centerSensorVector, vector<ft> rigthSensorVector, SlimeMoldSimulation* settings) {
	this->positionVector = positionVector;
	this->moveVector = moveVector;
	this->leftSensorVector = leftSensorVector;
	this->centerSensorVector = centerSensorVector;
	this->rigthSensorVector = rigthSensorVector;
	this->pixelVector = { it(positionVector[0]), it(positionVector[1]) };
	this->settings = settings;
	this->timeToLife = timeToLife;
	this->teamIndex = teamIndex;
	this->startAngle = make_pair(startAngle, moveVector);
}
//движение
void SlimeAgent::moveTurn() {
	// если движение успешно
	if (move()) {
		makeDeposit();
	}
	else {
		rotate(rand() % 2);
	}
}

//сканирование
void SlimeAgent::skanTurn() {
	// -1 - поворот налево, 0 - сохранение направления, 1 - поворот направо
	it rotateSide = activateSensors();
	if (rotateSide != 0) {
		rotate(rotateSide == 1);
	}
}
// функция хода
void SlimeAgent::makeFullTurn() {
	moveTurn();
	skanTurn();
}

short int SlimeAgent::isTimeToDeath() {
	timeToLife--;
	it generatorIndex = settings->location.checkNearGenerator(pixelVector, teamIndex);
	if (generatorIndex != -1) {
		settings->location.onAgentsDeath(pathVector, addOnGenerator, generatorIndex, startAngle.first);
		return 1;
	}
	if (timeToLife <= 0) {
		settings->location.onAgentsDeath(pathVector, addOnDeath, generatorIndex, startAngle.first);
		return 1;
	}
	
	return 0;
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
		//TODO: в статическую переменную это
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
	if (newPixel.empty()) {
		timeToLife = 0;
		return false;
	}
	if (!settings->location.canMakeMove(newPixel, pixelVector)) {
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
	//TODO сделать статикой
	settings->location.trailMap[pixelVector[0]][pixelVector[1]] += settings->depositPerStep;
}