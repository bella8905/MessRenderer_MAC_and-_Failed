#version 400

in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_normal;
out vec3 normal;

uniform mat4 view, proj, model;

void main() {
	gl_Position = proj * view * model * vec4( vertex_position, 1.0 );
    normal = vertex_normal;
}
