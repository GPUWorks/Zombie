#ifndef ZOMBIEGAME_H
#define ZOMBIEGAME_H

#include "zombieengine.h"
#include "gameinterface.h"
#include "device.h"
#include "gamedata.h"
#include "graphic.h"
#include "terrain2d.h"
#include "explosion.h"
#include "fog.h"
#include "weaponeffect.h"
#include "weaponinterface.h"

#include <mw/texture.h>
#include <mw/sprite.h>
#include <mw/sound.h>

#include <gui/component.h>

#include <map>
#include <memory>
#include <list>

namespace zombie {

	class Unit2D;
	class Car2D;
	class Weapon2D;

	// Responsible of loading map, units and initiate all
	// game related things and to start the game engine.
	// It also handle all game events triggered by the game engine,
	// e.g. what happens when a unit dies. All graphics and sound is
	// started here.
	class ZombieGame : public GameInterface, public gui::Component {
	public:
		ZombieGame(const GameData& gameData);
		~ZombieGame();

		// Start the game.
		void startGame();

		// Draw the graphic and (deltaTime) should be the time past
		// from the previous call to this funtion.
		// Override, gui::Component.
		void draw(Uint32 deltaTime) override;

		void zoom(float scale);

		int getZombiesKilled() const {
			return zombiesKilled_;
		}

		float getHealth() {
			return health_;
		}

		int getClipSize() {
			return clipsize_;
		}

		int getBulletsInWeapon() {
			return bulletsInWeapon_;
		}

		int getNbrUnits() {
			return engine_.getNbrUnits();
		}

		void init(gui::WindowMatrixPtr wptr);

	private:
		// Implements the GameInterface.
		void updateEachCycle(Unit& unit, Unit& human) override;

		void updateEachCycle(Unit& human) override;

		void humanDied(Unit& unit) override;

		void unitDied(Unit& unit) override;

		void collision(float impulse, Unit& unit) override;

		void collision(float impulse, Car& car) override;

		void collision(float impulse, Building& building) override;

		void shotMissed(Position startPosition, Position hitPosition) override;

		void shotHit(Position startPosition, Position hitPosition, Unit& unit) override;

		void explosion(Position position, float explosionRadius) override;

		// End of the GameInterface.

		void loadTerrain();
		
		float innerSpawnRadius_;
		float outerSpawnRadius_;
		float scale_;
		float lastSpawnTime_;
		float spawnPeriod_;

		std::shared_ptr<Explosion> explosion_;
		//std::shared_ptr<Fog> fog_;
		
		Animation humanInjured_;
		Animation humanDie_;
		Animation zombieInjured_;
		Animation zombieDie_;
		mw::Sprite tree_;
		mw::Sprite wall_;
		mw::Music music_;

		Position viewPosition_;
		Position refViewPosition_;

		ZombieEngine engine_;

		DevicePtr keyboard_;
		const GameData& gameData_;

		std::unique_ptr<Unit2D> human_;
		std::unique_ptr<Unit2D> zombie_;
		std::unique_ptr<Car2D> car_;

		std::list<std::shared_ptr<Graphic>> graphicGround_;
		std::list<std::shared_ptr<Graphic>> graphicMiddle_;
		std::list<std::shared_ptr<Graphic>> graphicSky_;
		std::vector<Position> spawningPoints_;
		std::map<std::string, Weapon2D> weapons_;

		Terrain2D terrain_;
		int zombiesKilled_;
		int unitMaxLimit_;
		int nbrUnits_;
		float health_;
		int bulletsInWeapon_;
		int clipsize_;
	};

} // Namespace zombie.

#endif // ZOMBIEGAME_H
