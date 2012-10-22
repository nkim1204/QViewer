uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

attribute vec4 vertex;
attribute vec3 color;

varying vec4 smooth_color;

void main()
{
	smooth_color = vec4(color,1.0);
        gl_Position = projection_matrix * model_view_matrix * vertex;
}
