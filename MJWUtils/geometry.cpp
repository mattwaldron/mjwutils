#include "util.h"
#include "types.h"

#include <vector>

Vector2 centerOfMass(std::vector<Vector2> points) {
	Vector2 center(0, 0);
	for (auto& c : points) {
		center = center + c;
	}
	center = center / (float)points.size();
	return center;
}

Vector2 interpolate(Vector2 start, Vector2 end, double percent) {
	auto diff = (end - start) * percent;
	return start + diff;
}

Vector2 interpolate(std::vector<Vector2> points, double percent) {
	if (points.size() < 2) {
		return Vector2(NAN, NAN);
	}
	return (interpolate(points[0], points[1], percent));
}

Vector2 bezier(std::vector<Vector2> points, double percent) {
	if (points.size() < 2) {
		return Vector2(NAN, NAN);
	}
	if (points.size() == 2) {
		return interpolate(points, percent);
	}
	std::vector<Vector2> newControlPoints;
	for (auto i = 1; i < points.size(); i++) {
		newControlPoints.push_back(interpolate(points[i - 1], points[i], percent));
	}
	return bezier(newControlPoints, percent);
}

std::vector<Vector2> traceBezier(std::vector<Vector2> controlPoints, double resolution) {
	std::vector<Vector2> tracePoints;
	for (auto p = 0.0f; p <= 1.0; p += resolution) {
		tracePoints.push_back(bezier(controlPoints, p));
	}
	return tracePoints;
}