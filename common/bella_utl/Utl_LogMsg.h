/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - Utl_ConsoleMsg
//
//
//  Copyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once


// this is only for windows now
// need to implement something else for OSX and Linux
#ifdef _WIN32
#include <windows.h>
#include <ostream>

#endif

#include "Utl_Global.h"
#include <fstream>
using namespace std;

enum CoutColor { COLOR_PASS, COLOR_ERROR, COLOR_WARNING, COLOR_WHITE };


extern ofstream outFile;
extern const string LOG_FILE;



class ColorModifier {
public:
    ColorModifier() : _color( COLOR_WHITE ) {}  // set it to something else
    ColorModifier( CoutColor t_color ) : _color( t_color ) {}
    ~ColorModifier() {}
     
private:
    CoutColor _color;
    
#ifdef _WIN32
    int _convertColorToConsoleColorID( CoutColor t_color ) {
        switch( t_color ) {
            case COLOR_PASS:
                return 10;
            case COLOR_ERROR:
                return 12;
            case COLOR_WARNING:
                return 14;
            case COLOR_WHITE:
                return 15;
            default: return -1;
        }
        
        return;
    }
#endif

public:
    CoutColor GetColor() const {
#ifdef _WIN32
        return _convertColorToConsoleColorID( _color );
#else
        return _color;
#endif
    }
};

class ColorReverter {
private:
    ColorReverter() {}
    ~ColorReverter() { if( _instance != 0 ) { delete _instance; _instance = 0; } }

#ifdef _WIN32
public:
    static CONSOLE_SCREEN_BUFFER_INFO _csbi;
#endif

private:
    static ColorReverter* _instance;

public:
    static ColorReverter* GetInstance() {
        if( _instance == 0 ) {
            _instance = new ColorReverter();
#ifdef _WIN32
            HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
            GetConsoleScreenBufferInfo( hstdout, &_csbi );
#endif
        }

        return _instance;
    }
};


std::ostream& operator<<( std::ostream& os, const ColorModifier& mod); 
std::ostream& operator<<( std::ostream& os, const ColorReverter* rev);

extern ColorModifier ColorMod_Warning;
extern ColorModifier ColorMod_Error;
extern ColorModifier ColorMod_Pass;
extern ColorReverter* ColorMod_Restore;



class ColorCout {
public:
    ColorCout( const ColorModifier& t_colorMod );
    ColorCout();
    ~ColorCout() {}

private:
    ColorModifier _colorMod;
    bool _colorWritten;

public:
    template< typename T >
    ColorCout& operator<<( T t_val )
    {
        string prepStr;
        if( !_colorWritten )
        {
            _colorWritten = true;
            bool isColorUsed = false;       // only going to set color for types which make sense

            switch( _colorMod.GetColor() )
            {
            case COLOR_WARNING:
                prepStr = "[warn] ";   
                isColorUsed = true;
                break;
            case COLOR_ERROR:
                prepStr = "[error] ";   
                isColorUsed = true;
                break;
            case COLOR_PASS:
                prepStr = "[pass] ";   
                isColorUsed = true;
                break;
            default:    break;
            }

            if( isColorUsed )   cout<<_colorMod;
        }

        cout<<prepStr<<t_val;

        if( outFile.is_open() ) {
            outFile<<prepStr<<t_val;
        }

        return *this;
    }

    void SetColorWritten( bool t_written ) { _colorWritten = t_written; }


};

class ColorEndl {
public:
    ColorEndl();
    ~ColorEndl() {}

public:
    static ColorReverter* _rvt;
};

ColorCout& operator<<( ColorCout& t_os, const  ColorEndl& t_colorEndl);


extern ColorCout LogError;
extern ColorCout LogWarning;
extern ColorCout LogPass;
extern ColorCout LogMsg;
extern ColorEndl LogEndl;
