#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

bool is_equal(const GeoCoord& lhs, const GeoCoord& rhs);
bool is_equal(const StreetSegment& lhs, const StreetSegment& rhs);

struct RouteSegment
{
	RouteSegment()
		: Gcost(0)
	{}

	// Since priority_queue implements a maxheap, we have to perform
	// the opposite comparison to get the desired behavior
	bool operator<(const RouteSegment& other) const
	{
		return Fscore > other.Fscore;
	}

	GeoCoord coord;
	GeoSegment segment;
	string streetName;
	double Gcost;
	double Fscore;
};

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
	void explore(priority_queue<RouteSegment>& toDo, const GeoCoord& end, const GeoCoord& geo, const GeoCoord& fromGeo, string streetName,
		MyMap<string, RouteSegment>& wayPointMap, RouteSegment& route) const;

private:
	MapLoader m_ml;
	SegmentMapper m_sm;
	AttractionMapper m_am;

};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	// Can't load the file
	if (!m_ml.load(mapFile)) {
		return false;
	}

	// Initialize the segment mapper and attraction mapper
	m_sm.init(m_ml);
	m_am.init(m_ml);

	return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord gc_start, gc_end;

	// Get the starting attraction's coords
	if (!m_am.getGeoCoord(start, gc_start)) {
		return NAV_BAD_SOURCE;
	}

	// Get the ending attraction's coords
	if (!m_am.getGeoCoord(end, gc_end)) {
		return NAV_BAD_DESTINATION;
	}

	// Clear the vector if it isn't empty
	if (!directions.empty()) {
		directions.clear();
	}

	// Convert the starting and ending attraction coords to string
	string str_gc_start = to_string(gc_start);
	string str_gc_end = to_string(gc_end);

	if (is_equal(gc_start, gc_end)) {
		return NAV_SUCCESS;
	}

	// Initiate the path finding from the starting geo location
	RouteSegment route;
	MyMap<string, RouteSegment> wayPointMap;
	priority_queue<RouteSegment> toDo;

	// Locate gc_start and gc_end segment
	StreetSegment ss_start = m_sm.getSegments(gc_start)[0];
	StreetSegment ss_end = m_sm.getSegments(gc_end)[0];
	bool routeFound = false;

	// Handle special case when start and end are on the same StreetSegment
	if (is_equal(ss_start, ss_end)) {
		// Construct the route from the starting coord to the ending coord
		route.coord = gc_start;
		route.segment = GeoSegment(gc_start, gc_end);
		route.streetName = ss_start.streetName;
		route.Gcost = distanceEarthMiles(gc_start, gc_end);
		wayPointMap.associate(str_gc_end, route);
		routeFound = true;
	} else {
		// Add the starting and ending coords of the segment that the attraction is on to be explored
		explore(toDo, gc_end, ss_start.segment.start, gc_start, ss_start.streetName, wayPointMap, route);
		explore(toDo, gc_end, ss_start.segment.end, gc_start, ss_start.streetName, wayPointMap, route);
	}

	// Loop until there are no more routes that are unexplored(route doesn't exist) or a route has been found
	while (!toDo.empty() && !routeFound) {
		// Get the first route from the queue(has the lowest Fscore)
		RouteSegment curr = toDo.top();
		toDo.pop();

		// Find all segments associated with the endpoint of the route segment
		vector<StreetSegment> segments = m_sm.getSegments(curr.segment.end);

		// Loop through the vector of associated segments
		for (size_t i = 0; i < segments.size(); i++) {
			// Check if the attraction is on the segment being processed(route found)
			if (is_equal(segments[i], ss_end)) {
				// Create a route to connect to the coord that the attraction is on
				route.coord = curr.segment.end;
				route.segment = GeoSegment(curr.segment.end, gc_end);
				route.streetName = segments[i].streetName;
				wayPointMap.associate(str_gc_end, route);
				routeFound = true;
				break;
			}

			// Check which end the route is connected to
			// e.g. if the route is connected to the starting coord of the segment, explore then ending coord
			// if the route is connected to the ending coord of the segment, explore the starting coord
			if (is_equal(curr.segment.end, segments[i].segment.start)) {
				explore(toDo, gc_end, segments[i].segment.end, curr.segment.end, segments[i].streetName, wayPointMap, route);
			} else {
				explore(toDo, gc_end, segments[i].segment.start, curr.segment.end, segments[i].streetName, wayPointMap, route);
			}
		}
	}

	// No route found
	if (!routeFound) {
		return NAV_NO_ROUTE;
	}

	// Create a stack to process the paths in the correct order
	// since the last item visited is the first item processed
	stack<RouteSegment> path;

	// Go through the map from the ending attraction all the way back
	// to the starting attraction and store the routes to create the path
	string key = str_gc_end;
	RouteSegment* curr = wayPointMap.find(key);
	
	// Loop through the routes that lead to the starting attraction
	while (curr != nullptr) {
		path.push(*curr);
		key = to_string((*curr).coord);
		curr = wayPointMap.find(key);
	}

	GeoSegment gs, next_gs;
	string streetName, nextStreetName;
	NavSegment navSegment;
	string direction;
	double distance;

	// Loop through the stored routes from the starting attraction to the ending attraction
	while (!path.empty()) {
		// Get the next route to process
		gs = path.top().segment;
		streetName = path.top().streetName;
		path.pop();

		// Calculate direction and distance of the segment
		direction = directionOfLine(gs);
		distance = distanceEarthMiles(gs.start, gs.end);

		// Add the route's info to the vector of directions
		directions.push_back(NavSegment(direction, streetName, distance, gs));

		// If there are routes following, compare the current route with the next route
		// to see if a turn is needed
		if (!path.empty()) {
			next_gs = path.top().segment;
			nextStreetName = path.top().streetName;

			if (nextStreetName != streetName) {
				// Add a turn
				direction = angleBetween2Lines(gs, next_gs) < 180 ? "left" : "right";
				directions.push_back(NavSegment(direction, nextStreetName));
			}
		}
	}

	// Route found
	return NAV_SUCCESS;
}

void NavigatorImpl::explore(priority_queue<RouteSegment>& toDo, const GeoCoord& end, const GeoCoord& geo, const GeoCoord& fromGeo, string streetName,
	MyMap<string, RouteSegment>& wayPointMap, RouteSegment& route) const
{
	// Convert the coord being explored to a string
	string coord = to_string(geo);

	// Check if the coord has been already explored or not
	// If the coord hasn't been explored, the map will return a nullptr
	if (wayPointMap.find(coord) == nullptr) {
		// Coord has not been explored yet
		// Store info about the segment into the route segment
		route.coord = fromGeo;
		route.segment = GeoSegment(fromGeo, geo);
		route.streetName = streetName;

		// Convert the starting coord that we are coming from to a string
		string origin = to_string(fromGeo);

		// Get the previous route segment that should be associated with the
		// starting coord of the current segment
		RouteSegment* pr = wayPointMap.find(origin);

		// Get the distance of the current segment
		double curr_dist = distanceEarthMiles(fromGeo, geo);

		// The only time the previous route segment will return nullptr is if
		// we are starting from the starting attraction(a.k.a. no routes have been explored yet)
		if (pr == nullptr) {
			// The Gcost should just be the distance from the starting attraction to the coord being explored
			route.Gcost = curr_dist;
		} else {
			// We are at a route farther down the line, so the Gcost should accumulate from the previous route's
			// Gcost and add the distance of the current segment to get it's Gcost
			route.Gcost = (*pr).Gcost + curr_dist;
		}

		// Calculate the Fscore by adding the Gcost with the distance from the current segment's ending coord to the 
		// end attraction's coord
		route.Fscore = route.Gcost + distanceEarthMiles(geo, end);

		// Add the route to the queue to be processed later and associate the coord being explored with the route
		// that it took to get there
		toDo.push(route);
		wayPointMap.associate(coord, route);
	}
}

bool is_equal(const GeoCoord & lhs, const GeoCoord & rhs)
{
	return lhs.latitude == rhs.latitude	&& lhs.longitude == rhs.longitude;
}

bool is_equal(const StreetSegment& lhs, const StreetSegment& rhs)
{
	return lhs.segment.start.latitude == rhs.segment.start.latitude
		&& lhs.segment.start.longitude == rhs.segment.start.longitude
		&& lhs.segment.end.latitude == rhs.segment.end.latitude
		&& lhs.segment.end.longitude == rhs.segment.end.longitude;
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}