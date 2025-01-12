#pragma once

#include <bits/stdc++.h>


class RNG;


typedef std::array<uint32_t, 3> TIndices;

const float PI_CONSTANT = 3.141592f;

class Point2D
{
private:
    float x, y;
public:

    Point2D();
    Point2D(float x, float y);


	Point2D operator * (float scale) const;
	Point2D operator / (float scale) const;
	Point2D operator + (const Point2D& other) const;
	Point2D operator - (const Point2D& other) const;
	Point2D rotated(float radians) const;

	bool operator < (const Point2D& other) const;
	bool operator == (const Point2D& other) const;

    float getX() const { return x; }
    float getY() const { return y; }

    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }

	std::string toString() const;
};

Point2D rotatePoint(const Point2D& point, double radians, const Point2D& origin);

class Triangle
{
public:
	~Triangle();

	const bool tiling;
	TIndices indices;

	std::vector<Triangle *> subTriangles;

	Triangle(bool t_123, const TIndices & inds);
};

class PenroseTiling
{

public:
	const float PHI = 1.0 / ((1.0 + std::sqrt(5.0)) / 2);
	const uint32_t POLY = 10; // Number of symmetries?

	const float BASE_SIZE = 1.25f;
	const uint32_t DEPTH = 8; //Recursion depth
	
	const bool P2 = false; // Tiling type

	std::set<Point2D> generatePenroseTiling(size_t numZones, RNG* rng);

private:
	void split(Triangle& p, std::vector<Point2D>& points, std::array<std::vector<uint32_t>, 5>& indices, uint32_t depth); 

};
