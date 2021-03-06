#ifndef MISSILELAUNCHER2D_H
#define MISSILELAUNCHER2D_H

#include "missilelauncher.h"
#include "missile2d.h"
#include "zombieentry.h"
#include "weapon2d.h"

#include <mw/sound.h>

#include <memory>

namespace zombie {

	Weapon2D loadMissileLauncher2D(GameInterface* gameInterface, ZombieEntry entry);

	class MissileLauncher2D : public MissileLauncher {
	public:
		MissileLauncher2D(int clipSize, float timeBetweenShots, float range,
			float damageRadius, float damage, float deathTime, float speed, float force,
			mw::Sound shot = mw::Sound(), mw::Sound reload = mw::Sound())
			: MissileLauncher(clipSize, timeBetweenShots, range, damageRadius, damage, deathTime, speed, force),
			shot_(shot), reload_(reload) {

		}

		WeaponInterfacePtr clone() const override {
			return std::make_shared<MissileLauncher2D>(*this);
		}

	protected:
		void reload() override {
			mw::Sound sound = reload_;
			sound.play();
		}

	private:
		mw::Sound shot_, reload_;		
	};

} // Namespace zombie.

#endif // MISSILELAUNCHER2D_H
