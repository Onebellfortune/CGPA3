#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

// input from vertex shader
in vec2 tc;

// the only output variable
out vec4 fragColor;

uniform int tc_xy;

void main()
{
	int key;
	if(tc_xy==0)
		fragColor=vec4(tc.xy,0,1);
	else if(tc_xy==1)
		fragColor = vec4(tc.xxx,1);
	else
		fragColor=vec4(tc.yyy,1);
	
}
