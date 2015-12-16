/////////////////////////////////////////////////////////////////
//
//  Utilities - Uti_Math
//
//
//  Coyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Utl_Math.h"
#include "glm/gtx/transform.hpp"
#include <assert.h>

namespace Utl {

const float g_Pi = atan(1.0f) * 4.0f;
const float g_Pi2o = 180.0f / g_Pi;
const float g_o2Pi = g_Pi / 180.0f;



mat4 GetModelMatFromTfms( const vec3& t_Trans, const vec3& t_Rotat, const vec3& t_Scale )
{
    mat4 translationMat = glm::translate( t_Trans );
    mat4 rotationMat = glm::rotate( t_Rotat.x, glm::vec3(1,0,0) );
    rotationMat = rotationMat*glm::rotate(glm::mat4(), t_Rotat.y, glm::vec3(0,1,0) );
    rotationMat = rotationMat*glm::rotate(glm::mat4(), t_Rotat.z, glm::vec3(0,0,1) );
    mat4 scaleMat = glm::scale( t_Scale );
    return translationMat*rotationMat*scaleMat;
}

bool SamePoints( const vec3& t_p1, const vec3& t_p2 )
{
    return Equals( t_p1.x, t_p2.x ) && Equals( t_p1.y, t_p2.y ) && Equals( t_p1.z, t_p2.z );
}


bool AreParallel( const vec4& t_v1, const vec4& t_v2 )
{
    assert( Equals( t_v1.w, 0.f ) );
    if( Equals( t_v1.y * t_v2.z, t_v1.z * t_v2.y ) &&
        Equals( t_v1.z * t_v2.x, t_v1.x * t_v2.z ) && 
        Equals( t_v1.x * t_v2.y, t_v1.y * t_v2.x ) )
    {
        return true;
    }

    return false;
}

bool ArePerpendicular( const vec4& t_v1, const vec4& t_v2 )
{
    return Equals( glm::dot( t_v1, t_v2  ), 0.f );
}

mat4 ToMat4( const mat3& t_rot ) {
    mat4 rot(   vec4( t_rot[0], 0.f ),
                vec4( t_rot[1], 0.f ),
                vec4( t_rot[2], 0.f ),
                vec4( 0.f, 0.f, 0.f, 1.f) );
    return rot;
}

mat3 ToMat3(  const mat4& t_mat ) {
    mat3 rot = mat3(   vec3( t_mat[0] ),
                       vec3( t_mat[1] ),
                       vec3( t_mat[2] ) );
    return rot;

}

}