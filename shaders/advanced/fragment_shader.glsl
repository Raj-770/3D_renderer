#version 330 core
in vec2 vXY_ndc;
out vec4 FragColor;

uniform float uTime;

// Corner colors: lower left, lower right, upper left, upper right
const vec3 colorLL = vec3(1.0, 0.0, 0.0); // Red
const vec3 colorLR = vec3(0.0, 1.0, 0.0); // Green
const vec3 colorUL = vec3(0.0, 0.0, 1.0); // Blue
const vec3 colorUR = vec3(1.0, 1.0, 0.0); // Yellow

void main()
{
    // Animate blend between color corners with time
    float anim = 0.5 + 0.5 * sin(uTime);

    // Bilinear interpolation for 2D gradient
    vec3 bottom = mix(colorLL, colorLR, vXY_ndc.x);
    vec3 top = mix(colorUL, colorUR, vXY_ndc.x);
    vec3 gradient = mix(bottom, top, vXY_ndc.y);

    // Animate gradient using a sine wave (makes the gradient slide up and down over time)
    float slide = 0.25 * sin(uTime + 6.2831*vXY_ndc.x);
    float yMod = clamp(vXY_ndc.y + slide, 0.0, 1.0);
    vec3 animatedGradient = mix(bottom, top, yMod);

    // Center "vignette" fade for focus effect
    float dist = distance(vXY_ndc, vec2(0.5));
    float vignette = smoothstep(0.5, 0.25, dist);

    // Final color mixes animated and static gradients
    vec3 color = mix(gradient, animatedGradient, anim) * vignette;
    FragColor = vec4(color, 1.0);

    // Optional: Dotted effect for thick lines (uncomment for dash)
    // if (int(gl_FragCoord.x / 8.0 + gl_FragCoord.y / 8.0) % 2 == 0)
    //     discard;
}
