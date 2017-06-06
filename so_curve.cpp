#include "so_curve.h"


void SoCurve::init()
{
	// Define buffers needed:

	/*_vshgou.load_and_compile(GL_VERTEX_SHADER, "../mcol_gouraud.vert");
	_fshgou.load_and_compile(GL_FRAGMENT_SHADER, "../gouraud.frag");
	_proggouraud.init_and_link(_vshgou, _fshgou);*/

	_vshgou.load_and_compile(GL_VERTEX_SHADER, "../smtlgouraud.vert");
	_fshgou.load_and_compile(GL_FRAGMENT_SHADER, "../gouraud.frag");
	_proggouraud.init_and_link(_vshgou, _fshgou);

	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for normals

	_proggouraud.uniform_locations(10); // will send 9 variables
	_proggouraud.uniform_location(0, "vTransf");
	_proggouraud.uniform_location(1, "vProj");
	_proggouraud.uniform_location(2, "lPos");
	_proggouraud.uniform_location(3, "la");
	_proggouraud.uniform_location(4, "ld");
	_proggouraud.uniform_location(5, "ls");
	_proggouraud.uniform_location(6, "ka");
	_proggouraud.uniform_location(7, "kd");
	_proggouraud.uniform_location(8, "ks");
	_proggouraud.uniform_location(9, "sh");

	// Define buffers needed:
	/*set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");*/

}

void SoCurve::build(const GsArray<GsVec>& curve_pnts, bool z_pressed) {
	P.clear(); N.clear(); C.clear();
	P.reserve(18); N.reserve(18); C.reserve(18);
	GsVec Z(0.0f, 0.0f, -0.5f);
	GsVec NN(0.0f, 0.0f, 0.0f);
	z = z_pressed;
	if(z_pressed){

		if (curve_pnts.size() > 1) {
			for (int i = 0; i < curve_pnts.size() - 1; i++) {
				GsVec f1 = curve_pnts[i];
				GsVec f2 = curve_pnts[i + 1];
				GsVec f3 = curve_pnts[i] + Z;
				GsVec f4 = curve_pnts[i + 1] + Z;

				GsVec N1 = f1;

				//Face1
				P.push_back(f1);
				P.push_back(f2);
				P.push_back(f3);

				//Face2
				P.push_back(f2);
				P.push_back(f4);
				P.push_back(f3);

				N.push_back(NN);

				/*C.push_back(GsColor::green);
				C.push_back(GsColor::green);*/

			}
		}

		
	}
	
	else if(!z_pressed) {
		if (curve_pnts.size() > 1) {
			for (int i = 0; i < curve_pnts.size() - 1; i++) {
				GsVec f1 = curve_pnts[i];
				GsVec f2 = curve_pnts[i + 1];


				P.push_back(f1);
				P.push_back(f2);
				

				N.push_back(NN);
			}

		}
	}
	

	// send data to OpenGL buffers:
	glBindVertexArray(va[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	/*glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);*/

	glBindVertexArray(0); // break the existing vertex array object binding

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	P.resize(0); N.resize(0); C.resize(0);

}

void SoCurve::draw(const GsMat& tr, const GsMat& pr, const GsLight& l, const Material& m)
{
	     
	// Draw Lines:
	glUseProgram(_proggouraud.id);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // false means no normalization

	glUniformMatrix4fv(_proggouraud.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_proggouraud.uniloc[1], 1, GL_FALSE, pr.e);

	float f[4]; //we convert below our color values to be in [0,1]
	glUniform3fv(_proggouraud.uniloc[2], 1, l.pos.e);
	glUniform4fv(_proggouraud.uniloc[3], 1, l.amb.get(f));
	glUniform4fv(_proggouraud.uniloc[4], 1, l.dif.get(f));
	glUniform4fv(_proggouraud.uniloc[5], 1, l.spe.get(f));

	glUniform4fv(_proggouraud.uniloc[6], 1, m.amb.get(f));
	glUniform4fv(_proggouraud.uniloc[7], 1, m.dif.get(f));
	glUniform4fv(_proggouraud.uniloc[8], 1, m.spe.get(f));
	glUniform1fv(_proggouraud.uniloc[9], 1, &m.sh);

	if (z) {
		glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	}
	else {
		glDrawArrays(GL_LINES, 0, _numpoints);

	}
	// Draw Lines:
	/*glUseProgram(prog);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	glDrawArrays(GL_LINES, 0, _numpoints);*/


}