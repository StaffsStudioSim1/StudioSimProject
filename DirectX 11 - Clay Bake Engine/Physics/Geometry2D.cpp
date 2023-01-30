#include"Geometry2D.h"
#include"matrices.h"
#include <cmath>
#include <cfloat>

// quick definition for a couple bits
#define  CMP(x, y)  (fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))
#define  CLAMP(number, minimum, maximum)  number = (number < minimum) ? minimum : \ ((number > maximum) ? maximum : number)
#define  OVERLAP(minA, minB, maxA, maxB)  ((minB <= maxA) && (minA <= maxB))

float Length(const Line2D& line)
{
	return Magnitude(line.end - line.start);
}

float LengthSq(const Line2D& line)
{
	return MagnitudeSq(line.end - line.start);
}

Vector2 GetMin(const Rectangle2D& rectangle)
{
	Vector2 point1 = rectangle.origin;
	Vector2 point2 = rectangle.origin + rectangle.size;

	return Vector2(fminf(point1.x, point2.x), fminf(point1.y, point2.y));
}

Vector2 GetMax(const Rectangle2D& rectangle)
{
	Vector2 point1 = rectangle.origin;
	Vector2 point2 = rectangle.origin + rectangle.size;

	return Vector2(fminf(point1.x, point2.x), fminf(point1.y, point2.y));
}

Rectangle2D FromMinToMax(const Vector2& min, const Vector2& max)
{
	return Rectangle2D(min, max - min);
}

bool PointOnALine(const Point2D& point, const Line2D& line)
{
	float M = (line.end.y - line.start.y) / (line.end.x - line.start.x); // find slope
	float B = line.start.y - M * line.start.x; // find y intercept
    return CMP(point.y, M * point.x + B);
}

bool PointInCircle(const Point2D& point, const Circle& circle)
{
	Line2D line(point, circle.position);
	if (LengthSq(line) < circle.radius * circle.radius)
	{
		return true;
	}
	return false;
}

bool PointInRectangle(const Point2D& point, const Rectangle2D rectangle)
{
	Vector2 min = GetMin(rectangle);
	Vector2 max = GetMax(rectangle);

	return min.x <= point.x && min.y <= point.y && point.x <= max.x && point.y <= max.y;
}

bool PointInOrientedRectangle(const Point2D& point, const OrientedRectangle& rectangle)
{
	Rectangle2D localRectangle(Point2D(), rectangle.halfExtents * 2.0f);
	Vector2 rotVector = point - rectangle.position;

	float theta = -DEG2RAD(rectangle.rotation);
	float zRotation2x2[] = { cosf(theta), sinf(theta), -sinf(theta), cosf(theta) };
	Multiply(rotVector.asArray, Vector2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	Vector2 localPoint = rotVector + rectangle.halfExtents;

	return PointInRectangle(localPoint, localRectangle);

	/* Alternate second step, Verbose
	vec3 pointToRotate(localPoint.x, localPoint.y, 0.0f);
	mat3 rotation = ZRotation3x3(-rectangle.rotation);
	vec3 result = MultiplyVector(pointToRotate, rotation);
	localPoint = vec2(result.x, result.y);*/

	// Alternate second step, With extras
	//localPoint = RotateVector(localPoint, -rectangle.rotation);
}

bool LineCircle(const Line2D& line, const Circle& circle)
{
	Vector2 ab = line.end - line.start;

	float t = Dot(circle.position - line.start, ab) / Dot(ab, ab);

	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}

	Point2D closestPoint = line.start + ab * t;
	Line2D circleToClosest(circle.position, closestPoint); 

	return LengthSq(circleToClosest) < circle.radius * circle.radius;
}
#if 1
bool LineRectangle(const Line2D& line, const Rectangle2D& rectangle)
{
	if (PointInRectangle(line.start, rectangle) || PointInRectangle(line.end, rectangle))
	{
		return true;
	}

	Vector2 norm = Normalized(line.end - line.start);
	norm.x = (norm.x != 0) ? 1.0f / norm.x : 0;
	norm.y = (norm.y != 0) ? 1.0f / norm.y : 0;
	Vector2 min = (GetMin(rectangle) - line.start) * norm;
	Vector2 max = (GetMax(rectangle) - line.start) * norm;

	float tmin = fmaxf(fminf(min.x, max.x), fminf(min.y, max.y));
	float tmax = fminf(fmaxf(min.x, max.x), fmaxf(min.y, max.y));
	if (tmax < 0 || tmin > tmax)
	{
		return false;
	}
	float t = (tmin < 0.0f) ? tmax : tmin;
	return t > 0.0f && t * t < LengthSq(line);
}
#else
bool LineRectangle(const Line2D& line, const Rectangle2D& rectangle) {
	// If either point of the line is in the rectangle, we have collision!
	if (PointInRectangle(line.start, rectangle) || PointInRectangle(line.end, rectangle)) 
	{
		return true;
	}
	// Now we do a raycast
	Vector2 min = GetMin(rectangle);
	Vector2 max = GetMax(rectangle);
	Vector2 norm = Normalized(line.end - line.start);
	float t1 = (min.x - line.start.x) / norm.x;
	float t2 = (max.x - line.start.x) / norm.x;
	float t3 = (min.y - line.start.y) / norm.y;
	float t4 = (max.y - line.start.y) / norm.y;
	float tmin = fmaxf(fminf(t1, t2), fminf(t3, t4));
	float tmax = fminf(fmaxf(t1, t2), fmaxf(t3, t4));
	
	// So the line segment is not intersecting
	if (tmax < 0) 
	{
		return false;
	}
	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax) 
	{
		return false;
	}
	float t = tmin;
	if (tmin < 0.0f)
	{
		t = tmax;
	}

	return t > 0.0f && t * t < LengthSq(line);

}
#endif

bool LineOrientedRectangle(const Line2D& line, const OrientedRectangle& rectangle)
{
	float theta = -DEG2RAD(rectangle.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	Line2D localLine;

	Vector2 rotVector = line.start - rectangle.position;
	Multiply(rotVector.asArray, Vector2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	localLine.start = rotVector + rectangle.halfExtents;

	rotVector = line.end - rectangle.position;
	Multiply(rotVector.asArray, Vector2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	localLine.end = rotVector + rectangle.halfExtents;

	Rectangle2D localRectangle(Point2D(), rectangle.halfExtents * 2.0f);
	return LineRectangle(localLine, localRectangle);
}

bool CircleCircle(const Circle& circle1, const Circle& circle2)
{
	Line2D line(circle1.position, circle2.position);
	float radiiSum = circle1.radius + circle2.radius;
	return LengthSq(line) <= radiiSum * radiiSum;
}

bool CircleRectangle(const Circle& circle, const Rectangle2D& rectangle)
{
	Point2D closestPoint = circle.position;
	Vector2 min = GetMin(rectangle);
	Vector2 max = GetMax(rectangle);
	closestPoint.x = (closestPoint.x < min.x) ? min.x :
		(closestPoint.x > max.x) ? max.x : closestPoint.x;
	closestPoint.y = (closestPoint.y < min.y) ? min.y :
		(closestPoint.y > max.y) ? max.y : closestPoint.y;

	Line2D line(circle.position, closestPoint);
	return LengthSq(line) <= circle.radius * circle.radius;
}

bool CircleOrientedRectangle(const Circle& circle, const OrientedRectangle& rectangle)
{
	Vector2 rotVector = circle.position - rectangle.position;
	float theta = -DEG2RAD(rectangle.rotation);
	float zRotation2x2[] = { cosf(theta), sinf(theta), -sinf(theta), cosf(theta) };
	Multiply(rotVector.asArray, Vector2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);

	Circle localCircle(rotVector + rectangle.halfExtents, circle.radius);
	Rectangle2D localRectangle(Point2D(), rectangle.halfExtents * 2.0f);

	return CircleRectangle(localCircle, localRectangle);
}

bool RectangleRectangle(const Rectangle2D& rectangle1, const Rectangle2D& rectangle2)
{
	Vector2 aMin = GetMin(rectangle1);
	Vector2 aMax = GetMax(rectangle1);
	Vector2 bMin = GetMin(rectangle2);
	Vector2 bMax = GetMax(rectangle2);

	bool xOverlap = ((bMin.x <= aMax.x) && (aMin.x <= bMax.x));
	bool yOverlap = ((bMin.y <= aMax.y) && (aMin.y <= bMax.y));

	return xOverlap && yOverlap;
}

Interval2D GetInterval(const Rectangle2D& rectangle, const Vector2& axis)
{
	Vector2 min = GetMin(rectangle);
	Vector2 max = GetMax(rectangle);

	Vector2 verts[] = { // Get all vertices of rect
		Vector2(min.x, min.y),
		Vector2(min.x, max.y),
		Vector2(max.x, max.y),
		Vector2(max.x, min.y)
	};

	// Set interval first projected vertex
	Interval2D result;
	result.min = Dot(axis, verts[0]);
	result.max = result.min;

	// For all other verts
	for (int i = 1; i < 4; ++i)
	{
		// Project vertex
		float projection = Dot(axis, verts[i]);
		result.min = (projection < result.min) ? projection : result.min;
		result.max = (projection > result.max) ? projection : result.max;
	}

	return result;
}

bool OverlapOnAxis(const Rectangle2D& rectangle1, const Rectangle2D& rectangle2, const Vector2& axis)
{
	Interval2D a = GetInterval(rectangle1, axis);
	Interval2D b = GetInterval(rectangle2, axis);
	return ((b.min <= a.max) && (a.min <= b.max));
}

bool RectangleRectangleSAT(const Rectangle2D& rectangle1, const Rectangle2D& rectangle2)
{
	Vector2 axisToTest[] = {
		Vector2(1, 0),
		Vector2(0, 1)
	};

	for (int i = 0; i < 2; ++i) 
	{
		if (!OverlapOnAxis(rectangle1, rectangle2, axisToTest[i]))
		{
			return false; // No collision has taken place
		}
	}

	return true; // We have a collision
}
Interval2D GetInterval(const OrientedRectangle& rectangle, const Vector2& axis)
{
	Rectangle2D nonOrientedRect = Rectangle2D(Point2D(rectangle.position - rectangle.halfExtents), rectangle.halfExtents * 2.0f);
	Vector2 min = GetMin(nonOrientedRect);
	Vector2 max = GetMax(nonOrientedRect);
	Vector2 verts[] = { min, max, Vector2(min.x, max.y), Vector2(max.x, min.y) };

	float theta = DEG2RAD(rectangle.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };

	for (int i = 0; i < 4; ++i) {
		Vector2 rotVector = verts[i] - rectangle.position;
		Multiply(rotVector.asArray, Vector2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
		verts[i] = rotVector + rectangle.position;
	}

	// Set interval first projected vertex
	Interval2D result;
	result.min = result.max = Dot(axis, verts[0]);

	// For all other verts
	for (int i = 1; i < 4; ++i) 
	{
		float projection = Dot(axis, verts[i]);
		result.min = (projection < result.min) ? projection : result.min;
		result.max = (projection > result.max) ? projection : result.max;
	}

	return result;
}

bool OverlapOnAxis(const Rectangle2D& rectangle1, const OrientedRectangle& rectangle2, const Vector2& axis)
{
	Interval2D a = GetInterval(rectangle1, axis);
	Interval2D b = GetInterval(rectangle2, axis);
	return ((b.min <= a.max) && (a.min <= b.max));
}

bool RectangleOrientedRectangle(const Rectangle2D& rectangle1, const OrientedRectangle& rectangle2)
{
	Vector2 axisToTest[]
	{
		Vector2(1, 0),
		Vector2(0, 1),
		Normalized(Vector2(rectangle2.halfExtents.x, 0)),
		Normalized(Vector2(0, rectangle2.halfExtents.y))
	};

	float theta = DEG2RAD(rectangle2.rotation);
	float zRotation2x2[] = { cosf(theta), sinf(theta), -sinf(theta), cosf(theta) };
	Multiply(axisToTest[2].asArray, Normalized(Vector2(rectangle2.halfExtents.x, 0)).asArray, 1, 2, zRotation2x2, 2, 2);
	Multiply(axisToTest[3].asArray, Normalized(Vector2(0, rectangle2.halfExtents.y)).asArray, 1, 2, zRotation2x2, 2, 2);

	for (int i = 0; i < 4; ++i) {
		if (!OverlapOnAxis(rectangle1, rectangle2, axisToTest[i])) 
		{
			return false; // No collision has taken place
		}
	}
	return true; // We have a collision
}

bool OverlapOnAxis(const OrientedRectangle& rectangle1, const OrientedRectangle& rectangle2, const Vector2& axis)
{
	Interval2D a = GetInterval(rectangle1, axis);
	Interval2D b = GetInterval(rectangle2, axis);
	return ((b.min <= a.max) && (a.min <= b.max));
}

bool OrientedRectangleOrientedRectangleSAT(const OrientedRectangle& rectangle1, const OrientedRectangle& rectangle2)
{
	Vector2 axisToTest[]{
		Vector2(1, 0),
		Vector2(0, 1),
		Vector2(), // TEMP
		Vector2(), // TEMP
		Vector2(), // TEMP
		Vector2() // TEMP
	};

	// Collision axis of rectangle 2
	float theta = DEG2RAD(rectangle2.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	Multiply(axisToTest[2].asArray, Normalized(Vector2(rectangle2.halfExtents.x, 0)).asArray, 1, 2, zRotation2x2, 2, 2);
	Multiply(axisToTest[3].asArray, Normalized(Vector2(0, rectangle2.halfExtents.y)).asArray, 1, 2, zRotation2x2, 2, 2);

	// Collision axis of rectangle 1
	theta = DEG2RAD(rectangle1.rotation);
	zRotation2x2[0] = cosf(theta);
	zRotation2x2[1] = sinf(theta);
	zRotation2x2[2] = -sinf(theta);
	zRotation2x2[3] = cosf(theta);
	Multiply(axisToTest[4].asArray, Normalized(Vector2(rectangle1.halfExtents.x, 0)).asArray, 1, 2, zRotation2x2, 2, 2);
	Multiply(axisToTest[5].asArray, Normalized(Vector2(0, rectangle1.halfExtents.y)).asArray, 1, 2, zRotation2x2, 2, 2);

	for (int i = 0; i < 6; ++i) {
		if (!OverlapOnAxis(rectangle1, rectangle2, axisToTest[i]))
		{
			return false; // No collision has taken place
		}
	}
	return true; // We have a collision
}


bool OrientedRectangleOrientedRectangle(const OrientedRectangle& rectangle1, const OrientedRectangle& rectangle2)
{
	Rectangle2D localRect1(Point2D(), rectangle1.halfExtents * 2.0f);
	OrientedRectangle localRect2(rectangle2.position, rectangle2.halfExtents, rectangle2.rotation);

	localRect2.rotation = rectangle2.rotation - rectangle1.rotation;
	Vector2 rotVector = rectangle2.position - rectangle1.position;
	float theta = -DEG2RAD(rectangle1.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	Multiply(rotVector.asArray, Vector2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	localRect2.position = rotVector + rectangle1.halfExtents;

	return RectangleOrientedRectangle(localRect1, localRect2);
}

Circle ContainingCircle(Point2D* pointArray, int arrayCount)
{
	Point2D center;
	for (int i = 0; i < arrayCount; ++i) 
	{
		center = center + pointArray[i];
	}
	center = center * (1.0f / (float)arrayCount);

	Circle result(center, 1.0f);

	// Find the squared radius
	result.radius = MagnitudeSq(center - pointArray[0]);
	for (int i = 1; i < arrayCount; ++i) 
	{
		float distance = MagnitudeSq(center - pointArray[i]);
		if (distance > result.radius) 
		{
			result.radius = distance;
		}
	}

	// This has been squared until now
	result.radius = sqrtf(result.radius);
	return result;
}

Rectangle2D ContainingRectangle(Point2D* pointArray, int arrayCount)
{
	Vector2 min = pointArray[0];
	Vector2 max = pointArray[0];
	for (int i = 0; i < arrayCount; i++)
	{
		min.x = pointArray[i].x < min.x ? pointArray[i].x : min.x;
		min.y = pointArray[i].y < min.y ? pointArray[i].y : min.y;
		max.x = pointArray[i].x > max.x ? pointArray[i].x : max.x;
		max.y = pointArray[i].y > max.y ? pointArray[i].y : max.y;
	}
	return FromMinToMax(min, max);
}

bool PointInShape(const BoundingShape& shape, const Point2D& point)
{
	for (int i = 0; i < shape.numCircles; ++i) 
	{
		if (PointInCircle(point, shape.circles[i])) 
		{
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) 
	{
		if (PointInRectangle(point, shape.rectangles[i])) 
		{
			return true;
		}
	}
	return false;
}

Circle ContainingCircleAlt(Point2D* pointArray, int arrayCount)
{
	Vector2 min = pointArray[0];
	Vector2 max = pointArray[0];

	for (int i = 0; i < arrayCount; ++i) {
		min.x = pointArray[i].x < min.x ? pointArray[i].x : min.x;
		min.y = pointArray[i].y < min.y ? pointArray[i].y : min.y;
		max.x = pointArray[i].x > max.x ? pointArray[i].x : max.x;
		max.y = pointArray[i].y > max.y ? pointArray[i].y : max.y;
	}

	return Circle((min + max) * 0.5f, Magnitude(max - min) * 0.5f);
}

#ifdef NO_EXTRAS

bool LineShape(const Line2D& line, const BoundingShape& shape)
{
	for (int i = 0; i < shape.numCircles; ++i)
	{
		if (LineCircle(line, shape.circles[i]))
		{
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) 
	{
		if (LineRectangle(line, shape.rectangles[i])) 
		{
			return true;
		}
	}
	return false;
}

bool CircleShape(const Circle& circle, const BoundingShape& shape)
{
	for (int i = 0; i < shape.numCircles; ++i) 
	{
		if (CircleCircle(circle, shape.circles[i]))
		{
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) 
	{
		if (CircleRectangle(circle, shape.rectangles[i])) 
		{
			return true;
		}
	}
	return false;
}

bool RectangleShape(const Rectangle2D& rectangle, const BoundingShape& shape)
{
	for (int i = 0; i < shape.numCircles; ++i) 
	{
		if (RectangleCircle(rectangle, shape.circles[i]))
		{
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) 
	{
		if (RectangleRectangle(rectangle, shape.rectangles[i]))
		{
			return true;
		}
	}
	return false;
}

bool OrientedRectangleShape(const OrientedRectangle& rectangle, const BoundingShape& shape)
{
	for (int i = 0; i < shape.numCircles; ++i) 
	{
		if (OrientedRectangleCircle(rect, shape.circles[i])) 
		{
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) 
	{
		if (OrientedRectangleRectangle(rect, shape.rectangles[i])) 
		{
			return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const Line2D& shape)
{
	os << "start: (" << shape.start.x << ", " << shape.start.y << "), end: (" << shape.end.x << ", " << shape.end.y << ")\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Circle& shape)
{
	os << "position: (" << shape.position.x << ", " << shape.position.y << "), radius: " << shape.radius << ". \n";
	return os;
}
std::ostream& operator<<(std::ostream& os, const Rectangle2D& shape)
{
	Vector2 min = GetMin(shape);
	Vector2 max = GetMax(shape);

	os << "min: (" << min.x << ", " << min.y << "), max: (" << max.x << ", " << max.y << ") \n";
	return os;
}

std::ostream& operator<<(std::ostream& os, const OrientedRectangle& shape)
{
	os << "position: (" << shape.position.x << ", " << shape.position.y << "), half size: (" << shape.halfExtents.x << ", " << shape.halfExtents.y << "), rotation: " << shape.rotation << ". \n";
	return os;
}

#endif // NO_EXTRAS