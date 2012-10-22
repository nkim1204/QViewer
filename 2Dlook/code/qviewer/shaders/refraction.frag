const float eta = 1.0 / 1.44; 
const float F  = ((1.0-eta) * (1.0-eta)) / ((1.0+eta) * (1.0+eta));
const float FresnelPower = 5.0;

uniform samplerCube cubemap;

varying vec3 vertex_view;
varying vec3 normal_view;
varying vec3 I;
varying vec3 N;

void main(void)
{

  vec3 fragmentReflection = reflect(I, N);
  vec3 fragmentRefraction = refract(I, vec3(N.x, N.y, N.z), eta);
  if(fragmentRefraction.x==0 && fragmentRefraction.y==0 && fragmentRefraction.z==0)
    fragmentRefraction = cross(cross(N,I),N);

  vec4 environmentColor = textureCube(cubemap, fragmentReflection);

  vec4 refractionColor = textureCube(cubemap, fragmentRefraction);

  float fresnel = F + (1.0 - F) * pow((1.0 - dot(-vertex_view, normal_view)), FresnelPower);

  gl_FragColor = mix(refractionColor, environmentColor, fresnel);


}
