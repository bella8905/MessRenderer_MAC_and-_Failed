/////////////////////////////////////////////////////////////////
//
//  Utilities - FileStreamHandler
//
// handles file i/o things such as stripping off comments
// TODO:: check on other platforms later
//
//  Coyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once

#include <fstream>
#include <sstream>
#include <queue>

#include "Utl_Global.h"

using std::ifstream;
using std::istringstream;
using std::queue;

namespace Utl {

// read in files in a safer way
// kindlly strip any comments starting with "//"
// and be nicer to different end flags
class SFileStreamHandler{

public:
    SFileStreamHandler( const string& t_File ) : _IsInited( false ) { Init( t_File ); }
    ~SFileStreamHandler() {}

private:
    bool _IsInited;

    ifstream _FileStream;
    queue<string> _CachedStrs;

public:
    // read one by one and skip comments
    SFileStreamHandler& operator >> ( int& t_Val );
    SFileStreamHandler& operator >> ( us& t_Val );
    SFileStreamHandler& operator >> ( ul& t_Val );
    SFileStreamHandler& operator >> ( string& t_Val );
    SFileStreamHandler& operator >> ( float& t_Val );
    SFileStreamHandler& operator >> ( bool& t_Val );

    operator void*() { if( _IsInited ) return this; else return 0;  }

    bool IsInited() const { return _IsInited; } 
    void CloseFile();
    void Deinit();

private:
    void Init( const string& t_file );

    void ClearCachedStrs();
    bool GetNextStrFromCachedStrs( string& t_Str );
    bool GetNextStrWithCommentStripped( string& t_Str );
    bool SafeReadLine( string& t_Str );   //Thanks to http://stackoverflow.com/a/6089413
    void FillCachedStrs( const string& t_Str );
    int StrToInt( const string& t_Str );
    float StrToFloat( const string& t_Str );
};

}