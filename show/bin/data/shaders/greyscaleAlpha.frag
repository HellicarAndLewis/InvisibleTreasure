uniform sampler2DRect tex0;

void main( void )
{  
	vec2 st0 = gl_TexCoord[0].st;
	vec4 color0 = vec4(texture2DRect(tex0, st0));
	float alpha = 1.0;
	if (color0.r == 0.0	&& color0.g == 0.0	&& color0.b == 0.0) {
		alpha = 0.0;
	}
	gl_FragColor = vec4(color0.r, color0.g, color0.b, alpha);
}