varying vec4 vert_color;
varying vec4 vert_pos_view;
varying vec3 vert_normal_view;


void main(void)
{
//    gl_FragData[0] = vec4(normalize(vert_normal_view),1.0);

    gl_FragData[1].xyz = normalize(vert_normal_view).xyz;
    gl_FragData[1].w = 1.0;

    gl_FragData[2] = normalize(vert_pos_view);
}
