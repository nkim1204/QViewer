uniform vec3 light_dir_world;
uniform sampler2D textureGrayScale;

varying vec4 vert_color;
varying vec4 vert_pos_view;
varying vec3 vert_normal_view;

vec3 cellShade(float dotNL, vec3 color){
    float U = texture2D(textureGrayScale,vec2(dotNL,0.5)).r;
//    if(dotNL > 0.95)
//        U=1.0;
//    else if(dotNL > 0.5)
//        U=0.5;
//    else if(dotNL > 0.25)
//        U=0.25;
//    else
//        U=0.1;
    return U*color;
}


void main(void)
{
    vec3 N = normalize(vert_normal_view);
    float dotNL = dot(light_dir_world,N);
    gl_FragColor = vec4(cellShade(dotNL,vert_color.rgb), 1.0);
//    gl_FragColor = cellShade(dotNL,vert_color.rgb);
}