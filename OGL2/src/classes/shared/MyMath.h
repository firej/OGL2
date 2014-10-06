/* ������� �������������� ���������� ===========================================================\\
\\==============================================================================================*/
#pragma once
/* ����� : =====================================================================================\\
||	1) �����
||	2) �������
||	3) ������� ���������, ������� � ���������
||	4) ++
\\==============================================================================================*/
#include <Math.h>

#define PI	3.14159265358979f
#define PI2	6.28318530717958f

void InitMATH	(void);								// ������������� �������������� ����������
float __fastcall ulrsqrt(float x);					// 1/�������_������
float __fastcall ulsqrt(float x);					// ������ 

//===[ sin, cos, tan ]============================================================
//======[ � �������� ��������� ��������� ���� ���������� � �������� ]============
extern float _sint [361];
extern float _cost [361];
extern float _tant [361];

float sint	(float angle);
double sint	(double angle);
float cost	(float angle);
double cost	(double angle);
float tant	(float angle);
double tant	(double angle);

//===[ �������+����� ]============================================================
// ����� ��� ���������� �������/�����
template < class VectorComponent > class Vector3
{
public:
// ������������/����������
	// ����������� �����������
	Vector3()
	{d.c.x=0;d.c.y=0;d.c.z=0;}
	// ����������� � ��������������
	Vector3(VectorComponent X, VectorComponent Y, VectorComponent Z) 
	{
		d.c.x = X;
		d.c.y = Y;
		d.c.z = Z;
	}

// ������
	// ������
	double	abs()
	{
		return sqrt(d.c.x*d.c.x+d.c.y*d.c.y+d.c.z*d.c.z);
	}
	// ������������ ������� (���������� ������ � 1)
	void	normalize()
	{
		VectorComponent L_squared, one_over_L;
		L_squared = (d.c.x * d.c.x) + (d.c.y * d.c.y) + (d.c.z * d.c.z);
		one_over_L = (VectorComponent) ulrsqrt((float)L_squared);
		d.c.x *= one_over_L;
		d.c.y *= one_over_L;
		d.c.z *= one_over_L;
	}
    
// ������������ ��������
	// ��������
	Vector3 operator+(Vector3 vVector)
	{
		return Vector3(vVector.d.c.x + d.c.x, vVector.d.c.y + d.c.y, vVector.d.c.z + d.c.z);
	}
	// ���������
	Vector3 operator-(Vector3 vVector)
	{
		return Vector3(d.c.x - vVector.d.c.x, d.c.y - vVector.d.c.y, d.c.z - vVector.d.c.z);
	}
	// ���������
	Vector3 operator*(double	num)
	{return Vector3(d.c.x * num, d.c.y * num, d.c.z * num);}
	Vector3 operator*(float		num)
	{return Vector3(d.c.x * num, d.c.y * num, d.c.z * num);}
	Vector3 operator*(char		num)
	{return Vector3(d.c.x * num, d.c.y * num, d.c.z * num);}
	Vector3 operator*(int num)
	{return Vector3(d.c.x * num, d.c.y * num, d.c.z * num);}
	Vector3 operator*(long int	num)
	{return Vector3(d.c.x * num, d.c.y * num, d.c.z * num);}
	// �������
	Vector3 operator/(double	num)
	{return Vector3(d.c.x / num, d.c.y / num, d.c.z / num);}
	Vector3 operator/(float		num)
	{return Vector3(d.c.x / num, d.c.y / num, d.c.z / num);}
	Vector3 operator/(char		num)
	{return Vector3(d.c.x / num, d.c.y / num, d.c.z / num);}
	Vector3 operator/(int		num)
	{return Vector3(d.c.x / num, d.c.y / num, d.c.z / num);}
	Vector3 operator/(long int	num)
	{return Vector3(d.c.x / num, d.c.y / num, d.c.z / num);}
	Vector3 operator-(const Vector3& other)const
	{ return Vector3(d.c.x - other.d.c.x, d.c.y - other.d.c.y, d.c.z - other.d.c.z);	}
	Vector3 operator+(const Vector3& other)const
	{ return Vector3(d.c.x + other.d.c.x, d.c.y + other.d.c.y, d.c.z + other.d.c.z);	}
	Vector3& operator += (const Vector3 v)
    {	d.c.x += v.d.c.x;
		d.c.y += v.d.c.y;
		d.c.z += v.d.c.z;
		return *this;
	}
	Vector3& operator -= (const Vector3 v)
	{	d.c.x -= v.d.c.x;
		d.c.y -= v.d.c.y;
		d.c.z -= v.d.c.z;
		return *this;
	}
	const Vector3& operator = (const Vector3 &Vect)
	{	if (&Vect == this) return *this;
		this->d.c.x	= Vect.d.c.x;
		this->d.c.y = Vect.d.c.y;
		this->d.c.z = Vect.d.c.z;
		return *this;
	}
	Vector3			vector_product	(Vector3	v)	// ��������� ������������
	{	//	      [ Ax ]   [ Bx ]   | i  j  k  |   [ Ay*Bz-Az*By ]
		//  AxB = [ Ay ] x [ By ] = | Ax Ay Az | = [ Az*Bx-Ax*Bz ]
		//	      [ Az ]   [ Bz ]   | Bx By Bz |   [ Ax*By-Ay*Bx ]
		return Vector3(	this->d.c.y*v.d.c.z - this->d.c.z*v.d.c.y,
						this->d.c.z*v.d.c.x - this->d.c.x*v.d.c.z,
						this->d.c.x*v.d.c.y - this->d.c.y*v.d.c.x);
	}
	VectorComponent	scalar_product	(Vector3	v)	// ��������� ������������
	{	//	[ 1 ]   [ 4 ]
		//	[ 2 ] * [ 5 ] = 1*4 + 2*5 + 3*6 = 32
		//	[ 3 ]   [ 6 ]
		return this->d.c.x * v.d.c.x + this->d.c.y * v.d.c.y + this->d.c.z * v.d.c.z;
	}
	double			angle			(Vector3	v)	// ���� ����� ���������
	{	// cos a = AB / |A|*|B|
		return	acos((double)( this->scalar_product(v) / this->abs()*v.abs() ));
	}
// ����������
	union	Vector3_union_type{
		struct	Vector3_struct_type{
			VectorComponent x, y, z;
				} c ;
			VectorComponent v [3];
			} d ;

};
// ���������� ���� ��������
typedef Vector3 <float> Vector3f;
typedef Vector3 <float> Point3f;
typedef Vector3 <float> Color3f;
typedef Vector3 <double> Vector3d;
typedef Vector3 <double> Point3d;
typedef Vector3 <double> Color3d;
typedef Vector3 <int> Vector3i;
typedef Vector3 <int> Point3i;
typedef Vector3 <int> Color3i;
typedef Vector3 <long int> Vector3l;
typedef Vector3 <long int> Point3l;
typedef Vector3 <long int> Color3l;
typedef Vector3 <char> Vector3c;
typedef Vector3 <char> Point3c;
typedef Vector3 <char> Color3c;

// ����� ��� �����/������� � ���������� ������������		  
template < class VectorComponent > class Vector2 
{
public:
// �����������/�����������
	// ����������� �� ��������
	Vector2() {}

	// ����������� � ��������������
	Vector2(VectorComponent X, VectorComponent Y) 
	{x = X; y = Y;}

// ������
	// ������
	float	abs()
	{return sqrt(x*x+y*y);}
	// ������������
	void	normalize()
	{
		float abs=Vector2::abs();
		x/=abs;
		y/=abs;
	}

// ������������� ��������
	// ��������
	Vector2 operator+(Vector2 vVector)
	{return Vector2(vVector.x + x, vVector.y + y);}
	// ���������
	Vector2 operator-(Vector2 vVector)
	{return Vector2(x - vVector.x, y - vVector.y);}
	// ���������
	Vector2 operator*(double	num)
	{return Vector2(x * num, y * num);}
	Vector2 operator*(float		num)
	{return Vector2(x * num, y * num);}
	Vector2 operator*(char		num)
	{return Vector2(x * num, y * num);}
	Vector2 operator*(int num)
	{return Vector2(x * num, y * num);}
	Vector2 operator*(long int	num)
	{return Vector2(x * num, y * num);}
	// �������
	Vector2 operator/(double	num)
	{return Vector2(x / num, y / num);}
	Vector2 operator/(float		num)
	{return Vector2(x / num, y / num);}
	Vector2 operator/(char		num)
	{return Vector2(x / num, y / num);}
	Vector2 operator/(int		num)
	{return Vector2(x / num, y / num);}
	Vector2 operator/(long int	num)
	{return Vector2(x / num, y / num);}

	Vector2& operator += (const Vector2 v)
    {	x += v.x;
		y += v.y;
		return *this;
	}
	Vector2& operator -= (const Vector2 v)
	{	x -= v.x;
		y -= v.y;
		return *this;
	}
	const Vector2& operator = (const Vector2 &Vect)
	{	if (&Vect == this) return *this;
		this->x	= Vect.x;
		this->y = Vect.y;
		return *this;
	}
	VectorComponent	scalar_product	(Vector2 v)		// ��������� ������������
	{
		return this->d.c.x * v.d.c.x + this->d.c.y * v.d.c.y;
	}

// ����������
	union	Vector2_union_type{
		struct	Vector2_struct_type{
            VectorComponent x, y;
				} c ;
		VectorComponent v [2];
			} d ;
};

// ���������� ��������
typedef Vector2 <float> Vector2f;
typedef Vector2 <float> Point2f;
typedef Vector2 <double> Vector2d;
typedef Vector2 <double> Point2d;
typedef Vector2 <int> Vector2i;
typedef Vector2 <int> Point2i;
typedef Vector2 <char> Vector2c;
typedef Vector2 <char> Point2c;
typedef Vector2 <long int> Vector2l;
typedef Vector2 <long int> Point2l;

// ����� ��� 4�������� �����
template < class ColorComponent > class Color4
{
public:
// ������������/����������
	// ����������� �����������
	Color4()
	{d.c.r=0;d.c.g=0;d.c.b=0;d.c.a=1;}
	// ����������� � ��������������
	Color4(ColorComponent R, ColorComponent G, ColorComponent B, ColorComponent A)
	{
		d.c.r = R;
		d.c.g = G;
		d.c.b = B;
		d.c.a = A;
	}
	Color4(Vector3<ColorComponent> v3, ColorComponent A)
	{
		d.c.r = (ColorComponent)v3.d.c.x;
		d.c.g = (ColorComponent)v3.d.c.y;
		d.c.b = (ColorComponent)v3.d.c.z;
		d.c.a = A;
	}
// ������
	// ������
	double	abs()
	{
		return sqrt(d.c.r*d.c.r+d.c.g*d.c.g+d.c.b*d.c.b);
	}
	// ������������ ������� (���������� ������ � 1)
	void	normalize()
	{
		ColorComponent L_squared, one_over_L;
		L_squared = (d.c.r * d.c.r) + (d.c.g * d.c.g) + (d.c.b * d.c.b);
		one_over_L = (ColorComponent) ulrsqrt((float)L_squared);
		d.c.r *= one_over_L;
		d.c.g *= one_over_L;
		d.c.b *= one_over_L;
	}
    
// ������������ ��������
	// ��������
	Color4 operator+(Color4 vVector)
	{
		return Color4(vVector.d.c.r + d.c.r, vVector.d.c.g + d.c.g, vVector.d.c.b + d.c.b);
	}
	// ���������
	Color4 operator-(Color4 vVector)
	{
		return Color4(d.c.r - vVector.d.c.r, d.c.g - vVector.d.c.g, d.c.b - vVector.d.c.b);
	}
	// ���������
	Color4 operator*(double	num)
	{return Color4(d.c.r * num, d.c.g * num, d.c.b * num, d.c.a);}
	Color4 operator*(float		num)
	{return Color4(d.c.r * num, d.c.g * num, d.c.b * num, d.c.a);}
	Color4 operator*(char		num)
	{return Color4(d.c.r * num, d.c.g * num, d.c.b * num, d.c.a);}
	Color4 operator*(int num)
	{return Color4(d.c.r * num, d.c.g * num, d.c.b * num, d.c.a);}
	Color4 operator*(long int	num)
	{return Color4(d.c.r * num, d.c.g * num, d.c.b * num, d.c.a);}
	// �������
	Color4 operator/(double	num)
	{return Color4(d.c.r / num, d.c.g / num, d.c.b / num, d.c.a);}
	Color4 operator/(float		num)
	{return Color4(d.c.r / num, d.c.g / num, d.c.b / num, d.c.a);}
	Color4 operator/(char		num)
	{return Color4(d.c.r / num, d.c.g / num, d.c.b / num, d.c.a);}
	Color4 operator/(int		num)
	{return Color4(d.c.r / num, d.c.g / num, d.c.b / num, d.c.a);}
	Color4 operator/(long int	num)
	{return Color4(d.c.r / num, d.c.g / num, d.c.b / num, d.c.a);}
	Color4 operator-(const Color4& other)const
	{ return Color4(d.c.r - other.d.c.r, d.c.g - other.d.c.g, d.c.b - other.d.c.b, 1);	}
	Color4 operator+(const Color4& other)const
	{ return Color4(d.c.r + other.d.c.r, d.c.g + other.d.c.g, d.c.b + other.d.c.b, 1);	}
	Color4& operator += (const Color4 v)
    {	d.c.r += v.d.c.r;
		d.c.g += v.d.c.g;
		d.c.b += v.d.c.b;
		return *this;
	}
	Color4& operator -= (const Color4 v)
	{	d.c.r -= v.d.c.r;
		d.c.g -= v.d.c.g;
		d.c.b -= v.d.c.b;
		return *this;
	}
	const Color4& operator = (const Color4 &Vect)
	{	if (&Vect == this) return *this;
		this->d.c.r	= Vect.d.c.r;
		this->d.c.g = Vect.d.c.g;
		this->d.c.b = Vect.d.c.b;
		this->d.c.a = Vect.d.c.a;
		return *this;
	}
	Color4	vector_product (Color4 v)		// ��������� ������������
	{
	//	      [ Ax ]   [ Bx ]   | i  j  k  |   [ Ay*Bz-Az*By ]
	//  AxB = [ Ay ] x [ By ] = | Ax Ay Az | = [ Az*Bx-Ax*Bz ]
	//	      [ Az ]   [ Bz ]   | Bx By Bz |   [ Ax*By-Ay*Bx ]
		return Color4(	this->d.c.g*v.d.c.b - this->d.c.b*v.d.c.g,
						this->d.c.b*v.d.c.r - this->d.c.r*v.d.c.b,
						this->d.c.r*v.d.c.g - this->d.c.g*v.d.c.r);
	}
// ����������
	union	Color4_union_type{
		struct	Color4_struct_type{
			ColorComponent r, g, b, a;
				} c ;
			ColorComponent v [4];
			} d ;

};

typedef Color4 <float> Vector4f;
typedef Color4 <float> Color4f;
typedef Color4 <double> Vector4d;
typedef Color4 <double> Color4d;
typedef Color4 <int> Vector4i;
typedef Color4 <int> Color4i;
typedef Color4 <long int> Vector4l;
typedef Color4 <long int> Color4l;
typedef Color4 <char> Vector4c;
typedef Color4 <char> Color4c;
