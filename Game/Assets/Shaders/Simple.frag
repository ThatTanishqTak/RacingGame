#version 330 core

// Fragment shader output: the final color of the fragment
out vec4 FragColor;

void main()
{
    // Set the triangle color to an orange-ish tone
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
