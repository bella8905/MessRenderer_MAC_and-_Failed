/////////////////////////////////////////////////////////////////
//
//  Utilities - Color
//
//  Coyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////



#pragma once

#include "Utl_Math.h"

namespace Utl{ 
    class CColor
    {
    public:
        CColor(void ) : _Color( 0.f, 0.f, 0.f, 1.f ) {} 
        CColor( float t_R, float t_G, float t_B, float t_A = 1.f );
        CColor( const vec4& t_Vec );
        CColor( const vec3& t_Vec );

        ~CColor(void) {}

    public:
        vec4 _Color;

    public:
        CColor operator + (const CColor &t_Color) const;
        CColor operator += (const CColor &t_Color);
        CColor operator - (const CColor &t_Color) const;
        CColor operator -= (const CColor &t_Color);
        CColor operator * ( const float& t_Scale) const;
        CColor operator *= ( const float& t_Scale);
        CColor operator * ( const CColor &t_Color ) const;
        CColor CircleProduct(const CColor &t_Color) const;
    };

}