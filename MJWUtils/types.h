#pragma once

struct Vector2 {
	double x;
	double y;

	Vector2(double xx, double yy) {
		x = xx;
		y = yy;
	}

	Vector2 operator+(const Vector2 &a) {
		return { x + a.x, y + a.y };
	}

	Vector2 operator-(const Vector2 &a) {
		return { x - a.x, y - a.y };
	}

	Vector2 operator*(const double &a) {
		return { x * a, y * a };
	}

	Vector2 operator/(const double &a) {
		return { x / a, y / a };
	}

	bool operator==(const Vector2 &a) const {
		return x == a.x && y == a.y;
	}
};
