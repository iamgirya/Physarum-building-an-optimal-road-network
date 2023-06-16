#include "SlimeMoldClass.h"

ft SlimeAgent::depositPerStep = 0;
vector<vector<ft>> SlimeAgent::leftRotationMatrix;
vector<vector<ft>> SlimeAgent::rightRotationMatrix;

void SlimeAgent::setUp(ft startAngle, it teamIndex, it timeToLife, vector<ft> positionVector, vector<ft> moveVector, vector<ft> leftSensorVector, vector<ft> centerSensorVector, vector<ft> rigthSensorVector, Location* location, bool isUndead) {
	this->positionVector = positionVector;
	this->moveVector = moveVector;
	this->leftSensorVector = leftSensorVector;
	this->centerSensorVector = centerSensorVector;
	this->rigthSensorVector = rigthSensorVector;
	this->pixelVector = { it(positionVector[0]), it(positionVector[1]) };
	this->location = location;
	this->timeToLife = timeToLife;
	this->teamIndex = teamIndex;
	this->startAngle = make_pair(startAngle, moveVector);
	this->isUndead = isUndead;
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
	if (isUndead) {
		return 0;
	}

	timeToLife--;
	it generatorIndex = location->checkNearGenerator(pixelVector, teamIndex);
	if (generatorIndex != -1) {
		location->onAgentsDeath(pathVector, addOnGenerator, generatorIndex, startAngle.first);
		return 1;
	}
	if (timeToLife <= 0) {
		location->onAgentsDeath(pathVector, addOnDeath, generatorIndex, startAngle.first);
		return 1;
	}
	
	return 0;
}

vector<ft> SlimeAgent::vSum(vector<ft>& x, vector<ft>& y) {
	vector<ft> rezult(x.size());
	for (int i = 0; i < x.size(); i++) {
		rezult[i] = x[i] + y[i];
	}
	return rezult;
}

vector<ft> SlimeAgent::vMult(vector<ft>& x, vector<vector<ft>>& r) {
	vector<ft> rezult(x.size(), 0);
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
	pixelSensorPosition = location->getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		lw = it(location->trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]]);
	}
	else {
		lw = 0;
	}

	tmp = vSum(positionVector, centerSensorVector);
	pixelSensorPosition = location->getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		cw = it(location->trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]]);
	}
	else {
		cw = 0;
	}

	tmp = vSum(positionVector, rigthSensorVector);
	pixelSensorPosition = location->getPixelOnCoord(tmp);
	if (!pixelSensorPosition.empty()) {
		rw = it(location->trailMap[pixelSensorPosition[0]][pixelSensorPosition[1]]);
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
		return 0;
	}
}
// поворот векторов скорости и сенсоров
void SlimeAgent::rotate(bool isRigth) {
	if (isRigth) {
		moveVector = vMult(moveVector, SlimeAgent::rightRotationMatrix);
		leftSensorVector = vMult(leftSensorVector, SlimeAgent::rightRotationMatrix);
		centerSensorVector = vMult(centerSensorVector, SlimeAgent::rightRotationMatrix);
		rigthSensorVector = vMult(rigthSensorVector, SlimeAgent::rightRotationMatrix);
	}
	else {
		moveVector = vMult(moveVector, SlimeAgent::leftRotationMatrix);
		leftSensorVector = vMult(leftSensorVector, SlimeAgent::leftRotationMatrix);
		centerSensorVector = vMult(centerSensorVector, SlimeAgent::leftRotationMatrix);
		rigthSensorVector = vMult(rigthSensorVector, SlimeAgent::leftRotationMatrix);
	}
}
//сдвиг и установка пикселя, если это возможно
bool SlimeAgent::move() {
	vector<ft> newPosition = vSum(positionVector, moveVector);
	vector<it> newPixel = location->getPixelOnCoord(newPosition);
	if (newPixel.empty()) {
		timeToLife = 0;
		return false;
	}
	if (!location->canMakeMove(newPixel, pixelVector)) {
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
	location->trailMap[pixelVector[0]][pixelVector[1]] += SlimeAgent::depositPerStep;
}