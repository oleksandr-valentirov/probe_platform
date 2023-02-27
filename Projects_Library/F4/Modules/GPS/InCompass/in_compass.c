/*
In this code, getDirection calculates the direction between two points on the Earth's 
surface using the haversine formula. getDistance calculates the distance between the points using the same formula.
*/



#include <math.h>

#define PI 3.14159265358979323846
#define EARTH_RADIUS 6371000


float toRadians(float degrees) {
    return degrees * PI / 180.0;
}

/*
The function getDirection returns the direction between two points on the Earth's 
surface in radians. This direction is measured from the north direction in a 
clockwise direction, so the result of the function will be in the range from 0 to 2π.
*/
float getDirection(float lat1, float lon1, float lat2, float lon2)
{
    float dLat = toRadians(lat2 - lat1);
    float dLon = toRadians(lon2 - lon1);

    float y = sin(dLon) * cos(toRadians(lat2));
    float x = cos(toRadians(lat1)) * sin(toRadians(lat2)) - sin(toRadians(lat1)) * cos(toRadians(lat2)) * cos(dLon);
    float brng = atan2(y, x);

    brng = toRadians(360) - fmod(toRadians(360) - brng, toRadians(360));

    return fmod(toRadians(360) - brng, toRadians(360));
}


/*
The function getDistance calculates the distance between two points on the Earth's 
surface and returns this distance in kilometers (km).
*/
float getDistance(float lat1, float lon1, float lat2, float lon2)
{
    float dLat = toRadians(lat2 - lat1);
    float dLon = toRadians(lon2 - lon1);

    float a = sin(dLat/2) * sin(dLat/2) +
            cos(toRadians(lat1)) * cos(toRadians(lat2)) *
            sin(dLon/2) * sin(dLon/2);
    float c = 2 * atan2(sqrt(a), sqrt(1-a));

    return EARTH_RADIUS * c;
}

/*
void example(void) {
    // start point
    float lat1 = 49.8397;
    float lon1 = 24.0297;

    // end point
    float lat2 = 50.4501;
    float lon2 = 30.5234;

    // Determine the direction of travel
    float direction = getDirection(lat1, lon1, lat2, lon2);

    // Determine the distance between the points
    float distance = getDistance(lat1, lon1, lat2, lon2);

    // Print the results
    printf("Direction: %f\n", direction);
    printf("Distance: %f\n", distance);
}
*/
