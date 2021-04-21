#ifndef __TRACKBALL_H__
#define __TRACKBALL_H__
#include "cgmath.h"

struct trackball
{
	bool	b_tracking = false;
	int		button;
	int		mods;
	float	scale;			// controls how much rotation is applied
	mat4	view_matrix0;	// initial view matrix
	vec2	m0;				// the last mouse position

	trackball( float rot_scale=1.0f ) : scale(rot_scale){}
	bool is_tracking() const { return b_tracking; }
	void begin( const mat4& view_matrix, vec2 m );
	void end() { printf("end\n"); b_tracking = false; }
	mat4 update( vec2 m ) const;
	mat4 update_zoom(vec2 m) const;
	mat4 update_pan(vec2 m) const;
};

inline void trackball::begin( const mat4& view_matrix, vec2 m )
{
	printf("begin\n");
	b_tracking = true;			// enable trackball tracking
	m0 = m;						// save current mouse position
	view_matrix0 = view_matrix;	// save current view matrix
}

inline mat4 trackball::update( vec2 m ) const
{
	printf("update\n");
	// project a 2D mouse position to a unit sphere
	static const vec3 p0 = vec3(0,0,1.0f);	// reference position on sphere
	vec3 p1 = vec3(m-m0,0);					// displacement
	if( !b_tracking || length(p1)<0.0001f ) return view_matrix0;		// ignore subtle movement
	p1 *= scale;														// apply rotation scale
	p1 = vec3(p1.x,p1.y,sqrtf(std::max(0.0f,1.0f-length2(p1)))).normalize();	// back-project z=0 onto the unit sphere
	
	// find rotation axis and angle in world space
	// - trackball self-rotation should be done at first in the world space
	// - mat3(view_matrix0): rotation-only view matrix
	// - mat3(view_matrix0).transpose(): inverse view-to-world matrix
	vec3 v = mat3(view_matrix0).transpose()*p0.cross(p1);
	float theta = asin( std::min(v.length(),1.0f) );
	//printf("%lf %lf %lf %lf %lf %lf\n", view_matrix0._11, view_matrix0._12, view_matrix0._13, view_matrix0._21, view_matrix0._22, view_matrix0._23);
	// resulting view matrix, which first applies
	// trackball rotation in the world space
	//printf("x_axis: %lf\n", v.x);
	//printf("y_axis: %lf\n", v.y);

	return view_matrix0*mat4::rotate(v.normalize(),theta);
}

inline mat4 trackball::update_zoom(vec2 m) const
{
	printf("update_zoom\n");
	// project a 2D mouse position to a unit sphere
	static const vec3 p0 = vec3(0, 0, 1.0f);	// reference position on sphere
	vec3 p1 = vec3(m - m0, 0);					// displacement
	if (!b_tracking || length(p1) < 0.0001f) return view_matrix0;		// ignore subtle movement
	p1 *= scale;														// apply rotation scale
	p1 = vec3(p1.x, p1.y, sqrtf(std::max(0.0f, 1.0f - length2(p1)))).normalize();	// back-project z=0 onto the unit sphere
	
	// find rotation axis and angle in world space
	// - trackball self-rotation should be done at first in the world space
	// - mat3(view_matrix0): rotation-only view matrix
	// - mat3(view_matrix0).transpose(): inverse view-to-world matrix
	vec3 v = mat3(view_matrix0).transpose() * p0.cross(p1);
	float theta = asin(std::min(v.length(), 1.0f));
	/*view_matrix0.look_at(v, (view_matrix0._21, view_matrix0._22, view_matrix0._23)
		, (view_matrix0._31, view_matrix0._32, view_matrix0._33));*/
	//printf("%lf %lf %lf %lf %lf %lf\n", view_matrix0._11,view_matrix0._12,view_matrix0._13,view_matrix0._21, view_matrix0._22, view_matrix0._23);
	// resulting view matrix, which first applies
	// trackball rotation in the world space
	//printf("x_axis: %lf\n", p1.x);
	//printf("y_axis: %lf\n", p1.y);
	mat4 rebuild = view_matrix0;
	vec3 n_eyeminusat = vec3(view_matrix0._11, view_matrix0._12, view_matrix0._13);
	vec3 u_upcrossn = vec3(view_matrix0._21, view_matrix0._22, view_matrix0._23);
	vec3 v_ncrossu = vec3(view_matrix0._31, view_matrix0._32, view_matrix0._33);
	rebuild._34 += p1.y*150.0f;
	
	//rebuild = rebuild.transpose();
	//mat4 rebuild = view_matrix0*mat4::rotate(v.normalize(), theta);

	/*printf("%lf %lf %lf\n %lf %lf %lf\n %lf %lf %lf\n", 
		view_matrix0._11, view_matrix0._12, view_matrix0._13, 
		view_matrix0._21, view_matrix0._22, view_matrix0._23,
		view_matrix0._31, view_matrix0._32, view_matrix0._33);*/
	printf("% lf % lf % lf\n",view_matrix0._14, view_matrix0._24, view_matrix0._34);
	return rebuild;
}

inline mat4 trackball::update_pan(vec2 m) const
{
	printf("update_pan\n");
	// project a 2D mouse position to a unit sphere
	static const vec3 p0 = vec3(0, 0, 1.0f);	// reference position on sphere
	vec3 p1 = vec3(m - m0, 0);					// displacement
	if (!b_tracking || length(p1) < 0.0001f) return view_matrix0;		// ignore subtle movement
	p1 *= scale;														// apply rotation scale
	p1 = vec3(p1.x, p1.y, sqrtf(std::max(0.0f, 1.0f - length2(p1)))).normalize();	// back-project z=0 onto the unit sphere

	// find rotation axis and angle in world space
	// - trackball self-rotation should be done at first in the world space
	// - mat3(view_matrix0): rotation-only view matrix
	// - mat3(view_matrix0).transpose(): inverse view-to-world matrix
	vec3 v = mat3(view_matrix0).transpose() * p0.cross(p1);
	float theta = asin(std::min(v.length(), 1.0f));
	mat4 rebuild = view_matrix0;
	rebuild._14 += p1.x * 150.0f;
	rebuild._24 += p1.y * 150.0f;
	// resulting view matrix, which first applies
	// trackball rotation in the world space
	return rebuild;
}

// utility function
inline vec2 cursor_to_ndc( dvec2 cursor, ivec2 window_size )
{
	// normalize window pos to [0,1]^2
	vec2 npos = vec2( float(cursor.x)/float(window_size.x-1),
					  float(cursor.y)/float(window_size.y-1) );
	
	// normalize window pos to [-1,1]^2 with vertical flipping
	// vertical flipping: window coordinate system defines y from
	// top to bottom, while the trackball from bottom to top
	return vec2(npos.x*2.0f-1.0f,1.0f-npos.y*2.0f);
}

#endif // __TRACKBALL_H__
