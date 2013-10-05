#ifndef ZOMBIEGAME_H
#define ZOMBIEGAME_H

#include "zombieengine.h"
#include "device.h"
#include "map.h"

#include <SDL.h>

namespace zombie {

	// Responsible of loading map, units and initiate all
	// game related things and to start the game engine.
	// It also handle all game events triggered by the game engine,
	// e.g. what happens when a unit dies.
	class ZombieGame {
	public:
		ZombieGame(int width = 500, int height = 500);
		~ZombieGame();

		// Starts the game.
		void startGame();

		// Draws the graphic and (deltaTime) should be the time past 
		// from the previous call to this funtion.
		void update(float deltaTime);

		// Makes the game reacting on an event (windowEvent).
		void eventUpdate(const SDL_Event& windowEvent);

		void zoom(double scale);

		void updateSize(int width, int height);

	private:
		void handleGameEvent(const GameEvent& gameEvent);
		void handleRemoval(bool& remove, MovingObject* mOb);

		float innerSpawnRadius_;
		float outerSpawnRadius_;

		Map map_;
		ZombieEngine engine_;
		DevicePtr keyboard1_, keyboard2_;		
	};

} // Namespace zombie_;

#endif // ZOMBIEGAME_H
