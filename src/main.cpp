#include "zombiewindow.h"

#include <tinyxml2.h>

#include <iostream>
#include <sstream>

using namespace tinyxml2;
using namespace std;

int main (int argc, char** argv) {

	// Load XML file.
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.LoadFile("zombie.xml");
	if (xmlDoc.Error()) {
		// Failed!
		return 1;
	}
	XMLHandle handleXml(xmlDoc.FirstChildElement("zombie"));
	XMLElement* element = handleXml.FirstChildElement("settings").FirstChildElement("width").ToElement();
	std::stringstream stream;
	stream << element->GetText() << " ";
	element = handleXml.FirstChildElement("settings").FirstChildElement("height").ToElement();
	stream << element->GetText() << " ";	

	int width, height;
	stream >> width >> height;
	
    zombie::ZombieWindow game(width, height, handleXml);
	game.startLoop();

    return 0;
}
