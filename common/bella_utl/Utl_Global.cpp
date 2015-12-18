/////////////////////////////////////////////////////////////////
//
//  Utilities - Global
// 
//  a reusable package which includes all basic things we need for CG projects
//
//  Coyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Utl_Global.h"
#include <ctime>

namespace Utl { 

string GetTime( EquryTimeType t_timeType )
 {
     time_t rawTime = time( 0 );
     tm* timeInfo = localtime( &rawTime );
     char buffer[80];
     if( t_timeType == TIME_STAMP_FILE_NAME ) {
         std::strftime(buffer,80,"%d_%m_%I_%M_%S", timeInfo);
     }
     else {
         std::strftime(buffer,80,"%d-%m-%Y %I:%M:%S", timeInfo);
     }

     return string( buffer );
 }

}