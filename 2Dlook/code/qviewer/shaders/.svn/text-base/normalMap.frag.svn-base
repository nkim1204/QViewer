uniform sampler2D textureDay;
uniform sampler2D textureNormals;
uniform vec3 light_dir_world;

varying vec3 eyeVec;
varying vec3 lightDir;
varying vec2 tex_coords;
varying vec3 tang;

vec3 shade(vec3 N, vec3 L, vec3 E, 
			vec3 color, float Ka, float Kd, float Ks, 
			vec3 lightCol, float shininess)
{			
  vec3 final_color = color*Ka*lightCol;	          //ambient
  
  float lambertTerm = dot(N,L);			  //lambert
  
  if(lambertTerm > 0.0) {
    final_color += color*Kd*lambertTerm*lightCol; //diffuse
    
    vec3 halfVec = normalize(L+E);
    float specular = pow(max(dot(N,halfVec),0.0),shininess );
    
    final_color = color*Ks*lightCol*(specular);  //specular
  }
  
  return final_color;		
}

void main (void)
{
  vec3 lVec = light_dir_world;//normalize(lightDir);//
  
  vec3 vVec = normalize(eyeVec);
  
  vec4 base = texture2D(textureDay, tex_coords);
  
  vec3 bump = normalize( texture2D(textureNormals, tex_coords).xyz * 2.0 - 1.0);
  
  vec3 color = shade(bump,lVec,vVec,base.rgb,0.3,0.5,2.0,vec3(1.0,1.0,1.0),5.0);
	
  gl_FragColor = vec4(color,1.0);
}
