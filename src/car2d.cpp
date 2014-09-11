#include "car2d.h"
#include "auxiliary.h"
#include "animation.h"
#include "gamedataentry.h"

#include <mw/opengl.h>

namespace zombie {

	Car2D::Car2D(float mass, float life, float width, float length, const Animation& animation) :
		Car(mass, life, width, length), animation_(animation) {
	}

	// Draws the car.
	void Car2D::draw(float accumulator, float timeStep, gui::WindowMatrixPtr wPtr) {
		// Draw body.
		const float alpha = accumulator / timeStep;

		State state = getState();
		state.position_ = alpha * state.position_ + (1.f - alpha) * previousState().position_;

		wPtr->useShader();
		wPtr->setColor(1, 1, 1);
		mw::Matrix44 old = wPtr->getModel();
		wPtr->setModel(old * mw::getTranslateMatrix(state.position_.x, state.position_.y) * mw::getRotateMatrix(state.angle_, 0, 0, 1) * mw::getScaleMatrix(getWidth(), getWidth()));
		animation_.draw(timeStep, wPtr);
		wPtr->setModel(old);
	}

	Car2D loadCar(GameDataEntry& entry) {
		Animation animation = entry.getAnimation("moveAnimation");
		float mass = entry.getFloat("mass");
		float width = entry.getFloat("width");
		float length = entry.getFloat("length");
		float life = entry.getFloat("life");
		return Car2D(mass, life, width, length, animation);
	}
	
} // Namespace zombie.