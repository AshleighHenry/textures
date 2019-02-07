#ifndef MY_MATRIX
#define MY_MATRIX
#include "MyVector3.h"


class MyMatrix3
{
public:
	MyMatrix3();
	MyMatrix3(
		double t_a11, double t_a12, double t_a13,
		double t_a21, double t_a22, double t_a23,
		double t_a31, double t_a32, double t_a33);
	MyMatrix3(MyVector3 t_row1, MyVector3 t_row2, MyVector3 t_row3);
	~MyMatrix3();

	std::string toString()const;

	bool operator ==(const MyMatrix3 t_other)const;
	bool operator !=(const MyMatrix3 t_other)const;


	MyMatrix3 operator +(const MyMatrix3 t_other) const;
	MyMatrix3 operator -(const MyMatrix3 t_other) const;
	MyMatrix3 operator *(const MyMatrix3 t_other) const;
	MyVector3 operator *(const MyVector3 t_vector)const;
	MyMatrix3 operator *(const double t_scale)const;

	MyMatrix3 transpose()const;
	double determinant() const;
	MyMatrix3 inverse() const;

	MyVector3 row(const int t_row)const; // 0 is first row, then 1, then 2, there is no row 3
	MyVector3 column(const int t_column) const; // 0,1,2 

	
	static MyMatrix3 rotationZ(const double t_angleRadians); // rotate 1,1,1 by PI/2  -> -1,1,1
	static MyMatrix3 rotationY(const double t_angleRadians); // rotate 1,1,1 by PI/2  ->  1,1,-1
	static MyMatrix3 rotationX(const double t_angleRadians); // rotate 1,1,1 by PI/2  ->  1,-1,1

	static MyMatrix3 translation(const MyVector3 t_displacement); // 2d translation make sure z=1
	static MyMatrix3 scale(const double t_scalingfactor);


private:
	double m_11;
	double m_12;
	double m_13;
	double m_21;
	double m_22;
	double m_23;
	double m_31;
	double m_32;
	double m_33;
};

#endif // !MY_MATRIX