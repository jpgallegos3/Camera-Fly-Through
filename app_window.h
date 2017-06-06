
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_light.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_model.h"
# include "so_poly.h"
# include "curve_eval.h"
# include "so_curve.h"
# include "so_capsule.h"

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
	// GlShader _flatvsh, _flatfsh, _vertg, _fragg;
	// GlProgram _prog, _progg;

    // My scene objects:
    SoAxis _axis;
    SoModel _model;
	SoModel _plane;
	SoPoly _poly;
	SoPoly _poly1;
	SoPoly _poly2;
	SoPoly _poly3;
	SoCurve _curve1;
	SoCurve _curve2;
	SoCurve _curve3;
	SoCurve _curve4;

	int num_seg;

    // Scene data:
    bool  _viewaxis, _loop;
    GsModel _gsm;
    GsLight _light;
    
    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
	int _w, _h, i;

	GsVec _trans, AB, AB_norm;;       // scene transformation data
	GsMat _str;         // scene transformation
    
	Material _mat;

	GsVec _eye, _center, _up, my_eye, my_center; // camera data
	GsMat _cam;               // camera transformation
	float _aspect, _znear, _zfar;// perspective projection data
	GsMat _proj;              // projection transformation
	GsMat _spr, stransf, sproj;               // final scene projection _spr=_cam*_proj
	GsArray<GsVec> V;
	GsArray<GsVec> V1;
	GsArray<GsPnt> P1;
	GsArray<GsVec> V2;
	GsArray<GsPnt> P2;
	GsArray<GsVec> VP;  //plane vertices
	GsArray<GsVec> IVP;  //inner plane vertices

	float _pickprec, length, angle_, curve_z, _angle, _plane_cam_x, _plane_cam_z;
	
	bool _oktodrag, _L, _B, _BS, _CR, _BO, _NC, z_pressed, x_pressed, _motion, c_pressed, _for, _down, g_pressed, _camera1, _camera2, _camera3, _view_curves;
	

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    void loadModel ( int model );
    GsVec2 windowToScene ( const GsVec2& v );
	GsVec rayXYintercept(const GsVec2& v);

   private : // functions derived from the base class
	   virtual void glutIdle();
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	void loop();
 };

#endif // APP_WINDOW_H
