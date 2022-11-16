#include <SlimeMoldClass.h>

int main()
{
	SlimeAgentsSettings sett = SlimeAgentsSettings(70, 9, 1, 45, 45, 1, 5, 0);
	Location loc = Location(100, 100, 3, 0.1, true, true);

	SlimeMoldSimulation sim = SlimeMoldSimulation(loc, sett);
	sim.startSimulation({ 50.5, 50.5 });
}


/*
At each execution step of the scheduler, every agent attempts to move forward one step in the cur-rent direction.
After every agent has attempted to move, the entire population performs its sensory behavior. If the movement is 
successful (i.e., if the next site is not occupied) the agent moves to the new site and deposits a constant
chemoattractant value. If the movement is not successful, the agent remains in its current position, no
chemoattractant is deposited, and a new orientation is randomly se-lected. Agents are selected from the population
randomly in the motor and sensory stages to avoid the possibility of long term bias by sequential ordering. The
agent both deposits to and senses from the trail map, resulting in an autocrine mode of stimulus/response. Note
that patterning is also possible with a simple passive agent response—where the agent responds to chemoattractant
concentration without affecting the chemoattractant levels. The passive approach to particle RD computing is
described in [25], and a comparison of the active and passive approaches can be found in [26].
*/