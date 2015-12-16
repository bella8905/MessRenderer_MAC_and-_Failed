#version 400

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_normal;

uniform mat4 view, proj, model;
out vec3 position_eye, normal_eye;

void main() {
    position_eye = vec3( view * model * vec4( vertex_position, 1.0 ) );
    // don't forget to normalize normals, we might resize the model by model matrix
    normal_eye = normalize( vec3( view * model * vec4( vertex_normal, 0.0 ) ) );
	gl_Position = proj * view * model * vec4( vertex_position, 1.0 );
}
