/// <summary>
/// Vector 3 class
/// Ashleigh Henry xoxo
/// </summary>
#include "MyVector3.h"
#include <string.h>
#define PI           3.14159265358979323846

/// <summary>
/// Defautl constructor set all 3 values to zero
/// </summary>
MyVector3::MyVector3() :
	x{ 0.0 },
	y{ 0.0 },
	z{ 0.0 }
{
}

/// <summary>

/// default destructor
/// nothing to release because we only have
/// primitive type data
/// </summary>
MyVector3::~MyVector3()
{
}

/// <summary>
/// 
/// </summary>
/// <returns>the vector as a string surrounded by square brackets and seperated by commas</returns>
std::string MyVector3::toString()
{
	const std::string output = "[" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "]";
	return output;
}

MyVector3::MyVector3(double t_x, double t_y, double t_z)
{
	x = { t_x };
	y = { t_y };
	z = { t_z };

}

MyVector3::MyVector3(sf::Vector3f t_sfVector)
{
	x = static_cast<float>(t_sfVector.x);
	y = static_cast<float>(t_sfVector.y);
	z = static_cast<float>(t_sfVector.z);
}

MyVector3::MyVector3(sf::Vector3i t_sfVector)
{
	// casts to 3D int
	x = static_cast<int>(t_sfVector.x);
	y = static_cast<int>(t_sfVector.y);
	z = static_cast<int>(t_sfVector.z);
}

MyVector3::MyVector3(sf::Vector2i t_sfVector)
{
	// casts to 2D int
	x = static_cast<int>(t_sfVector.x);
	y = static_cast<int>(t_sfVector.y);
	z = 0;
}

MyVector3::MyVector3(sf::Vector2u t_sfVector)
{
	// casts to unsigned
	x = static_cast<unsigned>(t_sfVector.x);
	y = static_cast<unsigned>(t_sfVector.y);
	z = 0;
}

MyVector3::MyVector3(sf::Vector2f t_sfVector)
{
	// casts to 2d float
	x = static_cast<float>(t_sfVector.x);
	y = static_cast<float>(t_sfVector.y);
	z = 0;
}

bool MyVector3::operator==(const MyVector3 t_right) const
{
	// equality check
	const bool answer{ x == t_right.x && y == t_right.y && z == t_right.z };
	return answer;

}

bool MyVector3::operator!=(const MyVector3 t_right) const
{
	const bool result{ x != t_right.x || y != t_right.y || z != t_right.z };
	return result;
}

MyVector3 MyVector3::operator+(const MyVector3 t_right) const
{
	return MyVector3(x + t_right.x, y + t_right.y, z + t_right.z);
}

MyVector3 MyVector3::operator-(const MyVector3 t_right) const
{
	return MyVector3(x - t_right.x, y - t_right.y, z - t_right.z);
}

MyVector3 MyVector3::operator*(const double t_scalar) const
{
	return MyVector3(x * t_scalar, y * t_scalar, z * t_scalar);
}

MyVector3 MyVector3::operator/(const double t_divisor) const
{
	// can't divide by zero so error check and returns a value which is checked for in the main and gives error message

	if (t_divisor == 0)
	{
		return MyVector3(0, 0, 0);
	}
	else
	{
		return MyVector3(x / t_divisor, y / t_divisor, z / t_divisor);
	}

}

MyVector3 MyVector3::operator+=(const MyVector3 t_right)
{
	x += t_right.x;
	y += t_right.y;
	z += t_right.z;
	return MyVector3(x, y, z);
}

MyVector3 MyVector3::operator-=(const MyVector3 t_right)
{
	x -= t_right.x;
	y -= t_right.y;
	z -= t_right.z;
	// x2 - X1, Y2 - Y1, z2 - z1 

	return MyVector3(x, y, z);
}

MyVector3 MyVector3::operator-()
{
	return MyVector3((x*-1), (y*-1), (z*-1));
}


double MyVector3::length() const
{
	double length = { 0 };
	length = std::sqrt(x * x + y * y + z * z);  //lenth of vector is h. h is x*x + y*y then squared
	return length;

}

double MyVector3::lengthSquared() const
{
	double lengthTwo = length() * length();
	return lengthTwo;
}

double MyVector3::dot(const MyVector3 t_other) const
{
	double dot = (x * t_other.x) + (y * t_other.y) + (z * t_other.z);
	return dot;

}

MyVector3 MyVector3::crossProduct(const MyVector3 t_other) const
{
	//(y * t_other.z - z * t_other.y),(z * t_other.x - x * t_other.z), (x * t_other.y - y * t_other.x)
	return MyVector3((y * t_other.z - z * t_other.y), (z * t_other.x - x * t_other.z), (x * t_other.y - y * t_other.x));
}

double MyVector3::angleBetween(const MyVector3 t_other) const
{
	double topLine = dot(t_other);
	double bottomLine = t_other.length() * length();
	double angle = acos(topLine / bottomLine) * (180 / PI);
	return angle;
}


MyVector3 MyVector3::unit() const
{
	double length = sqrt((x*x) + (y*y) + (z*z));
	return MyVector3((x / length), (y / length), (z / length));

}

void MyVector3::normalise()
{
	double lengthOfVector = length();
	x = x / lengthOfVector;
	y = y / lengthOfVector;
	z = z / lengthOfVector;
}

MyVector3 MyVector3::projection(const MyVector3 t_other) const
{
	// have to project each individual point
	double projectionX = x * dot(t_other) / lengthSquared();
	double projectionY = y * dot(t_other) / lengthSquared();
	double projectionZ = z * dot(t_other) / lengthSquared();

	return MyVector3(projectionX, projectionY, projectionZ);
}

MyVector3 MyVector3::rejection(const MyVector3 t_other) const
{
	return MyVector3(MyVector3(t_other) - projection(t_other)); // vector - projection
}





