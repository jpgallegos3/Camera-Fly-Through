
# include <iostream>
//# include <gsim/gs.h>
# include "ogl_tools.h"
# include "app_window.h"

//==========================================================================
// Main routine
//==========================================================================
int main ( int argc, char** argv )
 {
   // Init freeglut library:
   glutInit ( &argc, argv );
   glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitContextProfile ( GLUT_CORE_PROFILE );

   // Display some info:
   //std::cout<<"Camera keys: Arrows + PgUp and PgDn\n";
   //std::cout<<"Load models: keys 1,2,3\n";
   std::cout <<"Project 2 Fly through camera: \n";
   std::cout <<"Axis on/off: space bar\n";
   std::cout << "Curves on/off: v\n";
   std::cout << " 3 Curves to choose from:\n";
   std::cout << "	1 - BSpline Curve 1:\n";
   std::cout << "	2 - Overhead Plane curve\n";
   std::cout << "	3 - BSpline Curve 2:\n\n";
   std::cout << "Press 1, 2, 3 to pick a camera curve\n";
   std::cout<<"Press c to initiate camera \n\n";

   //std::cout<<"Notes:\n";
   //std::cout<<"- the smooth normal generation runs much faster in Release mode;\n";
   //std::cout<<"- this application crashed in one of my computers but I could not reproduce the problem, "<<
              //"let me know if you find a bug somewhere in the code.\n\n";
   // Now create the window of your application:
   AppWindow* w = new AppWindow ( "CSE_170 Jered Gallegos|Aaron Chiang Project 2", 300, 300, 640, 480 );
   
   // Use a white canvas etc:
   glClearColor(0.5, 0.5, 0.85, 0.5);
   glEnable(GL_BLEND); // for transparency and antialiasing smoothing
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_LINE_SMOOTH);
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
   glEnable(GL_POINT_SMOOTH);
   glPointSize(5.0);

   // Finally start the main loop:
   w->run ();
}
