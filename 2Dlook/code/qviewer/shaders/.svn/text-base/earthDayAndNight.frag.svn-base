uniform sampler2D textureDay, textureNight;

varying float diffuse;
varying vec2 tex_coords;

void main(void)
{
  vec3 colorDay   = vec3( texture2D(textureDay  , tex_coords));
  vec3 colorNight = vec3( texture2D(textureNight, tex_coords));
  
  vec3 color = mix(colorNight, colorDay, diffuse);
  
  gl_FragColor = vec4(color, 1.0);
}
