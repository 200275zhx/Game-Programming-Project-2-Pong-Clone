#version 460

in vec4 vertexColor;
in vec2 texCoordVar;

uniform sampler2D diffuse;
uniform bool use_texture;

out vec4 fragColor;

void main() {
    if (use_texture) { fragColor = texture(diffuse, texCoordVar); } 
    else { fragColor = vertexColor; }
}
