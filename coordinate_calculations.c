#include <math.h>
#include "coordinate_calculations.h"

const double KPiDouble = 3.141592654;
const double KDegreesToRadiansDouble = 3.141592654 / 180.0;

/**
A constant to convert radians to metres for the Mercator and other projections.
It is the semi-major axis (equatorial radius) used by the WGS 84 datum (see http://en.wikipedia.org/wiki/WGS84).
*/
const int KEquatorialRadiusInMetres = 6378137;

/** 
Find the great-circle distance in metres, assuming a spherical earth, between two lat-long points in degrees. 
http://stackoverflow.com/a/12383682
*/
double GreatCircleDistanceInMeters(double aLong1,double aLat1,double aLong2,double aLat2)
    {
    aLong1 *= KDegreesToRadiansDouble;
    aLat1 *= KDegreesToRadiansDouble;
    aLong2 *= KDegreesToRadiansDouble;
    aLat2 *= KDegreesToRadiansDouble;
    double cos_angle = sin(aLat1) * sin(aLat2) + cos(aLat1) * cos(aLat2) * cos(aLong2 - aLong1);

    /*
    Inaccurate trig functions can cause cos_angle to be a tiny amount
    greater than 1 if the two positions are very close. That in turn causes
    acos to give a domain error and return the special floating point value
    -1.#IND000000000000, meaning 'indefinite'. Observed on VS2008 on 64-bit Windows.
    */
    if (cos_angle >= 1)
        return 0;

    double angle = acos(cos_angle);
    return angle * KEquatorialRadiusInMetres;
    }
