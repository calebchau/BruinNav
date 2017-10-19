#ifndef SUPPORT_H
#define SUPPORT_H

#include "provided.h"
#include <string>

inline const std::string to_string(const GeoCoord& gc)
{
	return gc.latitudeText + "," + gc.longitudeText;
}

inline const std::string directionOfLine(const GeoSegment& gs) {
	double degree = angleOfLine(gs);

	std::string dir;

	if (degree >= 0 && degree <= 22.5)
		dir = "east";
	else if (degree > 22.5 && degree <= 67.5)
		dir = "northeast";
	else if (degree > 67.5 && degree <= 112.5)
		dir = "north";
	else if (degree > 112.5 && degree <= 157.5)
		dir = "northwest";
	else if (degree > 157.5 && degree <= 202.5)
		dir = "west";
	else if (degree > 202.5 && degree <= 247.5)
		dir = "southwest";
	else if (degree > 247.5 && degree <= 292.5)
		dir = "south";
	else if (degree > 292.5 && degree <= 337.5)
		dir = "southeast";
	else if (degree > 337.5 && degree < 360)
		dir = "east";

	return dir;
}

#endif