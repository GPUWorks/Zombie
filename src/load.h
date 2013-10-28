#ifndef LOAD_H
#define LOAD_H

#include "typedefs.h"
#include "buildingproperties.h"
#include "carproperties.h"
#include "weaponproperties.h"
#include "unitproperties.h"
#include "settings.h"

#include <tinyxml2.h>

#include <stdexcept>
#include <cmath>
#include <sstream>

namespace zombie {

	// Takes c-string as input and returns the correct conversion.
	// Throws mw::Exception if the input is null or if the conversion
	// fails.
	// E.g. float a = convertFromText<float>("1.2");
	// Gives output, a = 1.2;
	template <class Output>
	Output convertFromText(const char* txt) {
		if (txt == nullptr) {
			throw std::runtime_error("convertFromText, input null");
		}

		std::stringstream stream(txt);
		Output output;
		// Extract formatted input succeeds?
		if (stream >> output) {
			return output;
		} else {
			throw std::runtime_error("convertFromText, extract formatted failed");
		}
	}

	// Returns the input txt. If the c-string is empty a runtime exception is thrown.
	template <>
	const char* convertFromText<const char*>(const char* txt);

	// Returns the settings loaded from the <settings> in xml.
	Settings loadSettings(tinyxml2::XMLHandle settingsTag);

	// Returns all weapons loaded from the <weapons> in xml.
	std::vector<WeaponProperties> loadWeapons(tinyxml2::XMLHandle weaponsTag);

	// Returns all buildings loaded from the <mapObjects> in xml.
	std::vector<BuildingProperties> loadBuildings(tinyxml2::XMLHandle buildingTag);

	// Returns all buildings loaded from the <movingUnits> in xml.
	std::vector<UnitProperties> loadUnits(tinyxml2::XMLHandle movingUnitsTag);

	// Returns all cars loaded from the <movingUnits> in xml.
	std::vector<CarProperties> loadCars(tinyxml2::XMLHandle movingUnitsTag);

} // Namespace zombie.

#endif // LOAD_H
