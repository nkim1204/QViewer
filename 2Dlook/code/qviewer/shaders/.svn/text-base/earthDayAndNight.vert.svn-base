uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;
uniform vec3 light_dir_world;

attribute vec4 vertex;
attribute vec3 normal;
attribute vec2 texcoord;

varying vec2 tex_coords;
varying float diffuse;

void main()
{	
  vec3 lv, nv;

  nv  = normalize( normal_matrix * normal);
  
  vec3 pv = normalize(model_view_matrix * vertex).xyz;

  // center of model space in view space
  vec3 ov = (model_view_matrix * vec4(0.,0.,0.,1.)).xyz;

  diffuse = max(dot(light_dir_world, nv), 0.0);

  tex_coords = texcoord;

  gl_Position = projection_matrix * model_view_matrix * vertex;
}
