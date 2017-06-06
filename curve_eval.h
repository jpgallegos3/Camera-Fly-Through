


// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:

	GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts);
	GsVec eval_lagrange(float t, float sn, const GsArray<GsVec>& ctrlpnts);
	GsVec eval_bspline(float t, int k, const GsArray<GsVec>& pnts);
	GsVec crspline(float t, const GsArray<GsVec>& pnts);
	GsVec bospline(float t, const GsArray<GsVec>& pnts);
	static float N(int i, int k, float t, const GsArray<float>& u);
	static float N(int i, int k, float u);
	float fact(float n);


