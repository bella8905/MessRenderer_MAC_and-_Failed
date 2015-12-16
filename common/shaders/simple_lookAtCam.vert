#version 400

in vec3 vertex_position;
uniform mat4 view, proj, model;

void main() {
	gl_Position = proj * view * model * vec4( vertex_position, 1.0 );
    // gl_Position = vec4( vertex_position, 1.0 );
}
