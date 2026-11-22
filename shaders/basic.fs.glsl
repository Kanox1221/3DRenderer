#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 ObjectColor;
uniform bool showUV;

void main()
{
    if (showUV) {
        FragColor = vec4(TexCoord, 0.0, 1.0);
    } else {
        FragColor = vec4(ObjectColor, 1.0);
    }
}