

// Ensure the header file is included only once in multi-file projects
#ifndef SO_CURVE_H
#define SO_CURVE_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "so_capsule.h"
# include <gsim/gs_mat.h>
# include <gsim/gs_light.h>
# include <vector>

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:


class SoCurve : public GlObjects
{
private:
	GlShader _vshgou, _fshgou, _vshphong, _fshphong;
	GlProgram _proggouraud, _progphong;
	std::vector<GsVec>   P; // polyline coordinates
	std::vector<GsVec> N; // Normals
	std::vector<GsColor> C; // colors
	int _numpoints;
	bool z;

public:
	
	void init();
	void build(const GsArray<GsVec>& curve_pnts,bool z_pressed);
	void draw(const GsMat& tr, const GsMat& pr, const GsLight& l, const Material& m);
};

#endif // SO_CURVE_H
