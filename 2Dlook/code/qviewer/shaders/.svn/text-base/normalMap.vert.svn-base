uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;
uniform vec3 light_dir_world;

attribute vec4 vertex;
attribute vec2 texcoord;
attribute vec3 normal;
attribute vec3 tangent;

varying vec3 eyeVec;
varying vec3 lightDir;
varying vec2 tex_coords;
varying vec3 tang;

void main(void)
{
  gl_Position = projection_matrix *model_view_matrix* vertex;
  tex_coords=texcoord;
  tex_coords.y=-texcoord.y;

  /*  vec3 c1 = cross(normal, vec3(0.0,0.0,1.0));
  vec3 c2 = cross(normal, vec3(0.0,1.0,0.0));

  vec3 tangent;
  if(length(c1)>length(c2)){
    tangent = c1;
  }else{
    tangent = c2;
  }

  tangent = normalize(tangent);*/

  vec3 n = normalize(normal_matrix * normal);
  vec3 t = normalize(normal_matrix * tangent);
  vec3 b = normalize(normal_matrix * normalize(cross(normal, tangent)));
	
  vec3 vVertex = vec3(model_view_matrix * vertex);

  eyeVec.x = dot(vVertex, t);
  eyeVec.y = dot(vVertex, b);
  eyeVec.z = dot(vVertex, n);

  lightDir.x = dot(light_dir_world, t);
  lightDir.y = dot(light_dir_world, b);
  lightDir.z = dot(light_dir_world, n);

  tang = tangent;//normalize(normal_matrix * vec3(-vertex.y,vertex.x,0));
}
