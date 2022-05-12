#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

uniform mat4 transform;

out vec2 tex_coords;

void main() {
  gl_Position = transform * vec4(pos, 1.0f);
  tex_coords = tex;
}
