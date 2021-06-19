#version 330 core
out vec4 out_color;
in vec3 frag_color_input;
void main() {
    // i could do something interesting, or i could be lazy, and i decided to go with lazy
    out_color = vec4(frag_color_input, 1.0);
}