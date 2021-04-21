#pragma once
#ifndef __CIRCLE_H__
#define __CIRCLE_H__

struct sphere_t
{
	uint	id;
	vec3	center;		// 2D position for translation
	float	radius=2.0f;		// radius
	mat4	model_matrix;		// modeling transformation

	// public functions
	void	update( float t);
};

inline std::vector<sphere_t> create_spheres(int num_spheres)
{
	std::vector<sphere_t> spheres;
	
	for (int k = 0; k < num_spheres; k++) {
		sphere_t s;
		vec3 center = vec3(float(k*100+0.1f), float(k * 0.1f), float(k * 0.1f));
		float radius = (float)(1.0f - 0.08 * k);
		s = { uint(k),center, radius};
		
		spheres.emplace_back(s);
	}
	

	return spheres;
}

inline void sphere_t::update( float t)
{
	//radius = 1.0f;		// simple animation
	//theta	= t;
	float rotation;
	float orbit_radius=4.0f;
	float position_x;
	float position_y;
	switch (id) {
	case 0:
		radius = 30.f;
		rotation = 0.3f;
		orbit_radius = 0.003f;
		position_x = 0;
		position_y = 0;
		break;
	case 1:
		radius = 2.51f;
		rotation = 0.15f;
		orbit_radius = 4.00f;
		position_x = 45;
		position_y = 30;
		break;
	case 2:
		radius = 1.91f;
		rotation = -0.1f;
		orbit_radius = 1.33f;
		position_x = 55;
		position_y = -20;
		break;
	case 3:
		radius = 4.f;
		rotation = 1.0f;
		orbit_radius = 1.0f;
		position_x = 70;
		position_y = 90;
		break;
	case 4:
		radius = 3.f;
		rotation = .9f;
		orbit_radius = 0.75f;
		position_x = 80;
		position_y = 40;
		break;
	case 5:
		radius = 10.f;
		rotation = 3.0f;
		orbit_radius = 0.3f;
		position_x = 100;
		position_y = 50;
		break;
	case 6:
		radius = 7.f;
		rotation = 2.0f;
		orbit_radius = 0.2f;
		position_x = 120;
		position_y = -40;
		break;
	case 7:
		radius =5.f;
		rotation = -1.5f;
		orbit_radius = 0.01f;
		position_x = 135;
		position_y = 20;
		break;
	case 8:
		radius = 6.f;
		rotation = 1.4f;
		orbit_radius = 0.05f;
		position_x = 165;
		position_y = -90;
		break;
	}
	//float c	= cos(theta), s=sin(theta);
	model_matrix = mat4::rotate(vec3(0, 0, 1), t*rotation)* mat4::translate(vec3(position_x , position_y, 0))*mat4::scale(vec3(radius,radius,radius));
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
