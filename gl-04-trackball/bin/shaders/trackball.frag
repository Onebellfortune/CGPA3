#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

// input from vertex shader
in vec2 tc;
in vec3 norm;

// the only output variable
out vec4 fragColor;
uniform int tc_xy;

void main()
{
int key;
	fragColor=vec4(tc.xy,0,1);
	
}
