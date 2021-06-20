#shader vertex
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
out vec3 frag_color_input;
void main() {
    gl_Position = vec4(pos, 1.0);
    frag_color_input = color;
}
#shader fragment
#version 330 core
out vec4 out_color;
in vec3 frag_color_input;
void main() {
    out_color = vec4(frag_color_input, 1.0);
}