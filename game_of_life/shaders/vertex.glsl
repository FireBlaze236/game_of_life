#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 aOffset;
layout(location = 2) in vec3 aCol;

uniform mat4 proj;
uniform vec4 ucol;


flat out int vis;

out vec3 col;

void main()
{
    vis = int(aOffset.z);
    col = aCol;
    gl_Position = proj * vec4(pos.xy + aOffset.xy, 0.0, 1.0);
}