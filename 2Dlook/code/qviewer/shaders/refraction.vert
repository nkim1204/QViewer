uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;
uniform vec4 camera_position;

attribute vec4 vertex;
attribute vec3 normal;


varying vec3 vertex_view;
varying vec3 normal_view;
varying vec3 I;
varying vec3 N;

void main(void)
{
  I = normalize(vertex.xyz - camera_position.xyz);
  N = normalize(normal);

  vertex_view = normalize((model_view_matrix*vertex).xyz);
  normal_view = normalize(normal_matrix*normal);

  gl_Position = projection_matrix*model_view_matrix*vertex;
}
