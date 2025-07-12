#version 330 core

// Vertex attribute: position of the vertex (x, y, z)
layout (location = 0) in vec3 aPos;

void main()
{
    // gl_Position is a built-in output that takes a vec4
    gl_Position = vec4(aPos, 1.0);
}