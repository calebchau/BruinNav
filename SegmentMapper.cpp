#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;

private:
	MyMap<string, vector<StreetSegment>> m_map;

};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	// Get number of segments to loop over
	size_t numSegments = ml.getNumSegments();

	// Loop through all the segments
	for (size_t i = 0; i < numSegments; i++) {
		// Handle individual segments
		StreetSegment streetSegment;
		ml.getSegment(i, streetSegment);

		// Handle the segment's start and end coords
		// Convert the segment's start coord to a string and try to find it
		string coord = to_string(streetSegment.segment.start);
		vector<StreetSegment>* pv = m_map.find(coord);

		// No association exists yet, create a new vector with the current segment
		// and associate the segment's start coord with the vector
		if (pv == nullptr) {
			vector<StreetSegment> vec;
			vec.push_back(streetSegment);
			m_map.associate(coord, vec);
		} else {
			// Association already exists, push the current segment onto the existing vector
			pv->push_back(streetSegment);
		}

		// Convert the segment's end coord to a string and try to find it
		coord = to_string(streetSegment.segment.end);
		pv = m_map.find(coord);

		// No association exists yet, create a new vector with the current segment
		// and associate the segment's end coord with the vector
		if (pv == nullptr) {
			vector<StreetSegment> vec;
			vec.push_back(streetSegment);
			m_map.associate(coord, vec);
		} else {
			// Association already exists, push the current segment onto the existing vector
			pv->push_back(streetSegment);
		}

		// Loop through the segment's vector of attractions
		for (size_t j = 0; j < streetSegment.attractions.size(); j++) {
			// Convert the attraction's coord to a string
			// Associate it with the segment that it is located on
			coord = to_string(streetSegment.attractions[j].geocoordinates);
			vector<StreetSegment> vec;
			vec.push_back(streetSegment);
			m_map.associate(coord, vec);
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;

	// Convert the desired coordinate to a string and try to find it
	string coord = to_string(gc);
	const vector<StreetSegment>* pv = m_map.find(coord);

	// No segments associated with the coord, return empty vector
	if (pv == nullptr) {
		return segments;
	}

	// Return associated vector with the segments
	return *pv;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}