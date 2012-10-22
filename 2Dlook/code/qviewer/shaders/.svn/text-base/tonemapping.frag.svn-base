uniform sampler2D colorMap;
uniform float width, height;
uniform float gamma, exposure;

varying vec2 tex_coords;

void main(void)
{
    vec2 screenPos = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
    vec4 pixel = texture2D(colorMap, screenPos);
    pixel.rgb *= exposure;
    gl_FragColor = pow(pixel, vec4(gamma));
}
