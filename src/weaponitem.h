#ifndef WEAPONITEM_H
#define WEAPONITEM_H

#include "object.h"
#include "weapon.h"

namespace zombie {

	class WeaponItem : public Object {
	public:
		WeaponItem(float x, float y, const WeaponPtr& weapon) {
			radius_ = 0.5;
			weapon_ = weapon;

			body_ = nullptr;
		}

		void createBody(b2World* world) {
			// Box2d properties.
			b2BodyDef bodyDef;
			bodyDef.position.Set(x_, y_);
			bodyDef.angle = 0.0;
			body_ = world->CreateBody(&bodyDef);
			body_->SetUserData(this);

			// Add tensor. Should not be a physical object.
			{
				b2CircleShape circle;
				circle.m_p.Set(0, 0);
				circle.m_radius = radius_;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circle;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				fixtureDef.isSensor = true;

				// Add Body fixture.
				b2Fixture* fixture = body_->CreateFixture(&fixtureDef);
				fixture->SetUserData(this);
			}
		}

		const WeaponPtr& getWeapon() const {
			return weapon_;
		}

		Position getPosition() const {
			return body_->GetPosition();
		}

		float getRadius() const {
			return radius_;
		}

		b2Body* getBody() const override {
			return body_;
		}

	private:
		WeaponPtr weapon_;
		float radius_;
		float x_, y_;


		b2Body* body_;
	};

} // Namespace zombie.

#endif // WEAPONITEM_H
