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


enum EquryTimeType { 
    TIME_STAMP,  // %d-%m-%Y %H:%M:%S
    TIME_STAMP_FILE_NAME,  // %d_%m_%H_%M_%S, use it if we want a attach time to file name
};
string GetTime( EquryTimeType t_time = TIME_STAMP );

}

