uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;

attribute vec4 vertex;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec3 VertexTangent;
attribute vec3 VertexBinormal;

varying vec3 normalVec;
varying vec3 viewVec;
varying vec2 texCoords;

float bumpScale = 0.4;

void main()
{
	vec4 vertPosView = -normalize(model_view_matrix * vertex);
	vec3 normalView = normalize( normal_matrix * normal);

	normalVec = normalView;
	viewVec = vertPosView.xyz;

	gl_Position = projection_matrix *model_view_matrix* vertex;
	texCoords=texcoord;
}
