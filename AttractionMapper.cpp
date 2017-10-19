#include "provided.h"
#include "MyMap.h"
#include <string>
#include <cctype>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;

private:
	MyMap<string, GeoCoord> m_map;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	// Get number of segments to loop over
	size_t numSegments = ml.getNumSegments();

	// Loop through all the segments
	for (size_t i = 0; i < numSegments; i++) {
		// Handle individual segments
		StreetSegment streetSegment;
		ml.getSegment(i, streetSegment);

		// Loop through the segment's vector of attractions
		for (size_t j = 0; j < streetSegment.attractions.size(); j++) {
			// Get the lower case version of the attraction's name
			string name = streetSegment.attractions[j].name;

			for (size_t k = 0; k < name.size(); k++) {
				name[k] = tolower(name[k]);
			}

			// Associate the attraction with its geo coords
			m_map.associate(name, streetSegment.attractions[j].geocoordinates);
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	// Convert the attraction name to lower case
	for (size_t i = 0; i < attraction.size(); i++) {
		attraction[i] = tolower(attraction[i]);
	}

	// Check if it is in the map
	const GeoCoord* p_geoCoord = m_map.find(attraction);

	// Not in the map
	if (p_geoCoord == nullptr) {
		return false;
	}

	// Found it
	gc = *p_geoCoord;

	return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}