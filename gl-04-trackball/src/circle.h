#pragma once
#ifndef __CIRCLE_H__
#define __CIRCLE_H__

struct sphere_t
{
	vec3	center;		// 2D position for translation
	float	radius=1.0f;		// radius
	mat4	model_matrix;		// modeling transformation

	// public functions
	void	update( float t );
};

inline std::vector<sphere_t> create_spheres(int num_spheres)
{
	std::vector<sphere_t> spheres;
	
	for (int k = 0; k < num_spheres; k++) {
		sphere_t s;
		vec3 center = vec3(float(k*0.1f), float(k * 0.1f), float(k * 0.1f));
		float radius = (float)(1.0f - 0.08 * k);
		s = { center, radius};
		
		spheres.emplace_back(s);
	}


	return spheres;
}

inline void sphere_t::update( float t )
{
	radius = 1.0f;		// simple animation
	//theta	= t;
	
	//float c	= cos(theta), s=sin(theta);
	model_matrix = mat4::rotate(vec3(0, 0, 1), 0);
	// these transformations will be explained in later transformation lecture
	/*mat4 scale_matrix =
	{
		radius, 0, 0, 0,
		0, radius, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	mat4 rotation_matrix =
	{
		c,-s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	mat4 translate_matrix =
	{
		1, 0, 0, center.x,
		0, 1, 0, center.y,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	
	model_matrix = translate_matrix*rotation_matrix*scale_matrix;*/
}

#endif
