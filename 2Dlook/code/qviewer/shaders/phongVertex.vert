uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;
uniform vec3 light_dir_world;

attribute vec4 color;
attribute vec4 vertex;
attribute vec3 normal;

varying vec4 smooth_color;

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

void main()
{
    gl_Position = projection_matrix * model_view_matrix * vertex;

    vec4 vert_pos_view  = normalize( model_view_matrix * vertex);
    vec3 vert_normal_view = normalize( normal_matrix * normal);

    vec3 light_color = vec3(1.0,1.0,1.0);

    smooth_color.rgb = shade(vert_normal_view,normalize(light_dir_world),vert_pos_view.xyz,color.xyz,0.4,0.9,2.0,light_color,50.0);
    smooth_color.a = 1.0;
}
