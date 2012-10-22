uniform sampler2D colorMap;
uniform sampler2D depthMap;
uniform sampler2D normalMap;
uniform sampler2D posMap;
uniform sampler2D offsetMap;
uniform float depthThreshold, normThreshold, width, height;
uniform vec3 light_dir_world;


varying vec2 tex_coords;

vec3 shade(vec3 N, vec3 L, vec3 V,
           vec3 color, float Ka, float Kd, float Ks,
           vec3 lightCol, float shininess){

    vec3 final_color = color*Ka*lightCol;	//ambient

    float lambertTerm = dot(N,L);		//lambert

    if(lambertTerm > 0.0) {
        final_color += color*Kd*lambertTerm*lightCol; 	//diffuse

        vec3 R = reflect(L,N);
        float specular = pow(max(dot(R,V), 0.0), shininess);
        final_color += color*Ks*lightCol*(specular);	//specular
    }

    return final_color;
}

vec3 cellShade(float dotNL, vec3 color){
    float U;
    if(dotNL > 0.95)
        U=1.0;
    else if(dotNL > 0.5)
        U=0.5;
    else if(dotNL > 0.25)
        U=0.25;
    else
        U=0.1;
    return U*color;
}

void main(void)
{
    vec2 screenPos1 = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
    vec2 offset = texture2D(offsetMap,screenPos1).xy;
    vec2 screenPos = screenPos1 + offset;
    vec4 fragment = texture2D(colorMap, screenPos);
    vec4 depth = texture2D(depthMap, screenPos);
    vec4 normal = texture2D(normalMap, screenPos);
    vec4 pos = texture2D(posMap, screenPos);

    if(normal.w == 0){
        gl_FragColor = vec4(1.0);
        return;
    }


    // Phong Fragment
//    vec3 light_color = vec3(1.0,1.0,1.0);
//    vec4 temp;
//    temp.rgb = shade(normal.xyz, normalize(light_dir_world),pos.xyz,fragment.xyz,0.4,0.9,2.0,light_color,12.0);
//    temp.a = 1.0;


    // Toon Shading
    vec3 N = normal.xyz;
    float dotNL = dot(light_dir_world,N);
    vec4 temp = vec4(cellShade(dotNL,fragment.rgb), 1.0);


    // Silhouette
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

    vec4 temp2;

    temp2.rgb = temp.rgb * (((horizEdge.rgb * horizEdge.rgb) + (vertEdge.rgb * vertEdge.rgb)).r < depthThreshold ? vec3(1.0):vec3(0.0));
    temp2.a = 1.0;


    for (int i=0; i < 9; i++) {
        sample[i] = texture2D(normalMap, screenPos + c[i]);
    }


    // Laplacian
    //   -1 -1 -1
    //   -1  8 -1
    //   -1 -1 -1
    vec4 temp3 = (sample[4] * 8.0) -
                        (sample[0] + sample[1] + sample[2] +
                         sample[3] + sample[5] +
                         sample[6] + sample[7] + sample[8]);

//    gl_FragData[0].rgb = temp2.rgb * (((0.299*temp3.r+0.587*temp3.g+0.114*temp3.b)/3)/255 < normThreshold ? vec3(1.0):vec3(0.0));
//    gl_FragData[0].a = 1.0;

    gl_FragColor.rgb = temp2.rgb * (((0.299*temp3.r+0.587*temp3.g+0.114*temp3.b)/3)/255 < normThreshold ? vec3(1.0):vec3(0.0));
    gl_FragColor.a = 1.0;

}
