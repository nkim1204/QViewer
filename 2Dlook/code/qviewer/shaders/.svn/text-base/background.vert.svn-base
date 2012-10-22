uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

attribute vec4 vertex;

varying vec3 view_position;

void main(void)
{
  view_position = vec3(vertex.x,vertex.y,vertex.z);
  
  gl_Position = projection_matrix * model_view_matrix * vertex;
}
