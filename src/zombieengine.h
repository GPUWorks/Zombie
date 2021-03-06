#ifndef ZOMBIEENGINE_H
#define ZOMBIEENGINE_H

#include "box2ddef.h"
#include "contactlistener.h"
#include "actionhandler.h"

namespace zombie {

	// Forward declarations.
	class Weapon;
	class GameInterface;
	class Missile;
	class Building;
	class Unit;
	class Car;
	class Object;

	// Responsible of handeling the box2d part of the simulating.
	// I.e. hides the Box2d usage.
	class ZombieEngine : public ActionHandler {
	public:
		ZombieEngine(GameInterface& gameInterface, float impulseThreshold);
		~ZombieEngine();

		// Update the box2d interaction.
		void update(float timeStep);

		// Add a unit.
		void add(Unit* unit);

		// Add a car to the engine.
		void add(Car* car);

		// Add a generic object to the engine.
		void add(Object* object);
		
		// Remove the object from the world.
		void remove(Object* object);

		// Get the current game time.
		inline float getTime() const {
			return time_;
		}

		inline float getImpulseThreshold() const {
			return impulseThreshold_;
		}

	private:
		void unitEvent(Unit* unit, int eventType) override;
		void carEvent(Car* car, int eventType) override;

		void doAction(Unit* unit);
		void doAction(Car* unit);

		bool started_; // The game is started.
		float time_; // Game time.

		b2World world_;
		GameInterface& gameInterface_;
		ContactListener contactListener_;

		float impulseThreshold_;
	};

} // Namespace zombie.

#endif // ZOMBIEENGINE_H
