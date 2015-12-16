/////////////////////////////////////////////////////////////////
//
//  Utilities - Uti_Math
//
//
//  Coyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////
#pragma  once 

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Utl_Global.h"

using glm::vec4;
using glm::vec3;
using glm::mat4;
using glm::mat3;

namespace Utl { 

extern const float g_Pi;
extern const float g_Pi2o;
extern const float g_o2Pi;


extern mat4 GetModelMatFromTfms( const vec3& t_Trans, const vec3& t_Rotat, const vec3& t_Scale );
extern bool SamePoints( const vec3& t_p1, const vec3& t_p2 );
extern bool AreParallel( const vec4& t_v1, const vec4& t_v2 );
extern bool ArePerpendicular(  const vec4& t_v1, const vec4& t_v2 );
extern mat4 ToMat4( const glm::mat3& t_mat );
extern glm::mat3 ToMat3( const mat4& t_mat );

// ray intersection
struct SRay {
    vec4 _Origin;
    vec4 _Dir;  // normalized direction vector

    SRay() {}
    SRay( const vec4& t_Origin, const vec4& t_Dir ) : _Origin( t_Origin ), _Dir( glm::normalize( t_Dir ) ) {}
    SRay( const vec3& t_Origin, const vec3& t_Dir ) : _Origin( t_Origin, 1.f ), _Dir( glm::normalize( t_Dir ), 0.f ) {}

    vec4 GetPointOnRay( const float& t_Scale ) const { return _Origin + t_Scale * _Dir; }
    SRay Transform( const mat4& t_TransMat ) const { return SRay( t_TransMat * _Origin, t_TransMat * _Dir ); }
    SRay Translate( const vec3& t_offset ) const { return SRay( vec4( t_offset, 0.f ) + _Origin, _Dir ); }
    SRay GetRefletedRay ( const vec4& t_InstPoint, const vec4& t_SurfaceNormal ) const {
        vec4 RflRayDir;
        vec4 NormalizedNormal = glm::normalize( t_SurfaceNormal );
        if( AreParallel( _Dir, NormalizedNormal ) ) RflRayDir = NormalizedNormal;
        else if( ArePerpendicular( _Dir, NormalizedNormal ) ) RflRayDir = _Dir;
        else RflRayDir = _Dir - 2.f* NormalizedNormal* ( glm::dot( _Dir, NormalizedNormal ) );

        return SRay( t_InstPoint, RflRayDir ); 
    }
};

}