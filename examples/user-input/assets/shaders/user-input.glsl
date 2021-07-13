#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
}
#shader fragment
#version 330 core
out vec4 frag_color;
void main() {
    frag_color = vec4(0.0, 1.0, 0.0, 1.0);
}