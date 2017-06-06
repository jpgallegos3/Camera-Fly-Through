
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"
# include "math.h"
# define M_PI 3.1415926535897932384626433832795

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = false;
   _fovy = GS_TORAD(60.0f);
   _rotx = _roty = 0;
   _w = w;
   _h = h;

   _pickprec = 0.035f;
   _poly.select(-1, _pickprec);
   _poly1.select(1, _pickprec);
   _oktodrag = false;

   _eye.set(0, 1, 2);
   _center.set(0, 0, 0);
   my_center.set(1000, 1000, 1000);

   _up.set(0, 1, 0);
   _aspect = 1.0f; _znear = 0.1f; _zfar = 50.0f;
   num_seg = 1000;
   _mat = Material(GsColor::lightblue, GsColor(0.8f, 0.8f, 0.8f, 1.0f), GsColor::white, 8);
   
   i = 0;
   _L = false;
   _B = false;
   _BS = false;
   _CR = false;
   _BO = false;
   _NC = true;
   z_pressed = false;
   x_pressed = false;
   _motion = false;
   c_pressed = false;
   g_pressed = false;
   _for = true;
   _down = false;
   _angle = 0.0f;
   _plane_cam_x = 0.0f;
   _plane_cam_z = 0.0f;
   _camera1 = false;
   _camera2 = false;
   _camera3 = false;
   _view_curves = false;
}
void AppWindow::glutIdle() {
	_angle -= (M_PI / 1000);
	_plane_cam_x = 0.85*cos(_angle);
	_plane_cam_z = 0.85*sin(_angle);
	GsVec plane_cam = GsVec(_plane_cam_x, 1.1f, _plane_cam_z);
	GsVec plane_center = GsVec(0.0f, 0.0f, 0.0f);
	if (_camera2) {
		my_eye = plane_cam;
		my_center = plane_center;
	}

	if (_camera1) {
		if (V1.size() > 1 && i < V1.size()) {
			my_eye = V1[i];
		    my_center = V1[i + 1];

		}
	}

	if (_camera3) {
		if (V2.size() > 1 && i < V2.size()) {
			my_eye = V2[i];
			my_center = V2[i + 1];
		}
	}

	if (i >= V1.size() || i > V2.size()) {
		c_pressed = false;
	}
	redraw();
}

void AppWindow::initPrograms ()
 {

	 //Fixed Poly control points 1
	 P1.push(GsVec(1.07984f, 1.27098f,-0.5f));

	 P1.push(GsVec(1.05494f, 1.03154f, -0.13f));

	 P1.push(GsVec(0.997812f, 0.83062f, 0.24f));

	 P1.push(GsVec(0.633535f, 0.804795f, -0.24f));

	 P1.push(GsVec(0.146402f, 0.629696f, 0.57f));

	 P1.push(GsVec(-0.107022f, 0.64614f, 0.0f));

	 P1.push(GsVec(-0.146308f, 0.304491f, -1.0f));

	 P1.push(GsVec(-0.575228f,0.156787f, -0.86999f));

	 P1.push(GsVec(-0.83642f, 0.237634f, 0.79f));

	 // Init my scene objects:
	
	 _poly.init( GsColor::darkblue, GsColor::darkred);
	 _poly1.init(GsColor::darkblue, GsColor::darkred);
	 for (int i = 0; i < P1.size(); i++) {
		 _poly1.add(P1[i]);
	 }
	 
	 _curve1.init();
	 //Fixed Poly control points 2
	 P2.push(GsVec(1.32595f, 0.30561f, 0.45f));

	 P2.push(GsVec(0.628893f, 0.205166f, 0.09f));

	 P2.push(GsVec(0.227416f, 0.24384f, 0.0f));
	 
	 P2.push(GsVec(0.148391f, 0.152708f, 0.0f));

	 P2.push(GsVec(0.0592967f, 0.1476521f, 0.0f));

	 P2.push(GsVec(-0.00282604f, 0.1639634f, -0.75f));

	 P2.push(GsVec(-0.104194f, 0.12691f, -0.8f));

	 P2.push(GsVec(-0.348054f, 0.194051f, 1.06f));

	 P2.push(GsVec(-0.48508f, 0.224691f, 1.11f));

	 P2.push(GsVec(-0.863508f,0.217885f, 0.0f));

	 _poly2.init(GsColor::darkblue, GsColor::darkgreen);

	 for (int i = 0; i < P2.size(); i++) {
		 _poly2.add(P2[i]);
	 }

	 _curve2.init();
	 _curve3.init(); //plane curve
	 _curve4.init(); //inner plane curve
   // Init my scene objects:
   _axis.init ();
   _model.init ();
   
   _plane.init();
   
   // set light:
   _light.set ( GsVec(1,1,10), GsColor(90,90,90,255), GsColor::white, GsColor::white );

   // Load demo model:
   loadModel(1);
   _model.build(_gsm);

   loadModel(2);
   _plane.build(_gsm);

   redraw();

 }

static void printInfo ( GsModel& m )
 {
   std::cout<<"V:  "<<m.V.size()<<"\n";
   std::cout<<"F:  "<<m.F.size()<<"\n";
   std::cout<<"N:  "<<m.N.size()<<"\n";
   std::cout<<"M:  "<<m.M.size()<<"\n";
   std::cout<<"Fn: "<<m.Fn.size()<<"\n";
   std::cout<<"Fm: "<<m.Fm.size()<<"\n";
 }

void AppWindow::loadModel ( int model )
 {
   float f;
   GsString file;
   switch ( model )
    { case 1: f=0.0025f; file="../models/project2buildsv4.obj"; break;
      case 2: f = 0.01; file = "../models/MQ-9.obj"; break; 
	 
      default: return;
    }
   std::cout<<"Loading "<<file<<"...\n";
   if ( !_gsm.load ( file ) ) std::cout<<"Error!\n";
   //printInfo ( _gsm );
   _gsm.scale ( f ); // to fit our camera space
   
   
 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
   switch ( key )
    { case ' ': _viewaxis = !_viewaxis; redraw(); break;
	  //case 27 : exit(1); // Esc was pressed
      /*case 's' : std::cout<<"Smoothing normals...\n";
                _gsm.smooth ( GS_TORAD(35) ); 
                 printInfo(_gsm);
                 _model.build(_gsm); 
                 redraw(); 
                 break;*/
      /*case 'f' : std::cout<<"Flat normals...\n";
                _gsm.flat();
                 printInfo(_gsm);
                 _model.build(_gsm); 
                 redraw(); 
                 break;*/
      /*case 'p' : if ( !_model.phong() )
                  { std::cout<<"Switching to phong shader...\n";
                    _model.phong(true);
                  }
                 redraw(); 
                 break;*/
      /*case 'g' : if ( _model.phong() )
                  { std::cout<<"Switching to gouraud shader...\n";
                    _model.phong(false);
                  }
                 redraw(); 
                 break;*/
      //default : loadModel ( int(key-'0') );
                //break;
	
   //case 'a':
	   //std::cout << "num_seg = " << num_seg << std::endl;
	   //if (num_seg == 2) { break; }
	   //else { num_seg -= 2; }; redraw(); break;
   //case 'q': num_seg += 2; redraw(); break;
   //case'0': _NC = true; _L = false; _B = false; _BS = false; _CR = false;  _BO = false; c_pressed = false; redraw(); break;
   //case'1': _NC = false; _L = true; _B = false; _BS = false; _CR = false;  _BO = false; c_pressed = false; redraw(); break;
   //case'2': _NC = false; _L = false; _B = true; _BS = false; _CR = false;  _BO = false; c_pressed = false; redraw(); break;
   //case'3': _NC = false; _L = false; _B = false; _BS = true; _CR = false;  _BO = false; c_pressed = false; redraw(); break;
	case'1': i = 0; _camera1 = true; _camera2 = false; _camera3 = false; std::cout << "Selected Camera 1: BSpline Cruve 1" << std::endl;  redraw(); break;
	case'2': i = 0; _camera1 = false; _camera2 = true; _camera3 = false; std::cout << "Selected Camera 2: Plane Curve" << std::endl; redraw(); break;
	case'3':  i = 0; _camera1 = false; _camera2 = false; _camera3 = true;  std::cout << "Selected Camera 3: BSpline Cruve 2" << std::endl; redraw(); break;
   //case'4': _NC = false; _L = false; _B = false; _BS = false; _CR = true;  _BO = false; c_pressed = false; redraw(); break;
   //case'5': _NC = false; _L = false; _B = false; _BS = false; _CR = false;  _BO = true; c_pressed = false; redraw(); break;
   case'c': c_pressed = !c_pressed; i = 0; std::cout << "Beginging camera" << std::endl; redraw(); break;
   case'v': _view_curves = !_view_curves; redraw(); break;
   //case'z': z_pressed = !z_pressed; redraw(); break;
   //case'x': x_pressed = !x_pressed; redraw(); break;
   //case'f': _for = true; _down = false; g_pressed = false; std::cout << "for" << std::endl; redraw(); break;
   //case'd': _for = false; _down = true; g_pressed = false; std::cout << "down" << std::endl; redraw(); break;
   //case'g': _for = false; _down = false; g_pressed = true; std::cout << "center" << std::endl; redraw(); break;
   //case'u': for (int i = 0; i < _poly.num_vertices(); i++) {
	   //std::cout << _poly.vertexarray().get(i).x << "," << _poly.vertexarray().get(i).y << "," << _poly.vertexarray().get(i).z << std::endl;

   //} std::cout << std::endl; break;

   /*case 127: // Del pressed
	   if (_poly.selection() >= 0) // there is a selection
	   {
		   _poly.del(_poly.selection());
		   redraw();
	   } break;
   case 'i':  if (_poly.selection() >= 0) // there is a selection
   {
	   //std::cout << "Moving left: " << _poly.selection() << std::endl;
	   GsVec l = _poly.vertexarray().get(_poly.selection());

	   l.z -= 0.01;
	   _poly.move(_poly.selection(), l);

	   redraw();
   } break;

   case 'o':  if (_poly.selection() >= 0) // there is a selection
   {
	   //std::cout << "Moving right: " << _poly.selection() << std::endl;
	   GsVec l = _poly.vertexarray().get(_poly.selection());

	   l.z += 0.01;
	   _poly.move(_poly.selection(), l);

	   redraw();
   } break;
   case 27: // Esc was pressed
	   if (_poly.selection() >= 0) // there is a selection
	   {
		   _poly.select(-1, _pickprec);
		   redraw();
	   }
	   //else exit(1);
	   break;*/
}
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
	 bool rd = true;
	 const float incr = GS_TORAD(2.5f);
	 const float inct = 0.05f;
	 const float incf = 0.05f;
	 bool ctrl = glutGetModifiers()&GLUT_ACTIVE_CTRL ? true : false;
	 bool alt = glutGetModifiers()&GLUT_ACTIVE_ALT ? true : false;
	 //std::cout<<ctrl<<gsnl;

	 switch (key)
	 {
	 /*case GLUT_KEY_LEFT:      if (ctrl)_roty -= incr; else if (alt)_trans.x -= inct; break;
	 case GLUT_KEY_RIGHT:     if (ctrl)_roty += incr; else if (alt)_trans.x += inct; break;
	 case GLUT_KEY_UP:        if (ctrl)_rotx += incr; else if (alt)_trans.y += inct; break;
	 case GLUT_KEY_DOWN:      if (ctrl)_rotx -= incr; else if (alt)_trans.y -= inct; break;
	 case GLUT_KEY_PAGE_UP:   if (ctrl)_fovy -= incf; else if (alt)_trans.z += inct; break;
	 case GLUT_KEY_PAGE_DOWN: if (ctrl)_fovy += incf; else if (alt)_trans.z -= inct; break;*/
	 default: return; // return without rendering
	 }
	 if (rd) redraw(); // ask the window to be rendered when possible
 }

static GsVec plane_intersect(const GsVec& c, const GsVec& n, const GsVec& p1, const GsVec& p2)
{
	GsVec coords = n;
	float coordsw = -dot(n, c);
	float nl = n.norm();
	if (nl == 0.0) return GsVec::null;
	coords /= nl; coordsw /= nl;
	float fact = dot(coords, p1 - p2);
	if (fact == 0.0) return GsVec::null;
	float k = (coordsw + dot(coords, p1)) / fact;
	return mix(p1, p2, k);
}

GsVec AppWindow::rayXYintercept(const GsVec2& v)
{
	// Recall that a mouse click in the screen corresponds to a ray traversing the 3D scene
	// Here we intersect this ray with the XY plane:
	GsVec m(windowToScene(v));
	GsVec p1(m.x, m.y, _znear); // mouse click in the near plane
	GsMat M(sproj); M.invert();
	p1 = M * p1;          // mouse click in camera coordinates
	GsVec p2 = p1 - _eye;
	p2.normalize();
	p2 *= (_zfar + _znear); // zfar and znear are >0
	p2 += p1;             // p1,p2 ray in camera coordinates
	M = stransf; M.invert();
	p1 = M * p1;
	p2 = M * p2;          // p1,p2 ray in scene coordinates
	return plane_intersect(GsVec::null, GsVec::k, p1, p2); // intersect p1,p2 ray with XY plane
}

void AppWindow::glutMouse(int button, int state, int x, int y)
{
	GsVec m = rayXYintercept(GsVec2(x, y));
	//std::cout<<m<<gsnl;
	_motion = false;
	// Check if a vertex is being selected, ie, if m is very close to a vertex:
	int i;
	const GsArray<GsVec>& V = _poly.vertexarray();
	for (i = 0; i<V.size(); i++)
		if (dist(V[i], m) <= _pickprec) break;

	if (i<V.size()) // vertex selected
	{
		_poly.select(i, _pickprec);
		_oktodrag = true;
		redraw();
	}
	else if (state == GLUT_UP)
	{ // At this point no selection was detected and we will add a new vertex:
		_poly.add(m);
		_oktodrag = false;
		redraw();
	}
}
void AppWindow::glutMotion(int x, int y)
{
	//std::cout<<"Motion"<<std::endl;
	if (!_oktodrag) { _motion = false; return; }
	GsVec m = rayXYintercept(GsVec2(x, y));
	if (_poly.selection() >= 0) { _motion = true;  _poly.move(_poly.selection(), m); redraw(); }
}

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
	 V.remove(0, V.size());
	 V1.remove(0, V1.size());
	 V2.remove(0, V2.size());
	 VP.remove(0, VP.size());
	 IVP.remove(0, IVP.size());

   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { _axis.build(1.0f); // axis has radius 1.0
    }

   // Define our scene transformation:
   GsMat rx, ry, trans_plane_up, trans_plane_motion, init_plane_rotate, plane_rotate_motion;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix
   stransf.setrans(_trans);

   //initial plane translation into the air

   trans_plane_up.translation(GsVec(0.0f, 1.0f, 0.0f));
   trans_plane_motion.translation(GsVec(0.75f*cos(_angle), 0.0f, 0.75f*sin(_angle)));
   plane_rotate_motion.roty(-_angle);
   init_plane_rotate.roty(-(M_PI / 2));
   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp;

   //_proj.perspective(_fovy, _aspect, _znear, _zfar);
   //_spr = _proj * _cam; // final scene projection
   
   if (!c_pressed) {
	   camview.lookat(_eye, _center, _up);
   }

   else {
	   camview.lookat(my_eye, my_center, _up);
	   i++;

   }

   //camview.lookat ( _eye, _center, _up ); // set our 4x4 "camera" matrix

   float aspect=1.0f, znear=0.1f, zfar=50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   sproj = persp * camview; // set final scene projection

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause our values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   // Draw:
   if ( _viewaxis ) _axis.draw ( stransf, sproj );
   //_poly.draw(stransf, sproj);

   //_curve1.draw(stransf, sproj,_light,_mat);
   _model.draw ( stransf, sproj, _light );
   _plane.draw(stransf*trans_plane_up*trans_plane_motion*plane_rotate_motion*init_plane_rotate, sproj, _light);

   //evaluate bspline for first and second poly

   for (float i = 0; i <= num_seg; i++) {

	   V1.push(eval_bspline((i*(P1.size() - 1) / num_seg) + 2, 3, _poly1.vertexarray()));
	   V2.push(eval_bspline((i*(P2.size() - 1) / num_seg) + 2, 3, _poly2.vertexarray()));

   }

   //initialize plane curve
   GsVec plane;
   float plane_x, plane_z;
   for (int p = 0; p <= 50; p++) {
	   plane_x = 0.85f*cos(p*(M_PI / 25));
	   plane_z = 0.85f*sin(p*(M_PI / 25));
	   plane = GsVec(plane_x, 1.1f, plane_z);

	   VP.push(plane);

   }

   for (int p = 0; p <= 50; p++) {
	   plane_x = 0.75f*cos(p*(M_PI / 25));
	   plane_z = 0.75f*sin(p*(M_PI / 25));
	   plane = GsVec(plane_x, 1.0f, plane_z);

	   IVP.push(plane);



   }


   _curve2.build(V2, z_pressed);
   _curve1.build(V1, z_pressed);
   _curve3.build(VP, z_pressed);
   _curve4.build(IVP, z_pressed);
   if (_view_curves) {
	   _poly1.draw(stransf, sproj);
	   _poly2.draw(stransf, sproj);
	  
	   _curve1.draw(stransf, sproj, _light, _mat);
	   _curve2.draw(stransf, sproj, _light, _mat);
	   _curve3.draw(stransf, sproj, _light, _mat);
	   _curve4.draw(stransf, sproj, _light, _mat);
   }

	/*for (float i = 0; i <= num_seg; i++) {

			   //V1.push(eval_bspline((i*(P1.size() - 1) / num_seg) + 2, 3, _poly1.vertexarray()));
			   V2.push(eval_bspline((i*(P2.size() - 1) / num_seg) + 2, 3, _poly2.vertexarray()));

    }*/

	
		   /*if (x_pressed && (_motion == false)) {


			   if (_NC) { // No curve
				   V.remove(0, V.size());
			   }

			   if (_L) {
				   for (float i = 0; i <= num_seg; i++) {

					   V.push(eval_lagrange(i*(_poly.num_vertices() - 1) / num_seg, 0, _poly.vertexarray()));

				   }

				   _curve.build(V, z_pressed);
				   _curve.draw(stransf, sproj, _light, _mat);

			   }

			   if (_B) {
				   for (float i = 0; i <= num_seg; i++) {

					   V.push(eval_bezier((i / num_seg), _poly.vertexarray()));

				   }

				   _curve.build(V, z_pressed);
				   _curve.draw(stransf, sproj, _light, _mat);

			   }

			   if (_BS) {
				   for (float i = 0; i <= num_seg; i++) {

					   V.push(eval_bspline((i*(_poly.num_vertices() - 1) / num_seg) + 2, 3, _poly.vertexarray()));

				   }

				   _curve.build(V, z_pressed);
				   _curve.draw(stransf, sproj, _light, _mat);

			   }

			   if (_CR) {
				   for (float i = 0; i <= num_seg; i++) { //Catmull Rom Spline

					   V.push(crspline(i*(_poly.num_vertices() - 3) / num_seg, _poly.vertexarray()));
				   }
				   _curve.build(V, z_pressed);
				   _curve.draw(stransf, sproj, _light, _mat);
			   }
		   }


		   else if (x_pressed && _motion == true) {
			   _curve.draw(stransf, sproj, _light, _mat);
		   }

		   else if (!x_pressed)
		   {
			   if (_NC) { // No curve
				   V.remove(0, V.size());
			   }

			   if (_L) {
				   for (float i = 0; i <= num_seg; i++) {

					   V.push(eval_lagrange(i*(_poly.num_vertices() - 1) / num_seg, 0, _poly.vertexarray()));

				   }

				   _curve.build(V, z_pressed);
				   _curve.draw(stransf, sproj, _light, _mat);

			   }

			   if (_B) {
				   for (float i = 0; i <= num_seg; i++) {

					   V.push(eval_bezier((i / num_seg), _poly.vertexarray()));

				   }

				   _curve.build(V, z_pressed);
				   _curve.draw(stransf, sproj, _light, _mat);

			   }

			   if (_BS) {
				   for (float i = 0; i <= num_seg; i++) {

					   V.push(eval_bspline((i*(_poly.num_vertices() - 1) / num_seg) + 2, 3, _poly.vertexarray()));

				   }

				   _curve.build(V, z_pressed);
				   _curve.draw(stransf, sproj, _light, _mat);

			   }

			   if (_CR) {
				   for (float i = 0; i <= num_seg; i++) { //Catmull Rom Spline

					   V.push(crspline(i*(_poly.num_vertices() - 3) / num_seg, _poly.vertexarray()));
				   }
				   _curve.build(V, z_pressed);
				   _curve.draw(stransf, sproj, _light, _mat);
			   }
		   }*/





   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}


