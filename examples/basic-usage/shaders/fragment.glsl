#version 330 core
out vec4 out_color;
in vec3 frag_color_input;
void main() {
    out_color = vec4(frag_color_input, 1.0);
}