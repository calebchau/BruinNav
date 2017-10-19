#include <string>
#include <iostream>
#include <cassert>
#include <cstring>
#include "provided.h"
#include "support.h"
#include "MyMap.h"

using namespace std;

//void printDirectionsRaw(string start, string end, const vector<NavSegment>& directions);
void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments);
void printDirections(string start, string end, vector<NavSegment>& navSegments);

int main(int argc, char *argv[])
{
	//MyMap<string, int> map;
	//map.associate("Caleb", 1);
	//map.associate("Veronica", 2);
	//map.associate("Nathan", 3);
	//map.associate("Chiu", 4);
	//map.associate("Pusheen", 5);
	//map.associate("Feibi", 6);

	//cout << "Caleb = " << *map.find("Caleb") << endl;
	//cout << "Chiu = " << *map.find("Chiu") << endl;
	//cout << "Feibi = " << *map.find("Feibi") << endl;
	//cout << "Nathan = " << *map.find("Nathan") << endl;
	//cout << "Pusheen = " << *map.find("Pusheen") << endl;
	//cout << "Veronica = " << *map.find("Veronica") << endl;

	//map.clear();

	//assert(map.size() == 0);
	//assert(map.find("Caleb") == nullptr);
	//assert(map.find("Chiu") == nullptr);
	//assert(map.find("Feibi") == nullptr);
	//assert(map.find("Nathan") == nullptr);
	//assert(map.find("Pusheen") == nullptr);
	//assert(map.find("Veronica") == nullptr);

	MapLoader ml;
	ml.load("mapdata.txt");

	//StreetSegment seg1;
	//ml.getSegment(0, seg1);
	//assert(seg1.streetName == "10th Helena Drive");
	//assert(seg1.segment.start.latitudeText == "34.0547000");
	//assert(seg1.segment.start.longitudeText == "-118.4794734");
	//assert(seg1.segment.end.latitudeText == "34.0544590");
	//assert(seg1.segment.end.longitudeText == "-118.4801137");
	//assert(seg1.attractions.size() == 0);

	//ml.getSegment(ml.getNumSegments() - 1, seg1);
	//assert(seg1.streetName == "access road for water tank");
	//assert(seg1.segment.start.latitudeText == "34.0853742");
	//assert(seg1.segment.start.longitudeText == "-118.4955413");
	//assert(seg1.segment.end.latitudeText == "34.0852898");
	//assert(seg1.segment.end.longitudeText == "-118.4954341");
	//assert(seg1.attractions.size() == 0);

	//AttractionMapper am;
	//am.init(ml);
	//GeoCoord geo;
	//assert(am.getGeoCoord("Broxton Plaza", geo) && geo.latitudeText == "34.0635414" && geo.longitudeText == "-118.4474561");
	//assert(am.getGeoCoord("BROXTON PLAZA", geo) && geo.latitudeText == "34.0635414" && geo.longitudeText == "-118.4474561");
	//assert(am.getGeoCoord("Geffen Playhouse", geo) && geo.latitudeText == "34.0634558" && geo.longitudeText == "-118.4445262");
	//assert(am.getGeoCoord("Starbucks Coffee", geo) && geo.latitudeText == "34.0594135" && geo.longitudeText == "-118.4447466");
	//assert(am.getGeoCoord("USC", geo) == false);

	//SegmentMapper sm;
	//sm.init(ml);
	//vector<StreetSegment> streets;
	//streets = sm.getSegments(geo); // Starbucks Coffee from previous test
	//assert(streets.size() == 1);
	//assert(streets[0].streetName == "Westwood Boulevard");
	//assert(streets[0].segment.start.latitudeText == "34.0596259" && streets[0].segment.start.longitudeText == "-118.4446352");
	//assert(streets[0].segment.end.latitudeText == "34.0595394" && streets[0].segment.end.longitudeText == "-118.4445598");

	//geo = GeoCoord("34.0564200", "-118.4799368");
	//streets = sm.getSegments(geo); // 16th Helena Drive end of 1st segment
	//assert(streets.size() == 2);
	//assert(streets[0].streetName == "16th Helena Drive");
	//assert(streets[0].segment.start.latitudeText == "34.0564067" && streets[0].segment.start.longitudeText == "-118.4798349");
	//assert(streets[0].segment.end.latitudeText == "34.0564200" && streets[0].segment.end.longitudeText == "-118.4799368");
	//assert(streets[1].streetName == "16th Helena Drive");
	//assert(streets[1].segment.start.latitudeText == "34.0564200" && streets[1].segment.start.longitudeText == "-118.4799368");
	//assert(streets[1].segment.end.latitudeText == "34.0564109" && streets[1].segment.end.longitudeText == "-118.4805269");

	//geo = GeoCoord("10", "20"); // unknown coordinate
	//streets = sm.getSegments(geo);
	//assert(streets.size() == 0);

	//Navigator navigator;
	//if (!navigator.loadMapData("mapdata.txt")) {
	//	cerr << "Error initializing map data" << endl;
	//	return 1;
	//}

	//vector<NavSegment> directions;
	//navigator.navigate("1061 Broxton Avenue", "Headlines!", directions);
	//printDirectionsRaw("1061 Broxton Avenue", "Headlines!", directions);

	//navigator.navigate("1031 Broxton Avenue", "1061 Broxton Avenue", directions);
	//printDirectionsRaw("1031 Broxton Avenue", "1061 Broxton Avenue", directions);

	//navigator.navigate("Triangle Fraternity", "Diddy Riese", directions);
	//printDirectionsRaw("Triangle Fraternity", "Diddy Riese", directions);

	//navigator.navigate("Rieber Terrace", "Fatburger", directions);
	//printDirectionsRaw("Rieber Terrace", "Fatburger", directions);

	//navigator.navigate("Southern Regional Library Facility", "Diddy Riese", directions);
	//printDirectionsRaw("Southern Regional Library Facility", "Diddy Riese", directions);

	//navigator.navigate("Greystone Mansion", "Greystone Mansion", directions);
	//printDirections("Greystone Mansion", "Greystone Mansion", directions);
	//printDirectionsRaw("Greystone Mansion", "Greystone Mansion", directions);

	bool raw = false;
	if (argc == 5 && strcmp(argv[4], "-raw") == 0)
	{
		raw = true;
		argc--;
	}
	if (argc != 4)
	{
		cout << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\"" << endl
			<< "or" << endl
			<< "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\" -raw" << endl;
		return 1;
	}

	Navigator nav;

	if (!nav.loadMapData(argv[1]))
	{
		cout << "Map data file was not found or has bad format: " << argv[1] << endl;
		return 1;
	}

	if (!raw)
		cout << "Routing..." << flush;

	string start = argv[2];
	string end = argv[3];
	vector<NavSegment> navSegments;

	NavResult result = nav.navigate(start, end, navSegments);
	if (!raw)
		cout << endl;

	switch (result)
	{
	case NAV_NO_ROUTE:
		cout << "No route found between " << start << " and " << end << endl;
		break;
	case NAV_BAD_SOURCE:
		cout << "Start attraction not found: " << start << endl;
		break;
	case NAV_BAD_DESTINATION:
		cout << "End attraction not found: " << end << endl;
		break;
	case NAV_SUCCESS:
		if (raw)
			printDirectionsRaw(start, end, navSegments);
		else
			printDirections(start, end, navSegments);
		break;
	}

	cout << "Passed all tests" << endl;
}

//void printDirectionsRaw(string start, string end, const vector<NavSegment>& directions) {
//	double distance = 0;
//	cerr << "You are starting at: " << start << endl;
//	for (auto dir : directions) {
//		if (dir.m_command == NavSegment::NavCommand::PROCEED) {
//			cerr << "Proceed " << dir.m_distance << " miles " << dir.m_direction << " on " << dir.m_streetName << endl;
//			distance += dir.m_distance;
//		} else {
//			cerr << "Turn " << dir.m_direction << " onto " << dir.m_streetName << endl;
//		}
//	}
//	cerr << "You have reached your destination: " << end << endl;
//	cerr << "Total travel distance: " << distance << " miles" << endl;
//}

void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments)
{
	cout << "Start: " << start << endl;
	cout << "End:   " << end << endl;
	cout.setf(ios::fixed);
	cout.precision(4);
	for (auto ns : navSegments)
	{
		switch (ns.m_command)
		{
		case NavSegment::PROCEED:
			cout << ns.m_geoSegment.start.latitudeText << ","
				<< ns.m_geoSegment.start.longitudeText << " "
				<< ns.m_geoSegment.end.latitudeText << ","
				<< ns.m_geoSegment.end.longitudeText << " "
				<< ns.m_direction << " "
				<< ns.m_distance << " "
				<< ns.m_streetName << endl;
			break;
		case NavSegment::TURN:
			cout << "turn " << ns.m_direction << " " << ns.m_streetName << endl;
			break;
		}
	}
}

void printDirections(string start, string end, vector<NavSegment>& navSegments)
{
	cout.setf(ios::fixed);
	cout.precision(2);

	cout << "You are starting at: " << start << endl;

	double totalDistance = 0;
	string thisStreet;
	GeoSegment effectiveSegment;
	double distSinceLastTurn = 0;

	for (auto ns : navSegments)
	{
		switch (ns.m_command)
		{
		case NavSegment::PROCEED:
			if (thisStreet.empty())
			{
				thisStreet = ns.m_streetName;
				effectiveSegment.start = ns.m_geoSegment.start;
			}
			effectiveSegment.end = ns.m_geoSegment.end;
			distSinceLastTurn += ns.m_distance;
			totalDistance += ns.m_distance;
			break;
		case NavSegment::TURN:
			if (distSinceLastTurn > 0)
			{
				cout << "Proceed " << distSinceLastTurn << " miles "
					<< directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
				thisStreet.clear();
				distSinceLastTurn = 0;
			}
			cout << "Turn " << ns.m_direction << " onto " << ns.m_streetName << endl;
			break;
		}
	}

	if (distSinceLastTurn > 0)
		cout << "Proceed " << distSinceLastTurn << " miles "
		<< directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
	cout << "You have reached your destination: " << end << endl;
	cout.precision(1);
	cout << "Total travel distance: " << totalDistance << " miles" << endl;
}