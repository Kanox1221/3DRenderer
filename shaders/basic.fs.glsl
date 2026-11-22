#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 ObjectColor;
uniform sampler2D ourTexture;

uniform bool showUV;
uniform bool useTexture;

in vec3 WorldNormal;
uniform bool showNormal;

void main()
{
    if (showUV) {
        FragColor = vec4(TexCoord, 0.0, 1.0);
    } 
    else if (useTexture){
        // FragColor = texture(ourTexture, TexCoord);
        vec4 texel = texture(ourTexture, TexCoord);

        if (texel.a < 0.5) {
            discard;
        }

        if (showNormal) {
            vec3 N = normalize(WorldNormal);
            FragColor = vec4(N * 0.5 + 0.5, 1.0);
        } else {
            float ambientStrength = 0.5;
            vec3 lightColor = vec3(1.0);  // White color

            vec3 ambient = ambientStrength * lightColor;
            vec3 result = ambient * texel.rgb;

            FragColor = vec4(result, 1.0);
        }
    }
    else {
        FragColor = vec4(ObjectColor, 1.0);
    }
}