uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

attribute vec4 vertex;

varying vec3 pm;  // position in model space

void main() 
{
  pm = vertex.xyz * 120.;

  gl_Position = projection_matrix * model_view_matrix * vertex;
}
