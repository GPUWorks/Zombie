#include "roaddraw.h"
#include "map.h"

#include <SDL_opengl.h>

using namespace zombie;

RoadDraw::RoadDraw(const Map& map) : map_(map) {
	road_ = roadSprite;
}

bool RoadDraw::draw(double time) {
	const std::vector<LineFeature>& l = map_.getRoads();

	for (auto it = l.begin(); it != l.end(); it++) {
		double xStart =  it->getStart().x;
		double yStart =  it->getStart().y;
		double xEnd = it->getEnd().x;
		double yEnd =  it->getEnd().y;

		double x = xStart;
		double y = yStart;

		for (int i = 1; i < 100; i++) {
			glPushMatrix();
			glColor3d(0.4,0.4,0.4);
			glTranslated(0.5,0.5,0);
			glTranslated(x,y,0);
			//std::cout<<atan2((yStart-yEnd)/(xStart-xEnd));
			glRotated(std::atan2((yStart-yEnd),(xStart-xEnd)),0,0,1);
			glScaled(2,2,1);
			road_.draw();
			glPopMatrix();
			x = xStart + i*(xEnd-xStart)/100;
			y = yStart + i*(yEnd-yStart)/100;
		}
	}
	return true;
}
