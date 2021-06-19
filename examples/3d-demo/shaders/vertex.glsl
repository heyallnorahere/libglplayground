#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
out vec3 frag_color_input;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    frag_color_input = color;
}