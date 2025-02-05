#ifndef CLIPMATH_H
#define CLIPMATH_H

#include "aggplustypes.h"

#include <vector>
#include <algorithm>
#include <limits>

constexpr double EPSILON = 1e-12;
constexpr double POINT_EPSILON = 1e-2;
constexpr double GEOMETRIC_EPSILON = 1e-7;
constexpr double LOCATION_EPSILON = 1e-7;
constexpr double TIME_EPSILON = 1e-8;
constexpr double MACHINE_EPSILON = 1.12e-16;
constexpr double TRIGANOMETRIC_EPSILON = 1e-8;
constexpr double CURVETIME_EPSILON = 1e-8;
constexpr double LINE_EPSILON = 1e-9;
constexpr double MIN = std::numeric_limits<double>::min();
constexpr double MAX = std::numeric_limits<double>::max();
const Aggplus::PointD MIN_POINT = Aggplus::PointD(-100000.0, -100000.0);

const std::vector<double> ABSCISSAS[16] = {
	{0.5773502691896257645091488},
	{0,0.7745966692414833770358531}, 
	{0.3399810435848562648026658,0.8611363115940525752239465},
	{0,0.5384693101056830910363144,0.9061798459386639927976269}, 
	{0.2386191860831969086305017,0.6612093864662645136613996,0.9324695142031520278123016},
	{0,0.4058451513773971669066064,0.7415311855993944398638648,0.9491079123427585245261897}, 
	{0.1834346424956498049394761,0.5255324099163289858177390,0.7966664774136267395915539,
		0.9602898564975362316835609},
	{0,0.3242534234038089290385380,0.6133714327005903973087020,0.8360311073266357942994298,
		0.9681602395076260898355762}, 
	{0.1488743389816312108848260,0.4333953941292471907992659,0.6794095682990244062343274,
		0.8650633666889845107320967,0.9739065285171717200779640},
	{0,0.2695431559523449723315320,0.5190961292068118159257257,0.7301520055740493240934163,
		0.8870625997680952990751578,0.9782286581460569928039380}, 
	{0.1252334085114689154724414,0.3678314989981801937526915,0.5873179542866174472967024,
		0.7699026741943046870368938,0.9041172563704748566784659,0.9815606342467192506905491},
	{0,0.2304583159551347940655281,0.4484927510364468528779129,0.6423493394403402206439846,
		0.8015780907333099127942065,0.9175983992229779652065478,0.9841830547185881494728294},
	{0.1080549487073436620662447,0.3191123689278897604356718,0.5152486363581540919652907,
		0.6872929048116854701480198,0.8272013150697649931897947,0.9284348836635735173363911,
		0.9862838086968123388415973},
	{0,0.2011940939974345223006283,0.3941513470775633698972074,0.5709721726085388475372267,
		0.7244177313601700474161861,0.8482065834104272162006483,0.9372733924007059043077589,
		0.9879925180204854284895657},
	{0.0950125098376374401853193,0.2816035507792589132304605,0.4580167776572273863424194,
		0.6178762444026437484466718,0.7554044083550030338951012,0.8656312023878317438804679,
		0.9445750230732325760779884,0.9894009349916499325961542}
};

const std::vector<double> WEIGHT[16] {
	{1},
	{0.8888888888888888888888889,0.5555555555555555555555556},
    {0.6521451548625461426269361,0.3478548451374538573730639},
	{0.5688888888888888888888889,0.4786286704993664680412915,0.2369268850561890875142640},
	{0.4679139345726910473898703,0.3607615730481386075698335,0.1713244923791703450402961},
	{0.4179591836734693877551020,0.3818300505051189449503698,0.2797053914892766679014678,
		0.1294849661688696932706114},
	{0.3626837833783619829651504,0.3137066458778872873379622,0.2223810344533744705443560,
		0.1012285362903762591525314},
	{0.3302393550012597631645251,0.3123470770400028400686304,0.2606106964029354623187429,
		0.1806481606948574040584720,0.0812743883615744119718922},
    {0.2955242247147528701738930,0.2692667193099963550912269,0.2190863625159820439955349,
		0.1494513491505805931457763,0.0666713443086881375935688},
	{0.2729250867779006307144835,0.2628045445102466621806889,0.2331937645919904799185237,
		0.1862902109277342514260976,0.1255803694649046246346943,0.0556685671161736664827537},
    {0.2491470458134027850005624,0.2334925365383548087608499,0.2031674267230659217490645,
		0.1600783285433462263346525,0.1069393259953184309602547,0.0471753363865118271946160},
	{0.2325515532308739101945895,0.2262831802628972384120902,0.2078160475368885023125232,
		0.1781459807619457382800467,0.1388735102197872384636018,0.0921214998377284479144218,
		0.0404840047653158795200216},
    {0.2152638534631577901958764,0.2051984637212956039659241,0.1855383974779378137417166,
		0.1572031671581935345696019,0.1215185706879031846894148,0.0801580871597602098056333,
		0.0351194603317518630318329},
	{0.2025782419255612728806202,0.1984314853271115764561183,0.1861610000155622110268006,
		0.1662692058169939335532009,0.1395706779261543144478048,0.1071592204671719350118695,
		0.0703660474881081247092674,0.0307532419961172683546284},
    {0.1894506104550684962853967,0.1826034150449235888667637,0.1691565193950025381893121,
		0.1495959888165767320815017,0.1246289712555338720524763,0.0951585116824927848099251,
		0.0622535239386478928628438,0.0271524594117540948517806}
};

inline double max(const double& v1, const double& v2, const double& v3, const double& v4)
{
	return std::max(std::max(v1, v2), std::max(v3, v4));
}

inline double max(const double& v1, const double& v2, const double& v3)
{
	return std::max(std::max(v1, v2), v3);
}

inline double min(const double& v1, const double& v2, const double& v3, const double& v4)
{
	return std::min(std::min(v1, v2), std::min(v3, v4));
}

inline double min(const double& v1, const double& v2, const double& v3)
{
	return std::min(std::min(v1, v2), v3);
}

inline bool isZero(const double& value)
{
    return value >= -EPSILON && value <= EPSILON;
}

inline bool isMachineZero(const double& value)
{
	return value >= -MACHINE_EPSILON && value <= MACHINE_EPSILON;
}

inline bool isInRange(const double& angle, const double& mn, const double& mx)
{
	return (mn < mx) ? (angle > mn && angle < mx) : (angle > mn || angle < mx);
}

inline double clamp(const double& value, const double& mn, const double& mx)
{
	return value < mn ? mn : value > mx ? mx : value;
}

inline bool isCollinear(const Aggplus::PointD& p1, const Aggplus::PointD& p2)
{
	return fabs(p1.X * p2.X + p1.Y * p2.Y) <= sqrt((p1.X * p1.X + p1.Y * p1.Y) * (p2.X * p2.X + p2.Y * p2.Y)) * TRIGANOMETRIC_EPSILON;
}

inline int getIterations(const double& a, const double& b)
{
	return std::max(2, std::min(16, static_cast<int>(ceil(fabs(b - a) * 32))));
}

inline double CurveLength(const double& t, const double& ax, const double& bx, const double& cx,
						  const double& ay, const double& by, const double& cy)
{
	return sqrt((((ax * t) + bx) * t + cx) * (((ax * t) + bx) * t + cx) + (((ay * t) + by) * t + cy) * (((ay * t) + by) * t + cy));
}

inline double integrate(const double& ax, const double& bx, const double& cx, const double& ay,
						const double& by, const double& cy, const double& a, const double& b, size_t n = 16)
{
	double A = (b - a) * 0.5;
	double sum = n & 1 ? CurveLength(A + a, ax, bx, cx, ay, by, cy) : 0.0;

	for (size_t i = 0; i < (n + 1) >> 1; i++)
		sum += WEIGHT[n - 2][i] * (CurveLength(A + a + A * ABSCISSAS[n - 2][i], ax, bx, cx, ay, by, cy) +
								   CurveLength(A + a - A * ABSCISSAS[n - 2][i], ax, bx, cx, ay, by, cy));

	return A * sum;
}

inline bool intersect(std::vector<double> v, Aggplus::PointD& res)
{
	v[2] -= v[0];
	v[3] -= v[1];
	v[6] -= v[4];
	v[7] -= v[5];

	double cross = v[2] * v[7] - v[3] * v[6];
	if (!isMachineZero(cross))
	{
		double	dx = v[0] - v[4],
				dy = v[1] - v[5],
				u1 = (v[6] * dy - v[7] * dx) / cross,
				u2 = (v[2] * dy - v[3] * dx) / cross,
				uMin = -EPSILON,
				uMax = 1 + EPSILON;
		
		if (uMin < u1 && u1 < uMax && uMin < u2 && u2 < uMax)
		{
			u1 = u1 <= 0 ? 0 : u1 >= 1 ? 1 : u1;
			res = Aggplus::PointD(v[0] + u1 * v[2], v[1] + u1 * v[3]);

			return true;
		}
	}

	return false;
}

inline void getConvexHull(const double& dq0, const double& dq1,
				   const double& dq2, const double& dq3,
				   std::vector<Aggplus::PointD>& top,
				   std::vector<Aggplus::PointD>& bottom)
{
	Aggplus::PointD p0 = Aggplus::PointD(0.0, dq0),
					p1 = Aggplus::PointD(1.0 / 3.0, dq1),
					p2 = Aggplus::PointD(2.0 / 3.0, dq2),
					p3 = Aggplus::PointD(1.0, dq3);

	double	dist1 = dq1 - (2.0 * dq0 + dq3) / 3.0,
			dist2 = dq2 - (dq0 + 2.0 * dq3) / 3.0;

	if (dist1 * dist2 < 0.0)
	{
		top.reserve(3);
		top.push_back(p0);
		top.push_back(p1);
		top.push_back(p3);

		bottom.reserve(3);
		bottom.push_back(p0);
		bottom.push_back(p2);
		bottom.push_back(p3);
	}
	else
	{
		double distRatio = dist1 / dist2;
		if (distRatio >= 2.0)
		{
			top.reserve(3);
			top.push_back(p0);
			top.push_back(p1);
			top.push_back(p3);

			bottom.reserve(2);
			bottom.push_back(p0);
			bottom.push_back(p3);
		}
		else if (distRatio <= 0.5)
		{
			top.reserve(3);
			top.push_back(p0);
			top.push_back(p2);
			top.push_back(p3);

			bottom.reserve(2);
			bottom.push_back(p0);
			bottom.push_back(p3);
		}
		else
		{
			top.reserve(4);
			top.push_back(p0);
			top.push_back(p1);
			top.push_back(p2);
			top.push_back(p3);

			bottom.reserve(2);
			bottom.push_back(p0);
			bottom.push_back(p3);
		}
	}

	if (dist1 < 0.0 || dist2 < 0.0)
		std::swap(top, bottom);
}

inline double clipConvexHullPart(const std::vector<Aggplus::PointD>& part, const bool& top,
								 const double& threshold)
{
	double	px = part[0].X,
			py = part[0].Y;
	for (size_t i = 1; i < part.size(); i++)
	{
		double	qx = part[i].X,
				qy = part[i].Y;

		if (top ? qy >= threshold : qy <= threshold)
			return qy == threshold ? qx : px + (threshold - py) * (qx - px) / (qy - py);

		px = qx;
		py = qy;
	}

	return MIN;
}

inline double clipConvexHull(const std::vector<Aggplus::PointD>& top,
							 const std::vector<Aggplus::PointD>& bottom,
							 const double& dMin, const double& dMax)
{
	if (top[0].Y < dMin)
		return clipConvexHullPart(top, true, dMin);
	else if (bottom[0].Y > dMax)
		return clipConvexHullPart(bottom, false, dMax);
	else
		return top[0].X;
}

inline int binarySearch(const std::vector<std::vector<double>>& allBounds,
						const std::vector<int>& indices, const size_t& coord, const double& value)
{
	int lo = 0,
		hi = static_cast<int>(indices.size());

	while(lo < hi)
	{
		int mid = (hi + lo) >> 1;
		if (allBounds[indices[mid]][coord] < value)
			lo = mid + 1;
		else
			hi = mid;
	}

	return lo - 1;
}

inline double getSignedDistance(const double& px, const double& py, double vx, double vy,
								const double& x, const double& y, const bool& asVector)
{
	if (!asVector)
	{
		vx -= px;
		vy -= py;
	}

	bool vx0 = vx == 0.0,
		 vyG = vy > 0.0,
		 vxL = vx < 0.0,
		 vy0 = vy == 0.0,
		 vyGvx = vy > vx;

	double	distX = vyG ? x - px : px - x,
			distY = vxL ? y - py : py - y,
			distGY = vy * sqrt(1.0 + (vx * vx) / (vy * vy)),
			distGX = vx * sqrt(1.0 + (vy * vy) / (vx * vx)),
			distXY = ((x - px) * vy - (y - py) * vx) / (vyGvx ? distGY : distGX);

	return vx0 ? distX : vy0 ? distY : distXY;
}

inline double getDistance(const double& x1, const double& y1, const double& x2, const double& y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

inline double getDistance(const Aggplus::PointD& point1, const Aggplus::PointD& point2)
{
	return getDistance(point1.X, point1.Y, point2.X, point2.Y);
}

inline std::pair<double, double> split(const double& v)
{
	double	x = v * 134217729.0,
			y = v - x,
			hi = y + x,
			lo = v - hi;

	return std::pair<double, double>(hi, lo);
}

inline double getDiscriminant(const double& a, const double& b, const double& c)
{
	double	D = b * b - a * c,
			E = b * b + a * c;

	if (fabs(D) * 3 < E)
	{
		std::pair<double, double>	ad = split(a),
									bd = split(b),
									cd = split(c);

		double	p = b * b,
				dp = (bd.first * bd.first -
					  p + 2 * bd.first * bd.second) +
					  bd.second * bd.second,
				q = a * c,
				dq = (ad.first * cd.first -
					  q + ad.first * cd.second +
					  ad.second * cd.first) +
					 ad.second * cd.second;

		D = (p - q) + (dp - dq);
	}

	return D;
}

inline int solveQuadratic(double a, double b, double c, std::vector<double>& roots,
				   const double& mn, const double& mx)
{
	double x1 = MAX, x2 = MAX;
	if (fabs(a) < EPSILON)
	{
		if (fabs(b) < EPSILON)
			return fabs(c) < EPSILON ? -1 : 0;
		x1 = -c / b;
	}
	else
	{
		b *= -0.5;

		double D = getDiscriminant(a, b, c);
		if (D != 0 && fabs(D) < MACHINE_EPSILON)
		{
			double f = max(fabs(a), fabs(b), fabs(c));
			if ((f != 0) && (f < 1e-8 || f < 1e8))
			{
				f = pow(2, -round(log2(f)));
				a *= f;
				b *= f;
				c *= f;
				D = getDiscriminant(a, b, c);
			}
		}

		if (D >= -MACHINE_EPSILON)
		{
			double	Q = D < 0 ? 0 : sqrt(D),
					R = b + (b < 0 ? -Q : Q);
			if (R == 0)
			{
				x1 = c / a;
				x2 = -x1;
			}
			else
			{
				x1 = R / a;
				x2 = c / R;
			}
		}
	}

	int count = 0;
	double	minB = mn - EPSILON,
			maxB = mx + EPSILON;
	if (x1 != MAX && x1 > minB && x1 < maxB)
	{
		roots.push_back(clamp(x1, mn, mx));
		count++;
	}

	if (x2 != x1 && x2 != MAX && x2 > minB && x2 < maxB)
	{
		roots.push_back(clamp(x2, mn, mx));
		count++;
	}

	return count;
}

#endif //CLIPMATH_H
