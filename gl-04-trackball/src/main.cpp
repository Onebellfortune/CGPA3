#include "cgmath.h"		// slee's simple math library
#include "cgut.h"		// slee's OpenGL utility
#include "trackball.h"	// virtual trackball
#include "circle.h"

//*************************************
// global constants
static const char*	window_name = "cgbase - trackball";
static const char*	vert_shader_path = "../bin/shaders/trackball.vert";
static const char*	frag_shader_path = "../bin/shaders/trackball.frag";

uint				NUM_TESS = 72;
uint				NUM_SPHERES = 9;
//*************************************
// common structures
struct camera
{
	vec3	eye = vec3( 300, 0, 0 );
	vec3	at = vec3( 0, 0, 0 );
	vec3	up = vec3( 0, 0, 1 );
	mat4	view_matrix = mat4::look_at( eye, at, up );

	float	fovy = PI/4.0f; // must be in radian
	float	aspect;
	float	dnear = 1.0f;
	float	dfar = 1000.0f;
	mat4	projection_matrix;
};

//*************************************
// window objects
GLFWwindow*	window = nullptr;
ivec2		window_size = cg_default_window_size(); // initial window size

//*************************************
// OpenGL objects
GLuint	program	= 0;	// ID holder for GPU program
GLuint	vertex_array = 0;	// ID holder for vertex array object

//*************************************
// global variables
int		frame = 0;				// index of rendering frames
bool	b_index_buffer = true;
bool	b_wireframe = false;
bool	b_solid_color = false;
bool	b_rotation_mode = false;
GLint	tc_xy = 0;
float theta = 0.0f;
float t0 = 0.0f;
auto	spheres = std::move(create_spheres(NUM_SPHERES));
//*************************************
// scene objects

camera		cam;
trackball	tb;
std::vector<vertex>	sun;	// host-side vertices

//*************************************
void update()
{
	// update projection matrix
	cam.aspect = window_size.x/float(window_size.y);
	cam.projection_matrix = mat4::perspective( cam.fovy, cam.aspect, cam.dnear, cam.dfar );

	// build the model matrix for oscillating scale
	float t = float(glfwGetTime());
	float scale	= 1.0f+float(cos(t*1.5f))*5.0f;
	mat4 model_matrix;

	// update uniform variables in vertex/fragment shaders
	GLint uloc;
	uloc = glGetUniformLocation(program, "b_solid_color");	if (uloc > -1) glUniform1i(uloc, b_solid_color);
	uloc = glGetUniformLocation(program, "tc_xy");	if (uloc > -1) glUniform1i(uloc, tc_xy);
	uloc = glGetUniformLocation( program, "view_matrix" );			if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, cam.view_matrix );
	uloc = glGetUniformLocation( program, "projection_matrix" );	if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, cam.projection_matrix );
	uloc = glGetUniformLocation( program, "model_matrix" );			if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, model_matrix );
}

void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// notify GL that we use our own program
	glUseProgram( program );

	// bind vertex array object
	glBindVertexArray(vertex_array);
	
	
	for (auto& s : spheres) {
		float t = (float)glfwGetTime();
		float delta_time = t - t0;
		s.update(t);
		theta += delta_time * 0.5f;
		t0 = t;
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, s.model_matrix);
		glDrawElements(GL_TRIANGLES, NUM_TESS * (NUM_TESS) * 3 , GL_UNSIGNED_INT, nullptr);
	}
	
	
	
	
		// render vertices: trigger shader programs to process vertex data
	
	
	
	
	// swap front and back buffers, and display to screen
	glfwSwapBuffers( window );
}

void reshape( GLFWwindow* window, int width, int height )
{
	// set current viewport in pixels (win_x, win_y, win_width, win_height)
	// viewport: the window area that are affected by rendering 
	window_size = ivec2(width,height);
	glViewport( 0, 0, width, height );
}

void print_help()
{
	printf( "[help]\n" );
	printf( "- press ESC or 'q' to terminate the program\n" );
	printf( "- press F1 or 'h' to see help\n" );
	printf( "- press Home to reset camera\n" );
	printf( "\n" );
}

void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if(action==GLFW_PRESS)
	{
		if(key==GLFW_KEY_ESCAPE||key==GLFW_KEY_Q)	glfwSetWindowShouldClose( window, GL_TRUE );
		else if(key==GLFW_KEY_H||key==GLFW_KEY_F1)	print_help();
		else if(key==GLFW_KEY_HOME)					cam=camera();
	}
	else if (key == GLFW_KEY_W)
	{
		b_wireframe = !b_wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, b_wireframe ? GL_LINE : GL_FILL);
		printf("> using %s mode\n", b_wireframe ? "wireframe" : "solid");
	}
	
}

void mouse( GLFWwindow* window, int button, int action, int mods )
{
	
	if(button==GLFW_MOUSE_BUTTON_LEFT)
	{
		dvec2 pos; glfwGetCursorPos(window,&pos.x,&pos.y);
		vec2 npos = cursor_to_ndc( pos, window_size );
		if(action==GLFW_PRESS)			tb.begin( cam.view_matrix, npos );
		else if(action==GLFW_RELEASE)	tb.end();
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = cursor_to_ndc(pos, window_size);
		if (action == GLFW_PRESS)			tb.begin(cam.view_matrix, npos);
		else if (action == GLFW_RELEASE)	tb.end();
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = cursor_to_ndc(pos, window_size);
		if (action == GLFW_PRESS)			tb.begin(cam.view_matrix, npos);
		else if (action == GLFW_RELEASE)	tb.end();
	}
	tb.button = button;
	tb.mods = mods;
}

void motion( GLFWwindow* window, double x, double y )
{
	if(!tb.is_tracking()) return;
	vec2 npos = cursor_to_ndc( dvec2(x,y), window_size );
	//cam.view_matrix = tb.update( npos );
	if (tb.button == GLFW_MOUSE_BUTTON_LEFT && tb.mods == 0)
		cam.view_matrix=tb.update(npos);
	else if (tb.button == GLFW_MOUSE_BUTTON_MIDDLE ||
		(tb.button == GLFW_MOUSE_BUTTON_LEFT && (tb.mods & GLFW_MOD_CONTROL)))
		cam.view_matrix=tb.update_pan(npos);
	else if (tb.button == GLFW_MOUSE_BUTTON_RIGHT ||
		(tb.button == GLFW_MOUSE_BUTTON_LEFT && (tb.mods & GLFW_MOD_SHIFT))) {
		cam.view_matrix=tb.update_zoom(npos);
		//printf("%lf %lf\n", npos.x, npos.y);
	}
		

	
}

void update_vertex_buffer(const std::vector<vertex>& vertices, uint N)
{
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer

	// clear and create new buffers
	if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
	if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;

	// check exceptions
	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return; }

	// create buffers
	if (b_index_buffer)
	{
		std::vector<uint> indices;
		uint k1, k2;
		for (uint k = 0; k <= N; k++)
		{
			k1 = k * (N+1);      // k1 -- k1+1
							 // |	/		
			k2 = k1 + N + 1; // k2 -- k2+1   

			for (uint j = 0; j <= N; j++)
			{
				k1++;
				k2++;
				if (k != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				
				if (k != N - 1) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
				

			}
		}

		// generation of vertex buffer: use vertices as it is
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// geneation of index buffer
		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}
	else
	{
		std::vector<vertex> v; // triangle vertices
		//printf("%lf\n", r);
		for (uint k = 0; k <= N * N; k++)
		{
			v.push_back(vertices.front());	// the origin
			v.push_back(vertices[k + 1]);
			v.push_back(vertices[k + 2]);
			//printf("%lf\n", v[k].pos.x);
			printf("vector");
		}


		/*for (uint j = 0; j <= N / 2; j++) {
			t = PI * 1.0f * k / float(N / 2), ct = cos(t), st = sin(t);

		}*/

		//t = PI * 1.0f * k / float(N / 2), ct = cos(t), st = sin(t);//theta=latitude.
		//v.push_back({ vec3(r * st * cp,r * st * sp,r * ct), vec3(st * cp,st * sp,cp), vec2(p / (2.0f * PI),1.0f - (t / PI)) });


	// generation of vertex buffer: use triangle_vertices instead of vertices
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * v.size(), &v[0], GL_STATIC_DRAW);
	}

	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	if (vertex_array) glDeleteVertexArrays(1, &vertex_array);
	vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }
}

std::vector<vertex> create_circle_vertices(uint N)
{
	std::vector<vertex> v = { { vec3(0,0,0), vec3(0,0,-1.0f), vec2(0.5f) } }; // origin
	//float r = circles[0].radius;
	float	r = 30.f;
	//printf("radius of circle 0: %lf\n", r);

	for (uint k = 0; k <= N/2 ; k++)
	{
		float t = PI * 2.0f * k / float(N), cost = cos(t), sint = sin(t);
		for (uint j = 0; j <= N; j++) {
			float p = PI * 2.0f * j / float(N), cosp = cos(p), sinp = sin(p);//theta=latitude
			v.push_back({ vec3(sint * cosp,sint * sinp,cost), vec3(sint * cosp,sint * sinp,cost), vec2(p / (2 * PI),1.0f - t / PI) });
		}

	}
	
	/*r = 2.51f;
	for (uint k = 0; k <= N/2 ; k++)
	{
		float t = PI * 2.0f * k / float(N), cost = cos(t), sint = sin(t);
		for (uint j = 0; j <= N; j++) {
			float p = PI * 2.0f * j / float(N), cosp = cos(p), sinp = sin(p);//theta=latitude
			v.push_back({ vec3(r * sint * cosp + 45,r * sint * sinp+30,r * cost), vec3(sint * cosp,sint * sinp,cost), vec2(p / (2 * PI),1.0f - t / PI) });
		}

	}
	r = 1.91f;
	for (uint k = 0; k <= N/2 ; k++)
	{
		float t = PI * 2.0f * k / float(N), cost = cos(t), sint = sin(t);
		for (uint j = 0; j <= N; j++) {
			float p = PI * 2.0f * j / float(N), cosp = cos(p), sinp = sin(p);//theta=latitude
			v.push_back({ vec3(r * sint * cosp + 55,r * sint * sinp-20,r * cost), vec3(sint * cosp,sint * sinp,cost), vec2(p / (2 * PI),1.0f - t / PI) });
		}

	}
	r = 4.f;
	for (uint k = 0; k <= N/2 ; k++)
	{
		float t = PI * 2.0f * k / float(N), cost = cos(t), sint = sin(t);
		for (uint j = 0; j <= N; j++) {
			float p = PI * 2.0f * j / float(N), cosp = cos(p), sinp = sin(p);//theta=latitude
			v.push_back({ vec3(r * sint * cosp+70,r * sint * sinp+90,r * cost), vec3(sint * cosp,sint * sinp,cost), vec2(p / (2 * PI),1.0f - t / PI) });
		}

	}
	r = 3.f;
	for (uint k = 0; k <= N/2; k++)
	{
		float t = PI * 2.0f * k / float(N), cost = cos(t), sint = sin(t);
		for (uint j = 0; j <= N; j++) {
			float p = PI * 2.0f * j / float(N), cosp = cos(p), sinp = sin(p);//theta=latitude
			v.push_back({ vec3(r * sint * cosp + 80,r * sint * sinp+40,r * cost), vec3(sint * cosp,sint * sinp,cost), vec2(p / (2 * PI),1.0f - t / PI) });
		}

	}
	r = 10.f;
	for (uint k = 0; k <= N/2; k++)
	{
		float t = PI * 2.0f * k / float(N), cost = cos(t), sint = sin(t);
		for (uint j = 0; j <= N; j++) {
			float p = PI * 2.0f * j / float(N), cosp = cos(p), sinp = sin(p);//theta=latitude
			v.push_back({ vec3(r * sint * cosp + 100,r * sint * sinp+50,r * cost), vec3(sint * cosp,sint * sinp,cost), vec2(p / (2 * PI),1.0f - t / PI) });
		}

	}

	r = 7.f;
	for (uint k = 0; k <= N/2; k++)
	{
		float t = PI * 2.0f * k / float(N), cost = cos(t), sint = sin(t);
		for (uint j = 0; j <= N; j++) {
			float p = PI * 2.0f * j / float(N), cosp = cos(p), sinp = sin(p);//theta=latitude
			v.push_back({ vec3(r * sint * cosp + 120,r * sint * sinp-40,r * cost), vec3(sint * cosp,sint * sinp,cost), vec2(p / (2 * PI),1.0f - t / PI) });
		}

	}
	r = 5.f;
	for (uint k = 0; k <= N/2; k++)
	{
		float t = PI * 2.0f * k / float(N), cost = cos(t), sint = sin(t);
		for (uint j = 0; j <= N; j++) {
			float p = PI * 2.0f * j / float(N), cosp = cos(p), sinp = sin(p);//theta=latitude
			v.push_back({ vec3(r * sint * cosp + 135,r * sint * sinp+20,r * cost), vec3(sint * cosp,sint * sinp,cost), vec2(p / (2 * PI),1.0f - t / PI) });
		}

	}
	r = 6.f;
	for (uint k = 0; k <= N/2; k++)
	{
		float t = PI * 2.0f * k / float(N), cost = cos(t), sint = sin(t);
		for (uint j = 0; j <= N; j++) {
			float p = PI * 2.0f * j / float(N), cosp = cos(p), sinp = sin(p);//theta=latitude
			v.push_back({ vec3(r * sint * cosp + 165,r * sint * sinp+90,r * cost), vec3(sint * cosp,sint * sinp,cost), vec2(p / (2 * PI),1.0f - t / PI) });
		}

	}*/
	return v;
}


bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests

	// load the mesh
	
	sun = std::move(create_circle_vertices(NUM_TESS));
	
	// create vertex buffer; called again when index buffering mode is toggled
	update_vertex_buffer(sun, NUM_TESS);
	


	return true;
}

void user_finalize()
{
}

int main( int argc, char* argv[] )
{
	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// version and extensions

	// initializations and validations
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!user_init()){ printf( "Failed to user_init()\n" ); glfwTerminate(); return 1; }					// user initialization

	// register event callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movement

	// enters rendering/event loop
	for( frame=0; !glfwWindowShouldClose(window); frame++ )
	{
		glfwPollEvents();	// polling and processing of events
		update();			// per-frame update
		render();			// per-frame render
	}

	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}


