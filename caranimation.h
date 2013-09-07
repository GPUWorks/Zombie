#ifndef CARANIMATION_H
#define CARANIMATION_H

#include "graphictask.h"

#include <mw/sprite.h>

namespace zombie {

	class Car;

	class CarAnimation : public GraphicTask {
	public:
		CarAnimation(Car* car);
		~CarAnimation();

		bool draw(double time) override;
	
	private:				
		int idCarObject_;
		mw::Sprite carSprite_;
		double lastTime_;
	};

} // Namespace zombie.

#endif // CARANIMATION_H
