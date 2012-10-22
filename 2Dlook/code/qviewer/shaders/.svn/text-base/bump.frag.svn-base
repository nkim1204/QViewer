uniform sampler2D textureDay;
uniform sampler2D textureBump;
uniform vec3 light_dir_world;

varying vec3 normalVec;
varying vec3 viewVec;
varying vec2 texCoords;

vec3 shade(vec3 N, vec3 L, vec3 E, 
			vec3 color, float Ka, float Kd, float Ks, 
			vec3 lightCol, float shininess)
{			
  vec3 final_color = color*Ka*lightCol;	//ambient
  
  float lambertTerm = dot(N,L);			//lambert
  
  if(lambertTerm > 0.0) {
    final_color += color*Kd*lambertTerm*lightCol; 	//diffuse
    
    vec3 halfVec = normalize(L+E);
    float specular = pow(max(dot(N,halfVec),0.0),shininess );
    
    final_color += color*Ks*lightCol*(specular);	//specular
  }
  
  return final_color;		
}

void main(void)
{
        vec4 fragment = texture2D( textureDay, texCoords );
        vec4 bumpVal = texture2D( textureBump, texCoords );

	float gradientX = length( dFdx( bumpVal ) )/abs( dFdx( viewVec.x ) );
	float gradientY = length( dFdy( bumpVal ) )/abs( dFdy( viewVec.y ) );
	vec3 NormalizedNormal = normalize( normalVec );
	vec3 nx = normalize( dFdx( NormalizedNormal ) );
	vec3 ny = normalize( dFdy( NormalizedNormal ) );
	vec3 NewNormal = normalize( NormalizedNormal + .0015*( gradientX*nx + gradientY*ny ) );
	
	vec3 NlightDir = normalize( light_dir_world );

	vec3 color = shade(NewNormal,NlightDir,viewVec,fragment.rgb,0.3,0.9,0.8,vec3(1.0,1.0,1.0),100.0);
	
        gl_FragColor = vec4(color ,1.0);
}
