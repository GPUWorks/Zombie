#ifndef SURVIVORBEHAVIOR_H
#define SURVIVORBEHAVIOR_H

#include "aibehavior.h"
#include "typedefs.h"
#include "auxiliary.h"

#include <vector>

namespace zombie {

	class SurvivorBehavior : public AiBehavior {
	public:
		SurvivorBehavior();
		~SurvivorBehavior();

		Input calculateInput(Unit* unit, const std::vector<Unit*>& units, double time) override;

	private:
		Unit* findUninfectedTarget(Position position, const std::vector<Unit*>& units) const;

		double findNewTargetTime_;
		Unit* target_;

		double timeToUpdateAngleDirection_;
		double targetAngle_;
		bool forward_;
	};

} // Namespace zombie.

#endif // ZOMBIEBEHAVIOR_H

