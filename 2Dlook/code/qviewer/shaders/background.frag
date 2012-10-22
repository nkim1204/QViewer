uniform samplerCube cubemap;

varying vec3 view_position;

void main(void)
{
        vec4 environmentColor = textureCube(cubemap, view_position);
	
        gl_FragColor = environmentColor;
}
