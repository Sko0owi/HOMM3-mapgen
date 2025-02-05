#include "./PenroseTiling.h"
#include "./Random.h"

Point2D::Point2D() : x(0), y(0) {}

Point2D::Point2D(float x, float y) : x(x), y(y) {}

Point2D Point2D::operator * (float scale) const {
    return Point2D(x * scale, y * scale);
}

Point2D Point2D::operator / (float scale) const {
    return Point2D(x / scale, y / scale);
}

Point2D Point2D::operator + (const Point2D& other) const {
    return Point2D(x + other.x, y + other.y);
}

Point2D Point2D::operator - (const Point2D& other) const {
    return Point2D(x - other.x, y - other.y);
}

Point2D Point2D::rotated(float radians) const {
    return Point2D(x * std::cos(radians) - y * std::sin(radians), x * std::sin(radians) + y * std::cos(radians));
}

bool Point2D::operator < (const Point2D& other) const {
    return x < other.x || (x == other.x && y < other.y);
}

bool Point2D::operator == (const Point2D& other) const {
    return x == other.x && y == other.y;
}

std::string Point2D::toString() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

Point2D rotatePoint(const Point2D& point, double radians, const Point2D& origin) {
    return (point - origin).rotated(radians) + origin;
}

Triangle::Triangle(bool t_123, const TIndices & inds) : tiling(t_123), indices(inds) {}

Triangle::~Triangle() {
    for(auto& subTriangle : subTriangles) {
        delete subTriangle;
    }
}

void PenroseTiling::split(Triangle& p, std::vector<Point2D>& points,
	std::array<std::vector<uint32_t>, 5>& indices, uint32_t depth) 
{
	uint32_t s = points.size();
	TIndices& i = p.indices;

	const auto p2 = P2;

	if (depth > 0)
	{
		if (p.tiling ^ !p2)
		{
			points.push_back(Point2D((points[i[0]] * (1.0f - PHI) ) + (points[i[2]]) * PHI));
			points.push_back(Point2D((points[i[p2]] * (1.0f - PHI)) + (points[i[!p2]] * PHI)));

			auto * t1 = new Triangle(p2, TIndices({ i[(!p2) + 1], p2 ? i[2] : s, p2 ? s : i[1] }));
			auto * t2 = new Triangle(true, TIndices({ p2 ? i[1] : s, s + 1, p2 ? s : i[1] }));
			auto * t3 = new Triangle(false, TIndices({ s, s + 1, i[0] }));

			p.subTriangles = { t1, t2, t3 };
		}
		else
		{
			points.push_back(Point2D((points[i[p2 * 2]] * (1.0f - PHI)) + (points[i[!p2]]) * PHI));

			auto * t1 = new Triangle(true, TIndices({ i[2], s, i[1] }));
			auto * t2 = new Triangle(false, TIndices({ i[(!p2) + 1], s, i[0] }));

			p.subTriangles = { t1, t2 };
		}

		for (auto& t : p.subTriangles)
		{
			if (depth == 1)
			{
				for (uint32_t k = 0; k < 3; ++k)
				{
					if (k != (t->tiling ^ !p2 ? 2 : 1))
					{
						indices[indices.size() - 1].push_back(t->indices[k]);
						indices[indices.size() - 1].push_back(t->indices[((k + 1) % 3)]);
					}
				}

				indices[t->tiling + (p.tiling ? 0 : 2)].insert(indices[t->tiling + (p.tiling ? 0 : 2)].end(), t->indices.begin(), t->indices.end());
			}

			// Split recursively
			split(*t, points, indices, depth - 1);
		}
	}

	return;
}
std::set<Point2D> PenroseTiling::generatePenroseTiling(size_t numZones, RNG* rng)
{
	float scale = 173.2f / (numZones * 1.5f + 20);
	float polyAngle = (2 * PI_CONSTANT) / POLY;

	float randomAngle = rng->nextDouble(0.25 * PI_CONSTANT, 0.75 * PI_CONSTANT);

	std::vector<Point2D> points = { Point2D(0.0f, 0.0f), Point2D(0.0f, 1.0f).rotated(randomAngle) };
	std::array<std::vector<uint32_t>, 5> indices;

	for (uint32_t i = 1; i < POLY; ++i)
	{
		Point2D next = points[i].rotated(polyAngle);
		points.push_back(next);
	}

	for (auto& p : points)
	{
		p.setX(p.getX() * scale * BASE_SIZE);
	}

	std::set<Point2D> finalPoints;

	for (uint32_t i = 0; i < POLY; i++)
	{
		std::array<uint32_t, 2> p = { (i % (POLY + 1)) + 1, ((i + 1) % POLY) + 1 };

		Triangle t(true, TIndices({ 0, p[i & 1], p[!(i & 1)] }));

		split(t, points, indices, DEPTH);
	}
    
    for (auto& p : points) {

        Point2D p_updated = p + Point2D(0.5f, 0.5f);
        if(p_updated.getX() < 0.0f || p_updated.getX() > 1.0f || p_updated.getY() < 0.0f || p_updated.getY() > 1.0f) {
            continue;
        }

        finalPoints.insert(p_updated);
    }


	return finalPoints;


}