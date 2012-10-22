uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

attribute vec4 vertex;

varying vec3 vert_pos_model;  // position in model space

void main() 
{
  vert_pos_model.x = clamp(vertex.x, 0., 1.);
  vert_pos_model.y = clamp(vertex.y, 0., 1.);
  vert_pos_model.z = clamp(vertex.z, 0., 1.);

  gl_Position = projection_matrix * model_view_matrix * vertex;
}
