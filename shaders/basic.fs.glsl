#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 ObjectColor;
uniform sampler2D ourTexture;

void main()
{
    // FragColor = vec4(ObjectColor, 1.0f);
    FragColor = texture(ourTexture, TexCoord);
}