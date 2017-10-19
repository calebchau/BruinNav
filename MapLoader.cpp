#if defined(_WIN32) || defined(_WIN64)
#define strtok_r strtok_s
#endif

#include "provided.h"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;

private:
	vector<StreetSegment> m_segments;

};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream mapData(mapFile);

	// Can't open the file
	if (!mapData) {
		return false;
	}

	string line;
	while (getline(mapData, line)) {
		// Handle segment name
		StreetSegment streetSegment;
		streetSegment.streetName = line;

		// Handle segment geo coordinates
		getline(mapData, line);

		// Get starting geo coords
		char *context = NULL;
		char *p = strtok_r((char*)line.c_str(), ", ", &context);
		string str_lat = string(p);
		p = strtok_r(NULL, ", ", &context);
		string str_lon = string(p);
		GeoCoord startGeo(str_lat, str_lon);

		// Get ending geo coords
		p = strtok_r(NULL, ", ", &context);
		str_lat = string(p);
		p = strtok_r(NULL, ", ", &context);
		str_lon = string(p);
		GeoCoord endGeo(str_lat, str_lon);

		// Create the segment using the starting and ending coords
		streetSegment.segment = GeoSegment(startGeo, endGeo);

		// Handle attractions in segment
		int numAttr;
		mapData >> numAttr;
		mapData.ignore(10000, '\n');

		for (int i = 0; i < numAttr; i++) {
			// Handle attraction name
			getline(mapData, line);
			Attraction a;

			p = strtok_r((char*)line.c_str(), "|", &context);
			a.name = string(p);

			// Handle attraction geo coordinates
			p = strtok_r(NULL, ", ", &context);
			str_lat = string(p);
			p = strtok_r(NULL, ", ", &context);
			str_lon = string(p);

			a.geocoordinates = GeoCoord(str_lat, str_lon);

			// Add the attraction to the segment's vector of attractions
			streetSegment.attractions.push_back(a);
		}

		m_segments.push_back(streetSegment);
	}

	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_segments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum >= m_segments.size()) {
		return false;
	}

	// Get the desired segment in the vector
	seg = m_segments[segNum];

	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
	return m_impl->getSegment(segNum, seg);
}