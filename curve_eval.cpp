#include "curve_eval.h"
#include <math.h>

float fact(float n) {
	float result = 1;

	for (float i = 1; i <= n; i++) {
		result *= i;

	}
	return result;
}
 
GsVec eval_bezier (float t, const GsArray<GsVec>& ctrlpnts) {

	GsVec V(0.0f, 0.0f, 0.0f);

	
	float n = (float)ctrlpnts.size()-1;
	float Bt = 0;
	float poly1 = 1;
	float poly2 = 1;

	if (n > 1) {
		for (int i = 0; i < n; i++)
		{
			

			for (int k = 0; k < i; k++) {
				poly1 *= t;
			}

			for (int j = i; j < n; j++) {
				poly2 *= (1 - t);
			}

			Bt = (fact(n) / (fact(i)*(fact(n - i))))*poly1*poly2;
			

			V.x += (ctrlpnts(i).x*Bt);
			V.y += (ctrlpnts(i).y*Bt);
		}


	}




	return V;
}

GsVec eval_lagrange(float t, float sn, const GsArray<GsVec>& ctrlpnts) {

	

	GsVec V (0.0f, 0.0f, 0.0f);
	float n = (float)ctrlpnts.size();
	float t_i = 0;
	float t_j = 0;
	float result = 1;

	for (float i = 0; i < n; i++) { // b(t)
		result = 1;
		for (float j = 0; j < n; j++) {
			if (i != j) {
				t_i = i;
				t_j = j;
				result *= (t - t_j) / (t_i - t_j);
			}
		}

		V.x += ctrlpnts(i).x*result;
		V.y += ctrlpnts(i).y*result;
		V.z += ctrlpnts(i).z*result;
	}


	return V;

}

/*! B-Spline order k, n=pnts.size()-1;     For order k=3, (degree 2, quadratic case): t in [2,n+1]  */
GsVec eval_bspline(float t, int k, const GsArray<GsVec>& pnts) {
	//std::cout << "Here1" << std::endl;
	GsVec V;
	float n = (float)pnts.size();
	float result = 0.0f;
	
	for (float i = 0; i < n; i++) { 
		if (t > (n)) {
			t = (n);
		}
		result = N(i, k, t);

		V.x += pnts(i).x*result;
		V.y += pnts(i).y*result;
		V.z += pnts(i).z*result;

	}


	return V;
}

/*! Evalutes a Catmull-Rom cubic spline, n=pnts.size()-1, t in [0,n-2] */
GsVec crspline(float t, const GsArray<GsVec>& pnts) {
	GsVec V; 
	float n = (float)pnts.size();
	int tt;
	//std::cout << "Size: " << n << std::endl;
	
	//std::cout << "t = " << t << " ";

	if (t - (int)t == 0 && t > 0) { // t is and integer
		tt = t;

	}

	else {
		tt = t+1;
	}
	

	if (t >= 1) {
		t = t / tt;
	}
	


	//std::cout << "tt = " << tt << std::endl;
	if (n > 3) {
		GsVec P0 = pnts[tt-1];
		GsVec P1 = pnts[tt];
		GsVec P2 = pnts[tt + 1];
		GsVec P3 = pnts[tt + 2];
		V = 0.5f * ((2.0f * P1) +
			(-P0 + P2) * t +
			((2.0f * P0) - (5.0f*P1) + (4.0f*P2) - P3) * (t*t) +
			(-P0 + (3.0f * P1) - (3.0f * P2) + P3) * (t*t*t));

	}
	return V;



}

/*! Evalutes a Bessel-Overhauser spline, n=pnts.size()-1, t in [0,n-2] */
GsVec bospline(float t, const GsArray<GsVec>& pnts) {

	GsVec V;
	/*float n = (float)pnts.size();
	int tt;
	//std::cout << "Size: " << n << std::endl;

	//std::cout << "t = " << t << " ";

	if (t - (int)t == 0 && t > 0) { // t is and integer
		tt = t;

	}

	else {
		tt = t + 1;
	}


	if (t >= 1) {
		t = t / tt;
	}

	if (n > 2) {
		GsVec P0 = pnts[tt - 1];
		GsVec P1 = pnts[tt];
		GsVec P2 = pnts[tt + 1];

		float d_minus = distmax(P1, P0);
		float d_plus = distmax(P2, P1);

		GsVec V_plus = (P2 - P1) / d_plus;
		GsVec V_minus = (P1 - P0) / d_minus;
		V = ((d_plus*V_minus) + (d_minus * V_plus)) / (d_minus + d_plus);
		


	}*/
	

	return V;


}

// n=pnts.size()-1=1-k-1, l=u.size()-1, order k,
static float N(int i, int k, float t, const GsArray<float>& u)
{
	if (k == 1)
		return u[i] <= t && t <= u[i + 1] ? 1.0f : 0;
	else
		return  ((t - u[i]) / (u[i + k - 1] - u[i])) * N(i, k - 1, t, u) +
			((u[i + k] - t) / (u[i + k] - u[i + 1])) * N(i + 1, k - 1, t, u);
}


//n=pnts.size()-1, i in [0,n], u in [3,n+1]
static float N(int i, int k, float u)
{
	float ui = float(i);
	if (k == 1)
		return ui <= u && u < i + 1 ? 1.0f : 0;
	else
		return ((u - ui) / (k - 1)) * N(i, k - 1, u) +
		((ui + k - u) / (k - 1)) * N(i + 1, k - 1, u);



}
