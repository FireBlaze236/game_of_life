#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 proj;
uniform vec4 ucol;

uniform vec3 positions[1000];
uniform int visible[1000];
uniform vec3 cols[1000];

flat out int vis;

out vec3 col;

void main()
{
    vec3 p = positions[gl_InstanceID];
    col = cols[gl_InstanceID];
    vis = visible[gl_InstanceID];

    gl_Position = proj * vec4(pos.x + p.x, pos.y + p.y, p.z + pos.z, 1.0);

    
}