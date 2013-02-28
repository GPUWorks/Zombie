#ifndef ZOMBIEGAME_H
#define ZOMBIEGAME_H

#include "inputkeyboard.h"
//#include "inputjoystick.h"

#include "quadtree.h"
#include "input.h"
#include "map.h"
#include "bullet.h"

#include "typedefs.h"

#include <SDL.h>
#include <memory>
#include <vector>
#include <string>

namespace zombie {

// Forward declaration.
class TaskManager;
class PhysicalEngine;

class ZombieGame {
public:
    ZombieGame(int width = 500, int height = 500);
    ~ZombieGame();
    
	// Starts the game. The connection need to be active else nothing happens.
	void startGame();

	// Updates the game time by (msDeltaTime).
    void physicUpdate(Uint32 msDeltaTime);

	// Draws the graphic and (msDeltaTime) should be the time past 
	// from the previous call to this funtion.
	void graphicUpdate(Uint32 msDeltaTime);

	// Makes the game reacting on the evennt (windowEvent).
	void eventUpdate(const SDL_Event& windowEvent);

	void zoom(double scale);

	void updateSize(int width, int height);
protected:
	void reshapeWindowsOpenGL(int width, int height) {		
	}

	// Add a human player (unitPtr) to the game.
	void addHuman(HumanPlayerPtr human, UnitPtr unitPtr);

	// Add a new ai (unitPtr) to the game.
	void addNewAi(UnitPtr unitPtr);

	//void updateWindowSize(int width, int height);

	// sets the most south west building in "origo"
	void normalizeBuildings();
	
	// Inits the game.
	void initGame();

	// Returns a vector of all units visible by the unit (unit).
	std::vector<UnitPtr> calculateUnitsInView(const UnitPtr& unit);	

	void doShotDamage(UnitPtr shooter, const Bullet& properties);
	bool isVisible(UnitPtr unitToBeSeen, UnitPtr unitThatSees) const;

	void spawnAndCleanUpUnits();	// Spawns new zombies

	bool started_; // The game time is started.	
	double time_; // Local game time.

	typedef std::pair<HumanPlayerPtr,UnitPtr> PairHumanUnit;
	typedef std::pair<AiPlayerPtr,UnitPtr> PairAiUnit;
	typedef std::pair<PlayerPtr,UnitPtr> PairPlayerUnit;
	
	std::vector<PairHumanUnit> humanPlayers_;
	std::vector<PairAiUnit> aiPlayers_;
	std::vector<PairPlayerUnit> players_; // All units.
	
	PhysicalEngine* physicalEngine_;
	Quadtree<BuildingPtr> buildings_; // All buildings.
	TaskManager* taskManager_;

	int indexAiPlayer_;
	double timeToUpdateView_;
	double timeToUpdateSpawn_;
	double timeSinceSpawn_;

	int unitLevel_; // Specifies the wanted number of zombies on the map
	double scale_;
	double innerSpawnRadius_;
	double outerSpawnRadius_;
	Map map_;

	Position viewPosition_;
	bool graphic3D_;
};

} // namespace zombie

#endif // ZOMBIEGAME_H