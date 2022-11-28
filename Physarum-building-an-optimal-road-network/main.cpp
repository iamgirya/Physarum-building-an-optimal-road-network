#include <SlimeMoldClass.h>

int main()
{
	//SlimeAgentsSettings sett = SlimeAgentsSettings(6000, 9, 1, 45, 22.5, 1, 5, 0);
	//Location loc = Location(200, 200, 3, 0.3, true, false);

	//ввод настроек теперь мерез файл settings в директории проекта
	SlimeMoldSimulation sim = SlimeMoldSimulation();
	sim.setLocation(200, 200);
	sim.startSimulation({ 45.5, 45.5 });

}
