/////////////////////////////////////////////////////////////////
//
//  Utilities - FileStreamHandler
//
//
//  Coyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Utl_FileStreamHandler.h"
#include "Utl_LogMsg.h"

namespace Utl {

void SFileStreamHandler::Init( const string& t_file )
{
    if( _IsInited ) 
    {
        // MsgErr( "already inited" );
        return;
    }

    _FileStream.open( t_file );
    if( !_FileStream || _FileStream.bad() )
    {
        // MsgErr( "open file error" );
        LogError<<"_FILE_STREAM_HANDLER: open file error. "<<t_file<<LogEndl;
        CloseFile();
        return;
    }

    _IsInited = true;
}

void SFileStreamHandler::Deinit()
{
    if( !_IsInited )
    {
        // MsgErr( "hasn't been inited" );
        return;
    }

    // Assert( _CachedStrs.empty(), "try to deinit when we still have unhandled strs" );
    ClearCachedStrs();
    CloseFile();
    _IsInited = false;
}

void SFileStreamHandler::CloseFile()
{
    if( _IsInited )
    {
        _FileStream.close();
        _FileStream.clear();
    }
}

void SFileStreamHandler::ClearCachedStrs()
{
    queue<string>().swap( _CachedStrs );
}


bool SFileStreamHandler::GetNextStrFromCachedStrs( string& t_Str )
{
    string Str;
    if( !_CachedStrs.empty() )
    {
        // fetch the next one from our cached queue
        Str = _CachedStrs.front();
        // if the string starts with "//", abandon this line
        string CommentPrefix( "//" );
        if( !Str.compare( 0, CommentPrefix.size(), CommentPrefix ) )
        {
            ClearCachedStrs();
        }
        else
        {
            // return this str
            t_Str = Str;
            _CachedStrs.pop();
            return true;
        }
    }

    return false;
}

void SFileStreamHandler::FillCachedStrs( const string& t_Str )
{
    ClearCachedStrs();
    std::stringstream strstr( t_Str );
    std::istream_iterator<std::string> it( strstr );
    std::istream_iterator<std::string> end;

    while( it != end )
    {
        _CachedStrs.push( *it );
        ++it;
    }
}

bool SFileStreamHandler::GetNextStrWithCommentStripped( string& t_Str )
{
    t_Str.clear();
    if( !_IsInited )
    {
        // MsgErr( "hasn't inited" );
        return false;
    }

    string Str;
    if( !GetNextStrFromCachedStrs( Str ) )
    {
        // read and fill a line from stream
        if( !SafeReadLine( Str ) )
        {
            // we don't have anything new, clear everything and quit
            Deinit();
            return false;
        }

        FillCachedStrs( Str );
        if( !GetNextStrFromCachedStrs( Str ) )
        {
            Deinit();
            return false;
        }
    }

    t_Str = Str;
    return true;
}


bool SFileStreamHandler::SafeReadLine( string& t_Str)
{
    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.
    t_Str.clear();
    std::istream::sentry se( _FileStream, true);
    std::streambuf* sb = _FileStream.rdbuf();

    bool CheckingStr = true;
    while( CheckingStr ) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            CheckingStr = t_Str.empty();  //don't stop here if we got nothing
            break;
        case '\r':
            if(sb->sgetc() == '\n') sb->sbumpc();
            CheckingStr = t_Str.empty();
            break;
        case EOF:
            // Also handle the case when the last line has no line ending
            if( t_Str.empty()) _FileStream.setstate( std::ios::eofbit );
            CheckingStr = false;
            break;
        default:
            t_Str += (char)c;
        }
    }

    if( t_Str.empty() )   return false;
    return true;

}

SFileStreamHandler& SFileStreamHandler::operator >> ( string& t_Val )
{
    if( _IsInited ) GetNextStrWithCommentStripped( t_Val );
    return *this;
}


SFileStreamHandler& SFileStreamHandler::operator >> ( int& t_Val )
{
    if( _IsInited )
    {
        string Str;
        ( *this ) >> Str;
        t_Val = StrToInt( Str );
    }
    return *this;
}


SFileStreamHandler& SFileStreamHandler::operator >> ( bool& t_Val )
{
    if( _IsInited )
    {
        string Str;
        ( *this ) >> Str;
        t_Val = ( StrToInt( Str ) != 0 );
    }
    return *this;
}

SFileStreamHandler& SFileStreamHandler::operator >> ( us& t_Val )
{
    if( _IsInited )
    {
        int IntVal;
        ( *this ) >> IntVal;
        t_Val = ( us )IntVal;
    }
    return *this;
}

SFileStreamHandler& SFileStreamHandler::operator >> ( ul& t_Val )
{
    if( _IsInited )
    {
        int IntVal;
        ( *this ) >> IntVal;
        t_Val = ( ul )IntVal;
    }
    return *this;
}

SFileStreamHandler& SFileStreamHandler::operator >> ( float& t_Val )
{
    if( _IsInited )
    {
        string Str;
        ( *this ) >> Str;
        t_Val = StrToFloat( Str );
    }
    return *this;
}


int SFileStreamHandler::StrToInt( const string& t_Str )
{
    return atoi( t_Str.c_str() );
}

float SFileStreamHandler::StrToFloat( const string& t_Str )
{
    return ( float )atof( t_Str.c_str() );
}

}