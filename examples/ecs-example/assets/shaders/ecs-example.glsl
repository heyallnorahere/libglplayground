#shader vertex
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 _uv;
out vec2 uv;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    uv = _uv;
}
#shader fragment
#version 330 core
out vec4 out_color;
in vec2 uv;
uniform sampler2D tex;
void main() {
    out_color = texture(tex, uv);
}