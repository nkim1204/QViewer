uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

attribute vec4 vertex;
attribute vec2 texcoord;

varying vec2 tex_coords;

void main()
{
  tex_coords = texcoord;
  
  gl_Position = projection_matrix * model_view_matrix * vertex;
}
