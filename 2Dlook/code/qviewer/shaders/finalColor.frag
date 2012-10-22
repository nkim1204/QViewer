uniform float width, height;
uniform sampler2D colorMap;

void main(void)
{
    vec2 screenPos = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
    gl_FragColor = texture2D(colorMap,screenPos);
}
