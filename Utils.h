#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

// distance calculates the distance between two points, using their latitude and longitude
// Parameters: lat1 and lon1 are the latitude and longitude of the first point
//             lat2 and lon2 are the latitude and longitude of the second point
//             returns the distance between those two points in miles
//                calculated "as the crow flies" meaning that it doesn't follow streets
// Distance formula by Ed Williams https://edwilliams.org/avform147.htm
// Calculates the "great circle distance" between two points, using their latitude and longitude
// Although we don't need great circle distance for finding out way between two points in the same city
// it is still a helpful formula to go from latitude and longitude to distance
double distance(double lat1, double lon1, double lat2, double lon2) {
	// Switch to latidue and longitude in radians
	lat1 *= M_PI / 180;
	lon1 *= M_PI / 180;
	lat2 *= M_PI / 180;
	lon2 *= M_PI / 180;

	double distanceInRadians = 2 * asin(sqrt( pow((sin((lat1 - lat2) / 2)), 2) +
			cos(lat1) * cos(lat2) * pow((sin((lon1 - lon2) / 2)), 2)));

	// Convert to nautical miles, then multiply by 1.1508 to get regular miles
	return distanceInRadians * 180 * 60 / M_PI * 1.1508;
}