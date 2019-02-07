#include "InitialMyMatrix3.h"

MyMatrix3::MyMatrix3()
{	// first row
	m_11 = 0.0;
	m_12 = 0.0;
	m_13 = 0.0;
	// second row
	m_21 = 0.0;
	m_22 = 0.0;
	m_23 = 0.0;
	// third row
	m_31 = 0.0;
	m_32 = 0.0;

	m_33 = 0.0;
}

MyMatrix3::MyMatrix3(double t_a11, double t_a12, double t_a13, double t_a21, double t_a22, double t_a23, double t_a31, double t_a32, double t_a33)
{	// first row
	m_11 = t_a11;
	m_12 = t_a12;
	m_13 = t_a13;
	// second row
	m_21 = t_a21;
	m_22 = t_a22;
	m_23 = t_a23;
	// third row
	m_31 = t_a31;
	m_32 = t_a32;
	m_33 = t_a33;
}

MyMatrix3::MyMatrix3(MyVector3 t_row1, MyVector3 t_row2, MyVector3 t_row3)
{	// assign t_row 1 values to the first row of the matrice
	m_11 = t_row1.x;
	m_12 = t_row1.y;
	m_13 = t_row1.z;
	// assign t_row 2 values to the second row of the matrice
	m_21 = t_row2.x;
	m_22 = t_row2.y;
	m_23 = t_row2.z;
	// assign t_row 3 values to the third row of the matrice 
	m_31 = t_row3.x;
	m_32 = t_row3.y;
	m_33 = t_row3.z;
}

MyMatrix3::~MyMatrix3()
{
	// destructor 
}

std::string MyMatrix3::toString() const
{
	const std::string message = "[" + std::to_string(m_11) + ", " + std::to_string(m_12) + ", " + std::to_string(m_13) +", " 
									+ std::to_string(m_21) + ", " + std::to_string(m_22) + ", " + std::to_string(m_23) + ", "
									+ std::to_string(m_31) + ", " + std::to_string(m_32) + ", " + std::to_string(m_33) + "]";
	return std::string(message);
}

bool MyMatrix3::operator==(const MyMatrix3 t_other) const
{	// check each value of the matrices against each other. if any single is not the same as it's correspondent return false
	if (   m_11 == t_other.m_11 && m_12 == t_other.m_12 && m_13 == t_other.m_13
		&& m_21 == t_other.m_21 && m_22 == t_other.m_22 && m_23 == t_other.m_23
		&& m_31 == t_other.m_31 && m_32 == t_other.m_32 && m_33 == t_other.m_33	)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

bool MyMatrix3::operator!=(const MyMatrix3 t_other) const
{	// exact opposite of the == operator. if ever value does not equal the other return true, else return false
	if (   m_11 != t_other.m_11 && m_12 != t_other.m_12 && m_13 != t_other.m_13
		&& m_21 != t_other.m_21 && m_22 != t_other.m_22 && m_23 != t_other.m_23
		&& m_31 != t_other.m_31 && m_32 != t_other.m_32 && m_33 != t_other.m_33	)
	{
		return true;
	}
	else
	{
		return false;
	}
}

MyMatrix3 MyMatrix3::operator+(const MyMatrix3 t_other) const
{   // adds the two matrices together 
	MyMatrix3 answer = MyMatrix3{ m_11 + t_other.m_11, m_12 + t_other.m_12, m_13 + t_other.m_13,
								  m_21 + t_other.m_21, m_22 + t_other.m_22, m_23 + t_other.m_23,
								  m_31 + t_other.m_31, m_32 + t_other.m_32, m_33 + t_other.m_33 };
	return MyMatrix3(answer);
}

MyMatrix3 MyMatrix3::operator-(const MyMatrix3 t_other) const
{	// subtracts the two matrices from each other
	MyMatrix3 answer = MyMatrix3{ m_11 - t_other.m_11, m_12 - t_other.m_12, m_13 - t_other.m_13,
								  m_21 - t_other.m_21, m_22 - t_other.m_22, m_23 - t_other.m_23,
								  m_31 - t_other.m_31, m_32 - t_other.m_32, m_33 - t_other.m_33 };
	return MyMatrix3(answer);
}

MyMatrix3 MyMatrix3::operator*(const MyMatrix3 t_other) const
{
	// its just multiplying matrices 
	MyVector3 topRow = { (m_11 * t_other.m_11) + (m_12 * t_other.m_21) + (m_13* t_other.m_31),
						 (m_11 * t_other.m_12) + (m_12 * t_other.m_22) + (m_13* t_other.m_32),
						 (m_11 * t_other.m_13) + (m_12 * t_other.m_23) + (m_13* t_other.m_33) };

	MyVector3 secondRow = { (m_21 * t_other.m_11) + (m_22 * t_other.m_21) + (m_23* t_other.m_31),
							(m_21 * t_other.m_12) + (m_22 * t_other.m_22) + (m_23* t_other.m_32),
							(m_21 * t_other.m_13) + (m_22 * t_other.m_23) + (m_23* t_other.m_33) };

	MyVector3 ThirdRow = { (m_31 * t_other.m_11) + (m_32 * t_other.m_21) + (m_33* t_other.m_31),
						   (m_31 * t_other.m_12) + (m_32 * t_other.m_22) + (m_33* t_other.m_32),
						   (m_31 * t_other.m_13) + (m_32 * t_other.m_23) + (m_33* t_other.m_33) };
	
	return MyMatrix3(topRow,secondRow,ThirdRow);
}

MyVector3 MyMatrix3::operator*(const MyVector3 t_vector) const
{
	// multiplying matrices by a 3d vector 
	/*MyVector3 answer = {(m_11 * t_vector.x) + (m_21 * t_vector.y) + (m_31 * t_vector.z),
						(m_12 * t_vector.x) + (m_22 * t_vector.y) + (m_32 * t_vector.z),
						(m_13 * t_vector.x) + (m_23 * t_vector.y) + (m_33 * t_vector.z) };

	return MyVector3(answer);*/

	return MyVector3(row(0).dot(t_vector), row(1).dot(t_vector), row(2).dot(t_vector));
}

MyMatrix3 MyMatrix3::operator*(const double t_scale) const
{
	// multiply every value in the matrix by a scalar.
	MyMatrix3 answer = { m_11 * t_scale, m_12 * t_scale, m_13 * t_scale,
						 m_21 * t_scale, m_22 * t_scale, m_23 * t_scale,
						 m_31 * t_scale, m_32 * t_scale, m_33 * t_scale };
	

	return MyMatrix3(answer);
}

MyMatrix3 MyMatrix3::transpose() const
{ // swap rows and columns of the matrix to find the transpose 
	MyMatrix3 answer = { m_11, m_21, m_31,
						 m_12, m_22, m_32,
						 m_13, m_23, m_33 };
	return MyMatrix3(answer);
}

double MyMatrix3::determinant() const
{	
	double answer = m_11 * ((m_22 * m_33) - (m_32 * m_23)) - m_21 * ((m_33 * m_12) - (m_32*m_13)) + m_31 *((m_23*m_12) - (m_22 *m_13));
	return answer;
}

MyMatrix3 MyMatrix3::inverse() const
{	
	double deter = 1.0 / determinant();
	MyVector3 rowOne = { ((m_33 * m_22)  - (m_32 * m_23)),((m_32 * m_13) - (m_33 * m_12)), ((m_23 * m_12) - (m_22 * m_13)) };
	MyVector3 rowTwo = { ((m_31 * m_23) - (m_33 * m_21)),((m_33 * m_11) - (m_31 * m_13)), ((m_21 * m_13) - (m_23 * m_11)) };
	MyVector3 rowThr = { ((m_32 * m_21) - (m_31 * m_22)),((m_31 * m_12) - (m_32 * m_11)), ((m_22 * m_11) - (m_21 * m_12)) };
	MyMatrix3 answer = { rowOne,rowTwo,rowThr };
	return MyMatrix3(answer * deter);
}

MyVector3 MyMatrix3::row(const int t_row) const
{
	MyVector3 answer;
	switch (t_row)
	{
	case 0:
		answer = { m_11, m_12, m_13 };
		break;
	case 1:
		answer = { m_21, m_22, m_23 };
		break;
	case 2:
		answer = { m_31, m_32, m_33 };
		break;
	default:
		std::cout << "error, invalid row num" << std::endl;
		break;
	}
	return MyVector3(answer);
}

MyVector3 MyMatrix3::column(const int t_column) const
{
	MyVector3 answer;
	switch (t_column)
	{
	case 0:
		answer = { m_11, m_21, m_31 };
		break;
	case 1:
		answer = { m_12, m_22, m_32 };
		break;
	case 2:
		answer = { m_13, m_23, m_33 };
		break;
	default:
		std::cout << "error, invalid column num" << std::endl;
		break;
	}
	return MyVector3(answer);
}

MyMatrix3 MyMatrix3::rotationZ(const double t_angleRadians)
{
	MyVector3 topRow = { cos(t_angleRadians), -sin(t_angleRadians), 0 };
	MyVector3 midRow = { sin(t_angleRadians), cos(t_angleRadians), 0 };
	MyVector3 botRow = { 0,0,1 };
	return MyMatrix3(topRow,midRow,botRow);
}

MyMatrix3 MyMatrix3::rotationY(const double t_angleRadians)
{
	MyVector3 topRow = { cos(t_angleRadians), 0, sin(t_angleRadians) };
	MyVector3 midRow = { 0,1,0 };
	MyVector3 botRow = { -sin(t_angleRadians), 0, cos(t_angleRadians) };
	return MyMatrix3(topRow,midRow,botRow);
}

MyMatrix3 MyMatrix3::rotationX(const double t_angleRadians)
{
	MyVector3 topRow = { 1,0,0 };
	MyVector3 midRow = { 0, cos(t_angleRadians),-sin(t_angleRadians) };
	MyVector3 botRow = { 0, sin(t_angleRadians), cos(t_angleRadians) };

	return MyMatrix3(topRow,midRow,botRow);
}

MyMatrix3 MyMatrix3::translation(const MyVector3 t_displacement)
{
	
		MyMatrix3 answer = { 1,0,t_displacement.x,
							 0,1,t_displacement.y,
							 0,0,1 };
	return MyMatrix3(answer);
}

MyMatrix3 MyMatrix3::scale(const double t_scalingfactor)
{
	MyMatrix3 answer = { t_scalingfactor, 0, 0,
						 0, t_scalingfactor,0,
						 0,0,t_scalingfactor };
	return MyMatrix3(answer);
}

