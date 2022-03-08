#version 450 core

in vec2 vUV;

uniform sampler2D uColorTexture;

out vec4 oColor;

void main()
{
    oColor = texture(uColorTexture, vUV);
}