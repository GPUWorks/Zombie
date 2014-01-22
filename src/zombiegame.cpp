#include "zombiegame.h"
#include "inputkeyboard.h"
#include "weapon.h"
#include "movingobject.h"
#include "building.h"
#include "auxiliary.h"
#include "load.h"
#include "settings.h"
#include "car.h"
#include "human2d.h"
#include "zombie2d.h"
#include "car2d.h"
#include "weaponitem2d.h"
#include "building2d.h"

// External.
#include <mw/exception.h>

// Stl.
#include <cmath>
#include <sstream>

namespace zombie {

	ZombieGame::ZombieGame(const GameData& gameData) : engine_(this), gameData_(gameData) {
		keyboard1_ = DevicePtr(new InputKeyboard(SDLK_UP, SDLK_DOWN, SDLK_LEFT,
			SDLK_RIGHT, SDLK_SPACE, SDLK_r, SDLK_LSHIFT, SDLK_e));

		scale_ = 1.f;

		addKeyListener([&](gui::Component* component, const SDL_Event& keyEvent) {
			keyboard1_->eventUpdate(keyEvent);
		});
		
		innerSpawnRadius_ = 10.f;
		outerSpawnRadius_ = 40.f;
				
		gameData_.humanPlayer([&](State state, UnitProperties uP, const Animation& animation) {
			Unit* human = new Human2D(engine_.getWorld(), state,
				uP.mass_, uP.radius_, uP.life_, uP.walkingSpeed_,
				uP.runningSpeed_, Weapon(55, 0.2f, 8, 12), animation);
			engine_.setHuman(keyboard1_, human);
			viewPosition_ = human->getPosition();
		});
		
		gameData_.iterateCars([&](State state, CarProperties cP, const mw::Sprite& sprite) {
			// Engine takes the ownership.
			new Car2D(engine_.getWorld(), state, cP.mass_, cP.life_, cP.width_, cP.length_, sprite);
		});

		gameData_.iterateUnits([&](State state, UnitProperties uP, const Animation& animation) {
			engine_.addAi(new Zombie2D(engine_.getWorld(), state, uP.mass_, uP.radius_,
				uP.life_, uP.walkingSpeed_, uP.
				runningSpeed_, Weapon(35, 0.5f, 1, 10000), animation));
		});

		gameData_.iterateBuildings([&](BuildingProperties bP) {
			// Engine takes the ownership.
			new Building2D(engine_.getWorld(), bP.points_);
		});
	}

	ZombieGame::~ZombieGame() {
	}

	// Starts the game.
	void ZombieGame::startGame() {
		engine_.start();
	}

	void ZombieGame::draw(Uint32 deltaTime) {
		// Draw map centered around first human player.
		glPushMatrix();
		gui::Dimension dim = getSize();
		glTranslated(dim.width_*0.5f, dim.height_*0.5f, 0);
		glScale2f(50);
		glScale2f(scale_); // Is to fit the box drawn where x=[0,1] and y=[0,1].
		glTranslate2f(-viewPosition_);

		// Game is started?
		if (engine_.isStarted()) {
			engine_.update(deltaTime / 1000.f);
		} else {
			engine_.update(0);
		}

		glPopMatrix();
	}

	void ZombieGame::humanPosition(float x, float y) {
		viewPosition_ += 0.1f * (Position(x, y) - viewPosition_);
	}

	void ZombieGame::zoom(float scale) {
		scale_ *= scale;
	}

} // Namespace zombie.
