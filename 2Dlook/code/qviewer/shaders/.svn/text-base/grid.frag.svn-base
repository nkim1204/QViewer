varying vec3 pm;  // position in model space

void main()
{
	
	vec4 blanc = vec4(1.0,1.0,1.0,1.0);
	vec4 noir  = vec4(0.0,0.0,0.0,1.0);

	float coefx = (1.+cos(pm.x))/2.;
	coefx = pow(coefx, 5.);
	//float coefx = step(0.7,fract(pm.x/8.0));
	vec4 colorx = mix(blanc, noir, coefx);

	float coefy = (1.+cos(pm.y))/2.;
	coefy = pow(coefy, 5.);
	//float coefy = step(0.7,fract(pm.y/8.0));
	vec4 colory = mix(blanc, noir, coefy);
  
        gl_FragColor = vec4(vec3(min(colorx, colory)),1.);
}
