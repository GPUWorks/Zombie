#ifndef OBJECT_H
#define OBJECT_H

#include <mw/mathvector.h>

#include <mw/packet.h>

namespace zombie {

// Represent a object living inside the "zombie world".
class Object {
public:
	Object(int id);
	Object(mw::Packet& packet);

	virtual ~Object();
	
	//virtual bool popGameEvent(GameEvent*& gameEvent) = 0;

	/* Update the internal states by time (timestep). */
	//virtual void updateStates(double timestep) = 0;
	
	/* Returns true if the position (x,y) is inside the object. */ 
	virtual bool isInside(double x, double y) const = 0;

	virtual double healthPoints() const = 0;
	virtual void updateHealthPoint(double deltaLife) = 0;
	virtual bool isDead() const = 0;
	virtual mw::Packet generatePacket() const;

	int id() const;
private:

	int id_;
	static int lastAddedId_;
};

} // namespace zombie

#endif // OBJECT_H