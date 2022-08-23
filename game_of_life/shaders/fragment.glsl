#version 330 core

out vec4 fragCol;

flat in int vis;
in vec3 col;

void main()
{
    fragCol = vec4(col, 1.0) * float(vis);
}