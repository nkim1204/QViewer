uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;
uniform vec3 light_dir_world;

attribute vec4 color;
attribute vec4 vertex;
attribute vec3 normal;

varying vec4 vert_color;
varying vec4 vert_pos_view;
varying vec3 vert_normal_view;

void main()
{

  gl_Position = projection_matrix * model_view_matrix * vertex;

  vert_color = color;
  vert_pos_view  = normalize( model_view_matrix * vertex);
  vert_normal_view = normalize( normal_matrix * normal);
}
