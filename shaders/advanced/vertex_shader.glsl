#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vXY_ndc; // Normalized device coordinates [-1, 1] -> [0, 1]

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vec4 clipPos = projection * view * worldPos;
    gl_Position = clipPos;
    // Project to normalized device coordinates and map to [0, 1]
    vXY_ndc = (clipPos.xy / clipPos.w) * 0.5 + 0.5;
}
