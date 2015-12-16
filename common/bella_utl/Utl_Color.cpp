/////////////////////////////////////////////////////////////////
//
//  Utilities - Color
//
//  Coyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////


#include "Utl_Color.h"

namespace Utl {

CColor::CColor( float t_R, float t_G, float t_B, float t_A )
{
    _Color = glm::clamp( vec4( t_R, t_G, t_B, t_A ), 0.f, 1.f );
}

CColor::CColor( const vec4& t_Vec )
{
    _Color = glm::clamp( t_Vec, 0.f, 1.f );
}

CColor::CColor( const vec3& t_Vec )
{
    _Color = vec4( glm::clamp( t_Vec, 0.f, 1.f ), 1.f );
}

CColor CColor::operator + (const CColor &t_Color) const
{
    return CColor( _Color + t_Color._Color );
}


CColor CColor::operator += (const CColor &t_Color)
{
    vec4 temp = _Color + t_Color._Color;
    _Color = glm::clamp( temp, 0.f,1.f );
    return *this;
}


CColor CColor::operator - (const CColor &t_Color) const
{
    return CColor( _Color - t_Color._Color );
}


CColor CColor::operator -= (const CColor &t_Color)
{
    vec4 temp = _Color - t_Color._Color;
    _Color = glm::clamp( temp, 0.f,1.f );
    return *this;
}

CColor CColor::operator * (const float &t_Scale ) const
{
    return CColor( _Color * t_Scale );
}

CColor CColor::operator * (const CColor &t_Color ) const
{
    return CColor( _Color * t_Color._Color );
}


CColor CColor::operator *= (const float& t_Scale )
{
    vec4 temp = _Color * t_Scale;
    _Color = glm::clamp( temp, 0.f, 1.f );

    return *this;
}

CColor CColor::CircleProduct(const CColor &t_Color) const
{
    return CColor( _Color * t_Color._Color );
}

}