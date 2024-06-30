#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform bool use_texture;

out vec4 vertexColor;
out vec2 texCoordVar;

void main() {
    // set position
    vec4 pos = vec4(position, 1.0);  // Convert to 4-component vector
    gl_Position = projection_matrix * view_matrix * model_matrix * pos;

    // set color
    if (use_texture) { texCoordVar = texCoord; } 
    else { vertexColor = color; }
}