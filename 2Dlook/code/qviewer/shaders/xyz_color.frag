varying vec3 vert_pos_model;  // position in model space

void main()
{
  gl_FragColor = vec4(vert_pos_model.x,
		   vert_pos_model.y, 
		   vert_pos_model.z, 1.0);
}
