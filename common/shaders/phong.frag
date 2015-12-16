#version 400

in vec3 position_eye, normal_eye;

// uniform vec4 inputColor;
uniform vec3 light_pos_world;
uniform vec3 ls, ld, la;
uniform vec3 ks, kd, ka;
uniform float spl_exp;

uniform mat4 view;

out vec4 frag_color;

void main() {
	// fragColour = inputColor;
    vec3 Ia = la * ka;

    // diffusive reflectance
    vec3 light_pos_eye = vec3( view * vec4( light_pos_world, 1.0 ) );
    vec3 dir_surface_to_light_eye = normalize( light_pos_eye - position_eye );
    float dot_prod_diff = max( dot( dir_surface_to_light_eye, normal_eye ), 0.0 );
    vec3 Id = ld * kd * dot_prod_diff;

    // specular reflectance
    // blinn
    vec3 dir_surface_to_viewer_eye = normalize( - position_eye );
    vec3 halfway_eye = normalize( dir_surface_to_viewer_eye + dir_surface_to_light_eye );
    float dot_prod_spl =  max( dot( halfway_eye, normal_eye ), 0.0 );
    float spl_factor = pow( dot_prod_spl, spl_exp );

    vec3 Is = ls * ks * spl_factor;

    frag_color = vec4( Is + Id + Ia, 1.0 );
}
