#include "zombiegame.h"
#include "inputkeyboard.h"
#include "weapon.h"
#include "movingobject.h"
#include "building.h"
#include "auxiliary.h"
#include "load.h"

#include <mw/exception.h>

#include <cmath>
#include <sstream>

namespace zombie {

	// Returns a random postion between the defined outer and inner circle centered in position.
	Position generatePosition(Position position, float innerRadius, float outerRadius) {
		return position + (innerRadius + (outerRadius - innerRadius) * random()) * Position(std::cosf(random()*2.f*3.14f), std::sinf(random()*2.f*3.14f));
	}	

	ZombieGame::ZombieGame(int width, int height, tinyxml2::XMLHandle xml) : engine_(width, height) {
		// Set windows size.
		updateSize(width,height);
		
		keyboard1_ = DevicePtr(new InputKeyboard(SDLK_UP, SDLK_DOWN, SDLK_LEFT, 
			SDLK_RIGHT, SDLK_SPACE, SDLK_r,SDLK_LSHIFT,SDLK_e));

		engine_.addGrassGround(-50, 50, -50, 50);

		innerSpawnRadius_ = 10.f;
		outerSpawnRadius_ = 40.f;		
		
		//engine_.addEventListener(std::bind(&ZombieGame::handleGameEvent, this, std::placeholders::_1));

		load(xml);
		
		// Load map.
		loadMap("Base Defense Map");

		CarProperties volvoP = cars_["Volvo"];
		UnitProperties humanP = units_["Human"];
		UnitProperties zombieP = units_["Zombie"];
		
		Position position = generatePosition(ORIGO, 0, 50);
		engine_.addHuman(keyboard1_, position.x, position.y, 0.3f, humanP.mass_, humanP.radius_, humanP.life_, humanP.walkingSpeed_, humanP.runningSpeed_, Weapon(55, 0.2f, 8, 12));

		// Add cars.		
		for (int i = 0; i < 10; ++i) {
			Position spawn = generatePosition(ORIGO, 0, 50);
			engine_.addCar(spawn.x, spawn.y, 0.f, volvoP.mass_, volvoP.life_, volvoP.width_, volvoP.length_);
		}
		
		// Add zombies.
		for (int i = 0; i < 10; ++i) {
			Position spawn = generatePosition(ORIGO, 0, 50);
			engine_.addAi(spawn.x, spawn.y, 0.3f, zombieP.mass_, zombieP.radius_, zombieP.life_, zombieP.walkingSpeed_, zombieP.runningSpeed_, true, Weapon(35, 0.5f, 1, 10000));
		}
		
		for (int i = 0; i < 10; ++i) {
			Position spawn = generatePosition(ORIGO, 0, 50);
			engine_.addAi(spawn.x, spawn.y, 0.3f, humanP.mass_, humanP.radius_, humanP.life_, humanP.walkingSpeed_, humanP.runningSpeed_, false, Weapon(35, 0.5, 8, 120));
		}

		for (BuildingProperties& p : buildings_) {
			p.points_.pop_back(); // Last point same as first point! Due to difference in XML and Box2D.
			engine_.addBuilding(p.points_);
		}
	}

	ZombieGame::~ZombieGame() {
	}

	void ZombieGame::handleGameEvent(const GameEvent& gameEvent) {
		if (const UnitDie* unitDie = dynamic_cast<const UnitDie*>(&gameEvent)) {
			Position spawn = generatePosition(engine_.getMainUnitPostion(), innerSpawnRadius_, outerSpawnRadius_);
			//engine_.addAi(spawn.x, spawn.y, 0.3f, Weapon(35,0.5f,1,10000), true);
		}
	}

	void ZombieGame::handleRemoval(bool& remove, MovingObject* mOb) {
		bool outside = mOb->getPosition().LengthSquared() > outerSpawnRadius_ * outerSpawnRadius_;
		remove = outside;
	}

	// Starts the game.
	void ZombieGame::startGame() {
		engine_.start();
	}

	void ZombieGame::update(float deltaTime) {
		engine_.update(deltaTime);
	}

	void ZombieGame::zoom(double scale) {
		engine_.zoom(scale);
	}

	void ZombieGame::updateSize(int width, int height) {
		engine_.updateSize(width, height);
	}

	void ZombieGame::eventUpdate(const SDL_Event& windowEvent) {
		// Update human input.
		keyboard1_->eventUpdate(windowEvent);
	}

	bool ZombieGame::load(tinyxml2::XMLHandle xml) {
		try {
			loadSettings(xml.FirstChildElement("settings"));
			loadWeapons(xml.FirstChildElement("weapons"));
			loadMovingUnits(xml.FirstChildElement("movingObjects"));
		} catch (mw::Exception&) {
			return false;
		}
		return true;
	}

	// Must load weapons before. Else the no weapons will be placed.
	void ZombieGame::loadMap(std::string map) {
		// Load map file.
		tinyxml2::XMLDocument mapXml;
		mapXml.LoadFile("map.xml");
		if (mapXml.Error()) {
			// Failed!
			mapXml.PrintError();
			std::exit(1);
		}

		tinyxml2::XMLHandle mapHandle(mapXml);
		mapHandle = mapHandle.FirstChildElement("map");
		while (mapHandle.ToNode() != nullptr) {
			tinyxml2::XMLHandle tmp = mapHandle.FirstChildElement("name");
			std::string name = tmp.ToElement()->GetText();
			
			// Found map?
			if (name == map) {				
				tinyxml2::XMLHandle mapObHandler = mapHandle.FirstChildElement("mapObjects").FirstChildElement("mapObject");
				// Iterate through all mapObjects.
				while (mapObHandler.ToElement() != nullptr) {
					tinyxml2::XMLElement* tmp2 = mapObHandler.FirstChildElement("type").ToElement();
					std::string type = tmp2->GetText();

					if (type == "building") {
						tmp2 = tmp2->NextSiblingElement("geom");
						BuildingProperties properties = convertFromText<BuildingProperties>(tmp2->GetText());
						buildings_.push_back(properties);
					} else if (type == "humanSpawningPoint") {
						// Todo!
					} else if (type == "zombieSpawningPoint") {
						// Todo!
					} else {
						throw mw::Exception("No such mapObject: " + type + "\n");
					}

					// Next mapObject.
					mapObHandler = mapObHandler.NextSibling();
				}
				break;
			}

			// Next map.
			mapHandle = mapHandle.NextSiblingElement();
		}
	}

	Animation ZombieGame::loadAnimation(tinyxml2::XMLHandle xml) {
		tinyxml2::XMLHandle handle = xml.FirstChildElement("scale");
		if (handle.ToElement() == nullptr) {
			throw  mw::Exception("");
		}
		float scale = convertFromText<float>(handle.ToElement()->GetText());
		Animation animation(scale);
		
		while (handle.NextSiblingElement("image").ToElement() != nullptr) {
			handle = handle.NextSiblingElement("image");
			mw::TexturePtr texture = getLoadedTexture(handle.ToElement()->GetText());
			handle = handle.NextSiblingElement("time");
			float time = convertFromText<float>(handle.ToElement()->GetText());
			animation.add(texture, time);
		}

		return animation;
	}

	mw::TexturePtr ZombieGame::getLoadedTexture(std::string file) {
		unsigned int size = textures_.size();

		mw::TexturePtr& texture = textures_[file];
		// Image not found?
		if (textures_.size() > size) {
			texture = mw::TexturePtr(new mw::Texture(file));
			
			// Image not valid?
			if (!texture->isValid()) {
				// Return null pointer.
				texture = mw::TexturePtr();
			}
		}

		return texture;
	}
	
	void ZombieGame::loadWeapons(tinyxml2::XMLHandle xml) {
		// Find all weapons.
		tinyxml2::XMLElement* element = xml.FirstChildElement("weapon").ToElement();
		while (element != nullptr) {
			WeaponProperties properties;
			tinyxml2::XMLElement* tmp = element->FirstChildElement("name")->ToElement();
			properties.name_ = tmp->GetText();
			
			tmp = tmp->NextSiblingElement("image");
			properties.image_ = tmp->GetText();

			tmp = tmp->NextSiblingElement("damage");
			properties.damage_ = convertFromText<float>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("timeBetweenShots");
			properties.timeBetweenShots_ = convertFromText<float>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("range");
			properties.range_ = convertFromText<float>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("clipSize");
			properties.clipSize_ = convertFromText<int>(tmp->GetText());

			tmp = tmp->NextSiblingElement("shootSound");
			properties.shootSound_ = tmp->GetText();

			tmp = tmp->NextSiblingElement("reloadSound");
			properties.reloadSound_ = tmp->GetText();
			weapons_[properties.name_] = properties;

			element = element->NextSiblingElement();
		}
	}

	void ZombieGame::loadSettings(tinyxml2::XMLHandle xml) {
		try {
			tinyxml2::XMLElement* element = xml.FirstChildElement("width").ToElement();
			float w = convertFromText<float>(element->GetText());
		
			element = element->NextSiblingElement("height");
			float h = convertFromText<float>(element->GetText());

			element = element->NextSiblingElement("unitlevel");
			int unitLevel = convertFromText<int>(element->GetText());

			element = element->NextSiblingElement("zoomlevel");
			int zoomlevel = convertFromText<int>(element->GetText());
		} catch (mw::Exception&) {
			throw mw::Exception("Failed to load settings in xml file");
		}
	}

	void ZombieGame::loadMovingUnits(tinyxml2::XMLHandle xml) {
		// Find all cars.
		tinyxml2::XMLElement* element = xml.FirstChildElement("car").ToElement();
		while (element != nullptr) {
			CarProperties properties;
			tinyxml2::XMLElement* tmp = element->FirstChildElement("name")->ToElement();
			properties.name_ = tmp->GetText();
			
			tmp = tmp->NextSiblingElement("image");
			properties.image_ = tmp->GetText();

			tmp = tmp->NextSiblingElement("mass");
			properties.mass_ = convertFromText<float>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("width");
			properties.width_ = convertFromText<float>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("length");
			properties.length_ = convertFromText<float>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("life");
			properties.life_ = convertFromText<float>(tmp->GetText());
			cars_[properties.name_] = properties;

			element = element->NextSiblingElement("car");
		}

		// Find all units.
		element = xml.FirstChildElement("unit").ToElement();
		while (element != nullptr) {
			UnitProperties properties;
			tinyxml2::XMLElement* tmp = element->FirstChildElement("name")->ToElement();
			properties.name_ = tmp->GetText();

			tmp = tmp->NextSiblingElement("animation");
			properties.animation_ = loadAnimation(tmp);

			tmp = tmp->NextSiblingElement("mass");
			properties.mass_ = convertFromText<float>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("radius");
			properties.radius_ = convertFromText<float>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("life");
			properties.life_ = convertFromText<float>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("infected");
			properties.infected_ = convertFromText<bool>(tmp->GetText());
			
			tmp = tmp->NextSiblingElement("walkingSpeed");
			properties.walkingSpeed_ = convertFromText<float>(tmp->GetText());

			tmp = tmp->NextSiblingElement("runningSpeed");
			properties.runningSpeed_ = convertFromText<float>(tmp->GetText());

			tmp = tmp->NextSiblingElement("stamina");
			properties.stamina_ = convertFromText<float>(tmp->GetText());
			units_[properties.name_] = properties;

			element = element->NextSiblingElement("unit");
		}
	}

} // Namespace zombie.
