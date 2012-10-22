uniform sampler2D colorMap;
uniform sampler2D depthMap;
uniform float threshold, width, height;

varying vec2 tex_coords;

void main(void)
{
    vec2 screenPos = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
    vec4 fragment = texture2D(colorMap, screenPos);
    vec4 depth = texture2D(depthMap, screenPos);
    
    vec2 c[9];
    float incrX = 1.0/width;
    float incrY = 1.0/height;
    c[0] = vec2(-incrX,incrY);
    c[1] = vec2( 0.0  ,incrY);
    c[2] = vec2( incrX,incrY);
    c[3] = vec2(-incrX,0.0);
    c[4] = vec2( 0.0,0.0);
    c[5] = vec2( incrX,incrY);
    c[6] = vec2(-incrX,-incrY);
    c[7] = vec2( 0.0,-incrY);
    c[8] = vec2( incrX,-incrY);

    vec4 sample[9];
    
    for (int i=0; i < 9; i++) {
        sample[i] = texture2D(depthMap, screenPos + c[i]);
    }
    
    // Sobel filter
    //    -1 -2 -1       1 0 -1
    // H = 0  0  0   V = 2 0 -2
    //     1  2  1       1 0 -1
    //
    // result = sqrt(H^2 + V^2)

    vec4 horizEdge = sample[2] + (2.0*sample[5]) + sample[8] -
            (sample[0] + (2.0*sample[3]) + sample[6]);

    vec4 vertEdge = sample[0] + (2.0*sample[1]) + sample[2] -
            (sample[6] + (2.0*sample[7]) + sample[8]);

    gl_FragColor.rgb = fragment.rgb * (((horizEdge.rgb * horizEdge.rgb) + (vertEdge.rgb * vertEdge.rgb)).r < threshold ? vec3(1.0):vec3(0.0));
    gl_FragColor.a = 1.0;
}
