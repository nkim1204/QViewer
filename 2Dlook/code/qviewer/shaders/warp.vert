attribute vec2 vertex;
attribute vec2 texcoord;
uniform vec3 light_dir_world;

varying vec2 tex_coords;

void main()
{
    tex_coords=texcoord;
    gl_Position = vec4(vertex,0,0);
}
