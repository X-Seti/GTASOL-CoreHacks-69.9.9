#ifndef __UTILITIES_H
#define __UTILITIES_H

#define M_PI       3.14159265358979323846
#define ARRLEN(a) (sizeof(a)/sizeof(a[0])) 
#include <math.h>
#define _pad(x,n) unsigned char x[n]
#define OFFSETOF(type, field)    ((unsigned long) &(((type *) 0)->field))
#include <cmath>    // To ensure getting a fixed set of overloads
using std::sqrt;

inline int
sqrt(int in)
{
	return static_cast<int>(std::sqrt(static_cast<double>(in)));
}

//  And so on for any standard integral types you want...
//  And your class here...

namespace utl{
    template <typename T> inline const T abs(T const & x){
        return ( x<0 ) ? -x : x;
    }

    /* This function adjust the angle in a specified range
     */
    inline const float AdjustLimitsToAngle(float fAngle, float const fLowerBound, float const fUpperBound) {
        while(fAngle > fUpperBound) {
            fAngle -= (2.0f * (float)M_PI);
        }
        while(fAngle < fLowerBound) {
            fAngle += (2.0f * (float)M_PI);
        }
        return fAngle;
    }
}



/*
namespace SafetyFirst
{
	inline int
		sqrt(int in)
	{
		return static_cast<int>(std::sqrt(static_cast<double>(in)));
	}

	inline double
		sqrt(double in)
	{
		return std::sqrt(in);
	}

	//  And so on for any other types you might need.  The
	//  standard provides std::sqrt for the floating point
	//  types only.
}
*/

#endif