#version 330 core

out vec4 fragmentColor;

uniform ivec2 u_resolution;

void main() {
    vec2 uv = (2.0 * gl_FragCoord.xy - u_resolution.xy) / u_resolution.y;
    fragmentColor = vec4(uv, 0.0, 1.0);
}
