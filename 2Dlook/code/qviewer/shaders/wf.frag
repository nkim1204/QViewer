uniform float width, height;
uniform float t;

const float PI = 3.141592653589793238462643383279;

void main(void)
{
    vec2 screenPos = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
//    gl_FragData[0] = vec4(cos(t*PI*screenPos.x) + sin(t*PI*screenPos.y),sin(t*PI*screenPos.y) + cos(t*PI*screenPos.x),0,0);
    screenPos.x = t*cos(2*PI*(screenPos.x+t));
    screenPos.y = 0*sin(2*PI*(screenPos.y+t*0));


    gl_FragData[0] = vec4(screenPos.x,screenPos.y,0,0);
}
