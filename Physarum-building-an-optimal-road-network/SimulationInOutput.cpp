#include "SlimeMoldClass.h"

//input

bool SlimeMoldSimulation::updateSettingsFromFile() {
	ifstream input("settings.txt");

	string isNeedToUpdate;
	it ttl; ft sod; it sw; ft sa; ft ra; ft ss; ft dps; ft corcd; it dif; ft dec; bool ipb; bool icma;
	it newPopulation; it countOfGenerators;
	try {
		input >> isNeedToUpdate;
		if (isNeedToUpdate == "yes" || stepSize == 0) {
			input >> ttl >> sod >> sw >> sa >> ra >> ss >> dps >> corcd >> dif >> dec >> ipb >> icma;
			input >> newPopulation >> countOfGenerators;

			generators.clear();
			generatorsQueue.clear();
			for (int i = 0; i < countOfGenerators; i++) {
				vector<ft> pos(2);
				it rate;
				input >> pos[0] >> pos[1] >> rate;

				generators.push_back(make_pair(pos, rate));
				generatorsQueue.push_back(queue<ft>());
			}

			bool isNeedToUpdateAgents = population != 0 && (sensorOffsetDistance != sod || sensorAngle != sa || stepSize != ss);
			bool isNeedToResizePopulation = population != newPopulation;

			setUp(ttl, newPopulation, sod, 1, sa, ra, ss, dps, corcd, dif, dec, ipb, icma);

			if (isNeedToResizePopulation) {
				if (particles.size() < newPopulation) {
					auto newPart = generatePopulationRandomPositions(newPopulation - particles.size(), location.getSizes());
					for (auto u : newPart) {
						particles.push_back(u);
					}
				}
				else {
					particles.resize(newPopulation);
				}
			}

			if (isNeedToUpdateAgents) {
				for (it i = 0; i < particles.size(); i++) {
					ft partAngle = atan2(particles[i]->positionVector[0], particles[i]->positionVector[1]) / PI * 180;
					particles[i] = generateAgent(particles[i]->positionVector, partAngle, -1);
				}
			}

			input.close();

			ofstream output("settings.txt");
			output << "no" << endl;
			output << ttl << ' ' << sod << ' ' << sw << ' ' << sa << ' ' << ra << ' ' << ss << ' ' << dps << ' ' << corcd << ' ' << dif << ' ' << dec << ' ' << ipb << ' ' << icma << endl;
			output << newPopulation << ' ' << countOfGenerators;
			for (int i = 0; i < countOfGenerators; i++) {
				output << endl << generators[i].first[0] << ' ' << generators[i].first[1] << ' ' << generators[i].second;
			}
			output.close();
			return true;
		}
		else {
			input.close();
			return false;
		}
	}
	catch (exception) {
		input.close();
		return false;
	}
}

//output

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
	ofstream output("out" + to_string(ii) + ".txt");

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

void SlimeMoldSimulation::outputInBmp(bool isChangedSettings = false) {
	static int bmpi = -1;
	static vector<ft> colorVector = { (0.3 + rand() % 50 / 100.0), (0.3 + rand() % 50 / 100.0) , (0.3 + rand() % 50 / 100.0) };
	bmpi++;
	bmpi %= 10000;
	int w = location.getSizes()[0], h = location.getSizes()[1];

	FILE* f;
	unsigned char* img = NULL;
	int filesize = 54 + 3 * w * h;

	img = (unsigned char*)malloc(3 * w * h);
	memset(img, 0, 3 * w * h);

	int r, g, b;
	/*
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{

			int x = i; int y = (h - 1) - j;
			r = location.trailMap[i][j];
			g = 0;
			b = 0;
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			img[(x + y * w) * 3 + 2] = (unsigned char)(r);
			img[(x + y * w) * 3 + 1] = (unsigned char)(g);
			img[(x + y * w) * 3 + 0] = (unsigned char)(b);
		}
	}
	*/
	//for (int i = 0; i < 200; i++) {
	//	for (int j = 0; j < 200; j++) {
	//		if (true) {
	//			//img[(i + j * w) * 3 + 0] = 0;
	//			//img[(i + j * w) * 3 + 1] = 0;
	//			img[(i + j * w) * 3 + 2] = location.trailMap[i][j] * 10 > 255 ? 255 : location.trailMap[i][j] * 10;
	//		}
	//	}
	//}

	if (!isChangedSettings) {
		for (int colorOffset = 0; colorOffset < 3; colorOffset++) {
			for (int i = 0; i < particles.size(); i++) {
				if (img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + colorOffset] > (255 - 8) * colorVector[colorOffset]) {
					img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + colorOffset] += 8 * colorVector[colorOffset];
				}
				else {
					img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + colorOffset] = 255 * colorVector[colorOffset];
				}
			}
		}
	}
	else {
		for (int i = 0; i < particles.size(); i++) {
			if (img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + 2] > 255 - 8) {
				img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + 2] += 8;
			}
			else {
				img[(particles[i]->pixelVector[0] + particles[i]->pixelVector[1] * w) * 3 + 2] = 255;
			}
		}
	}

	for (int i = 0; i < generators.size(); i++) {
		img[(it(generators[i].first[0]) + it(generators[i].first[1]) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1]) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1]) * w) * 3 + 2] = 255;
		img[(it(generators[i].first[0] + 1) + it(generators[i].first[1]) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0] + 1) + it(generators[i].first[1]) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0] + 1) + it(generators[i].first[1]) * w) * 3 + 2] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1] + 1) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1] + 1) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1] + 1) * w) * 3 + 2] = 255;
		img[(it(generators[i].first[0] - 1) + it(generators[i].first[1]) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0] - 1) + it(generators[i].first[1]) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0] - 1) + it(generators[i].first[1]) * w) * 3 + 2] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1] - 1) * w) * 3 + 0] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1] - 1) * w) * 3 + 1] = 255;
		img[(it(generators[i].first[0]) + it(generators[i].first[1] - 1) * w) * 3 + 2] = 255;
	}




	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);


	string name = "img" + to_string(bmpi) + ".bmp";
	f = fopen(name.data(), "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	for (int i = 0; i < h; i++)
	{
		fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
	}

	free(img);
	fclose(f);
}