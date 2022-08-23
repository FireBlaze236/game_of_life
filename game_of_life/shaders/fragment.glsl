#version 330 core

out vec4 fragCol;

flat in int vis;

void main()
{
    fragCol = vec4(0.0, 1.0, 0.0, 1.0) * float(vis);
}