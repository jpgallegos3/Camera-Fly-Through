// Ensure the header file is included only once in multi-file projects
#ifndef SO_CAPSULE_H
#define SO_CAPSULE_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:

class Light
{
public:
	GsVec pos;
	GsColor amb, dif, spe; //ambient,diffuse, specular
	Light() {};
	Light(const GsVec& p, const GsColor& a, const GsColor& d, const GsColor& s)
		: pos(p), amb(a), dif(d), spe(s) {}
};

class Material
{
public:
	GsColor amb, dif, spe; //ambient,diffuse, specular
	float sh; // shininess
	Material() {};
	Material(const GsColor& a, const GsColor& d, const GsColor& s, float sn)
		: amb(a), dif(d), spe(s), sh(sn) {}
};

class SoCapsule : public GlObjects
{
private:
	GlShader _vshgou, _fshgou, _vshphong, _fshphong;
	GlProgram _proggouraud, _progphong;
	std::vector<GsPnt>   P; // coordinates
	std::vector<GsVec>   N; // normals
	int _numpoints;         // saves the number of points

public:
	std::vector<GsVec>   NL;// normal lines

public:

	void init();
	void build(float len, float rt, float rb, int nfaces, bool _flatn);
	void draw(const GsMat& tr, const GsMat& pr, const Light& l, const Material& m);
};




#endif // SO_CAPSULE_H#pragma once
