#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 _uv;
// todo: add more fields for advanced lighting; though for now, we only need this
layout(location = 3) in ivec4 bone_ids;
layout(location = 4) in vec4 weights;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 bones[100]; // 100 max
out vec2 uv;
mat4 get_bone_transform() {
    mat4 matrix = bones[bone_ids[0]] * weights[0];
    matrix += bones[bone_ids[1]] * weights[1];
    matrix += bones[bone_ids[2]] * weights[2];
    matrix += bones[bone_ids[3]] * weights[3];
    return matrix;
}
void main() {
    mat4 bone_transform = get_bone_transform();
    gl_Position = projection * view * model * bone_transform * vec4(position, 1.0);
    uv = _uv;
}
