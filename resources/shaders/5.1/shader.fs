#version 330 core
out vec4 FragColor;

uniform sampler2D ourTexture;

in vec2 outTC;

void main()
{
    FragColor = texture(ourTexture, outTC);
}
