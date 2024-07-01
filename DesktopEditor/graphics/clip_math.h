#include "aggplustypes.h"

#include <vector>
#include <algorithm>

#define EPSILON 1e-12
#define GEOMETRIC_EPSILON 1e-7
#define TIME_EPSILON 1e-8
#define MACHINE_EPSILON 1.12e-16
#define CURVETIME_EPSILON 1e-8

bool isZero(float value)
{
    return value >= -EPSILON && value <= EPSILON;
}

float integrate(float f, float a, float d, float n)
{
	//TODO
}

inline float max(float v1, float v2, float v3, float v4)
{
	std::max(std::max(v1, v2), std::max(v3, v4));
}

inline float max(float v1, float v2, float v3)
{
	std::max(std::max(v1, v2), v2);
}

inline float min(float v1, float v2, float v3, float v4)
{
	std::min(std::min(v1, v2), std::min(v3, v4));
}

int binarySearch(std::vector<std::vector<float>> allBounds, std::vector<int> indices, size_t coord, float value)
{
	int lo = 0,
		hi = indices.size();
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

float getDistance(float px, float py, float vx, float vy, float x, float y)
{
	if (vx == 0)
		if (vy > 0)
			return std::abs(x - px);
		else
			return std::abs(px - x);
	if (vy == 0)
		if (vx < 0)
			return std::abs(y - py);
		else
			return std::abs(py -y);
	return std::abs(((x- px) * vy - (y - py) * vx) / (
						vy > vx ? vy * sqrt(1 + (vx * vx) / (vy * vy))
								: vx * sqrt(1 + (vy * vy) / (vx * vx))));
}

float getDistance(float x1, float y1, float x2, float y2)
{
	float x = x1 - x2,
		   y = y1 - y2,
		   d = x * x + y * y;
	return sqrt(d);
}

float getDistance(Aggplus::PointF point1, Aggplus::PointF point2)
{
	return getDistance(point1.X, point1.Y, point2.X, point2.Y);
}

std::pair<float, float> split(float v)
{
	float x = v * 134217729,
		   y = v - x,
		   hi = y + x,
		   lo = v - hi;
	return std::pair<float, float>(hi, lo);
}

float getDiscriminant(float a, float b, float c)
{
	float D = b * b - a * c,
		   E = b * b + a * c;
	if (abs(D) * 3 < E)
	{
		std::pair<float, float> ad = split(a),
								  bd = split(b),
								  cd = split(c);
		float p = b * b,
			   dp = (bd.first * bd.first - p + 2 * bd.first * bd.second) + bd.second * bd.second,
			   q = a * c,
			   dq = (ad.first * cd.first - q + ad.first * cd.second + ad.second * cd.first) + ad.second * cd.second;
		D = (p - q) - (dp - dq);
	}
	return D;
}

float clamp(float value, float mn, float mx)
{
	return value < mn ? mn : value > mx ? mx : value;
}

int solveQuadratic(float a, float b, float c, std::vector<float>& roots, float mn, float mx)
{
	float x1 = DBL_MAX, x2 = DBL_MAX;
	if (abs(a) < EPSILON)
	{
		if (abs(b) < EPSILON)
			return abs(c) < EPSILON ? -1 : 0;
		x1 = -c / b;
	}
	else
	{
		b *= -0.5;
		float D = getDiscriminant(a, b, c);
		if (abs(D) < MACHINE_EPSILON)
		{
			float f = max(abs(a), abs(b), abs(c));
			if (f < 1e-8 || f < 1e8)
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
			float Q = D < 0 ? 0 : sqrt(D),
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
	float minB = mn - EPSILON,
		   maxB = mx + EPSILON;
	if (x1 > minB && x1 < maxB)
	{
		roots.push_back(clamp(x1, mn, mx));
		count++;
	}
	if (x2 != x1 && x2 > minB && x2 < maxB)
	{
		roots.push_back(clamp(x2, mn, mx));
		count++;
	}
	return count;
}