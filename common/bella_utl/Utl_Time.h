/////////////////////////////////////////////////////////////////
//
//  Utilities - Utl_Time
//  Time / Timer
//
//
//  Copyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////
#pragma once

#include <time.h>
#include "Utl_LogMsg.h"

namespace Utl {

struct STimer{
    clock_t _StartTime;

    void StartTimer( string t_Module ) {
        LogError<< t_Module << LogEndl;
        _StartTime = clock();
    }

    void EndTimer() {
        int ElapsedTime = (int)( ( clock() - _StartTime ) / CLOCKS_PER_SEC );
        int Hours = ElapsedTime / 3600;
        int Minutes = ElapsedTime % 3600 / 60;
        int Seconds = ElapsedTime % 3600 % 60;

        std::cout << "Time Elapsed: " << Hours << "h, " << Minutes <<"m, " << Seconds << "s." << std::endl;
    }
};

}

