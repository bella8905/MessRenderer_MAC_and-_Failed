/////////////////////////////////////////////////////////////////
//
//  Utilities - Global
// 
//  a reusable package which includes all basic things we need for CG projects
//
//  Coyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <string>
#include <time.h>

using std::string;


#define Clamp( x, xmin, xmax )	x < xmin ? xmin : ( x > xmax ? xmax : x )

typedef unsigned short us;
typedef unsigned long ul;

namespace Utl { 

template<typename T>
bool Equals( const T& t_x, const T& t_y )  { return std::abs( t_x - t_y ) < std::numeric_limits<T>::epsilon(); }

template bool Equals( const float& t_x, const float& t_y );
template bool Equals( const double& t_x, const double& t_y );

enum EquryTimeType { 
    TIME_STAMP,  // %d-%m-%Y %H:%M:%S
    TIME_STAMP_FILE_NAME,  // %d_%m_%H_%M_%S, use it if we want a attach time to file name
};
string GetTime( EquryTimeType t_time = TIME_STAMP );

}

