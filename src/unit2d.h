#ifndef UNIT2D_H
#define UNIT2D_H

#include "unit.h"
#include "animation.h"
#include "zombieentry.h"

#include <mw/sound.h>

namespace zombie {

	class Unit2D : public Unit {
	public:
		Unit2D();

		Unit2D(float mass, float radius, float life, float walkingSpeed,
			float runningSpeed, bool isInfected, const WeaponPtr& weapon, 
			const Animation& animation, Position grip);

		Unit2D(const Unit2D& unit);

		Unit2D& operator=(const Unit2D& unit);

		void setDieSound(const mw::Sound& sound);

		void setHitSound(const mw::Sound& sound);

		void draw(float accumulator, float timeStep, const GameShader& gameShader);

	private:
		void eventHandler(int unitEvent) override;

		Animation animation_;		
		mw::Sound die_;
		mw::Sound hit_;
	};

	Unit2D loadUnit(GameInterface* gameInterface, std::string unitTag, ZombieEntry entry, bool infected);

} // Namespace zombie.

#endif // UNIT2D_H
