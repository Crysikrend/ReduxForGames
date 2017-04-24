#version 330 core
in vec2 newTexCoord;

out vec4 color;

uniform vec4 colour;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    //color = texture(ourTexture1, newTexCoord);
    color = mix(colour, mix(texture(ourTexture1, newTexCoord), texture(ourTexture2, newTexCoord), 0.2), 0.8);
}