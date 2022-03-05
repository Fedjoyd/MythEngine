
#include "maths/Vec2.hpp"

Vec2 const Vec2::Right(1, 0);
Vec2 const Vec2::Left(-1, 0);
Vec2 const Vec2::Up(0, 1);
Vec2 const Vec2::Down(0, -1);
Vec2 const Vec2::One(1, 1);
Vec2 const Vec2::Zero(0, 0);

Vec2::Vec2()
	:m_x{0}, m_y{0}
{
}

Vec2::Vec2( Vec2 from, Vec2 to )
: 	m_x{to.m_x - from.m_x},
  	m_y{to.m_y - from.m_y}
{
}

Vec2::Vec2(float x, float y)
	: m_x{ x },
	m_y{ y }
{
}
Vec2::Vec2(float xy)
	: m_x{ xy },
	m_y{ xy }
{
}

Vec2::Vec2( Vec2 const& copy )
:   m_x{ copy.m_x },
	m_y{ copy.m_y }
{

}
float Vec2::AngleDeg (Vec2 const& from, Vec2 const& to)     //(Angle SIGNE (entre -180 et 180) en degree !)
{
    return Maths::RadToDeg( AngleRad( from, to ));
}
float Vec2::AngleRad (Vec2 const& from, Vec2 const& to)     //(Angle SIGNE (entre -180 et 180) en radian !)
{
    return Maths::Acos( Dot( from, to ) / Maths::Sqrt(from.SquaredLength() * to.SquaredLength()));
}
Vec2 Vec2::Clamp	(Vec2 const& value, Vec2 const& min, Vec2 const& max)
{
	return Vec2( Maths::Clamp( value.m_x, min.m_x, max.m_x ), Maths::Clamp( value.m_y, min.m_y, max.m_y ));
}
Vec2 Vec2::ClampMagnitude (Vec2 const& vector, float const minMagnitude, float const    maxMagnitude)
{
	float const VecMagnitude = vector.Length();

	if(Maths::IsNearlyZero(VecMagnitude))
	{
		return Vec2::Zero;
	}

    return Maths::Clamp( VecMagnitude, minMagnitude, maxMagnitude ) * vector / VecMagnitude;
}
Vec2 Vec2::ClampMaxMagnitude (Vec2 const& vector, float const maxMagnitude)
{
	if(maxMagnitude < MATH_EPSILON)
	{
		return Vec2::Zero;
	}

	float const SquaredLen{ vector.SquaredLength()};

	if( SquaredLen > Maths::Square(maxMagnitude))
	{
		return vector * Maths::InvSqr(SquaredLen) * maxMagnitude;
	}
	else
	{
		return vector;
	}
}
Vec2 Vec2::ClampMinMagnitude (Vec2 const& vector, float const minMagnitude)
{
	if(minMagnitude < MATH_EPSILON)
	{
		return Vec2::Zero;
	}
	float const SquaredLen{ vector.SquaredLength()};

	if( SquaredLen < Maths::Square(minMagnitude))
	{
		return vector * Maths::InvSqr(SquaredLen) * minMagnitude;
	}
	else
	{
		return vector;
	}
}
float Vec2::Distance (Vec2 const& a, Vec2 const& b)
{
    return Maths::Sqrt(DistanceSquared(a, b));
}
float Vec2::DistanceSquared (Vec2 const& a, Vec2 const& b)
{
    return Maths::Square(b.m_x - a.m_x)+ Maths::Square(b.m_y - a.m_y);
}
bool Vec2::IsCollinear (Vec2 const& a,Vec2 const& b) // a finir
{
    return Maths::IsNearlyZero( a.m_x * b.m_y - b.m_x *a.m_y );
}
bool Vec2::IsNearlyEqual (Vec2 const& a, Vec2 const& b,float const tolerance)
{
    return Maths::IsNearlyEqual(a.m_x, b.m_x, tolerance) && Maths::IsNearlyEqual(a.m_y, b.m_y, tolerance); 
}
bool Vec2::IsOrthogonal (Vec2 const& a,Vec2 const& b)
{
    return Maths::IsNearlyZero(Dot(a, b));
}
Vec2 Vec2::Lerp (Vec2 const& a,Vec2 const& b,float const alpha)
{
	return a + (b - a) * alpha;
}
Vec2 Vec2::Max (Vec2 const& a, Vec2 const& b)
{
    return { Maths::Max(a.m_x, b.m_x), Maths::Max(a.m_y, b.m_y) };
}
Vec2 Vec2:: Min(Vec2 const& a, Vec2 const& b)
{
    return { Maths::Min(a.m_x, b.m_x), Maths::Min(a.m_y, b.m_y) };
}
Vec2 Vec2::MoveTowards (Vec2 const& current, Vec2 const& target,float const    maxDistanceDelta)
{
	Vec2 const CrtToTarget{current, target};
	float const distance = CrtToTarget.SquaredLength();

	if(distance <= maxDistanceDelta || Maths::IsNearlyZero(distance))
	{
		return target;
	}
    return current + CrtToTarget/distance * maxDistanceDelta;
}
Vec2 Vec2::Project (Vec2 const& vector, Vec2 const& onTarget)
{
    return onTarget * ( Dot( onTarget, vector ) / onTarget.SquaredLength());
}
Vec2 Vec2::ProjectOnNormal (Vec2 const& vector, Vec2 const& onNormal)
{
    return onNormal * Dot(onNormal, vector);
}
Vec2 Vec2::Reflect (Vec2 const& direction, Vec2 const& normal)
{
	return Dot( direction, normal ) * -2 * normal + direction;
}
Vec2 Vec2::ReflectInvert (Vec2 const& direction, Vec2 const& normal)
{
	return Dot(direction, normal) * 2 * normal - direction;
}
Vec2 Vec2::RotateDeg (Vec2 const& vector, float const    angleDeg)
{
	return RotateRad(vector, Maths::DegToRad(angleDeg));
}
Vec2 Vec2::RotateRad (Vec2 const& vector, float const    angleRad)
{
	float cosTheta = Maths::Cos(angleRad), 
		  sinTheta = Maths::Sin(angleRad);

	return { vector.m_x * cosTheta - vector.m_y * sinTheta, 
		     vector.m_x * sinTheta + vector.m_y * cosTheta };
}
float Vec2::SignedAngleDeg (Vec2 const& from, Vec2 const& to)
{
	float angle = Maths::Atan2(to.m_y, to.m_x) - Maths::Atan2(from.m_y, from.m_x);
	if (angle > PI)
	{
		angle -= TAU;
	}
	else if (angle <= -PI)
	{
		angle += TAU;
	}
	return angle;
}
float Vec2::SignedAngleRad (Vec2 const& from, Vec2 const& to)
{
    return Maths::DegToRad( SignedAngleDeg(from, to) ); 
}
float Vec2::SquaredLength() const
{
    return m_x*m_x + m_y*m_y;
}
float Vec2::Length() const
{
    return Maths::Sqrt( SquaredLength() );
}
float   Vec2::Dot(Vec2 const& a, Vec2 const& b)
{
    return a.m_x * b.m_x + a.m_y * b.m_y;
}

Vec2 Vec2::GetAbs() const
{
	return { Maths::Abs(m_x), Maths::Abs(m_y) };
}
float Vec2::GetAbsMax() const
{
	return Maths::Max(Maths::Abs(m_x), Maths::Abs(m_y));
}
float Vec2::GetAbsMin() const
{
	return Maths::Min(Maths::Abs(m_x), Maths::Abs(m_y));
}
float Vec2::GetMax() const
{
	return Maths::Max(m_x, m_y);
}
float Vec2::GetMin() const
{
	return Maths::Min(m_x, m_y);
}
Vec2 Vec2::GetPerpendicular() const
{
	return { -m_y, m_x };
}
Vec2 Vec2::GetSign() const
{
	return { (m_x >= 0) ? 1.f : -1.f,
			 (m_y >= 0) ? 1.f : -1.f };
}
Vec2 Vec2::GetSafeNormalized(float const tolerance) const
{
	float _SquaredLength = SquaredLength();

	if (_SquaredLength < tolerance)
	{
		return Vec2::Zero;
	}
	else if (_SquaredLength == 1)
	{
		return *this;
	}
	_SquaredLength = Maths::Sqrt(_SquaredLength);

	return { m_x / _SquaredLength, m_y / _SquaredLength };
}
Vec2 Vec2::GetUnsafeNormalized() const
{
	float VecLength = Length();
	return { m_x / VecLength, m_y / VecLength};
}
bool Vec2::IsNearlyZero(float const    tolerance) const
{
	return Maths::IsNearlyZero(m_x, tolerance) && Maths::IsNearlyZero(m_y, tolerance);

}
bool Vec2::IsNormalized() const
{
	return Maths::IsNearlyEqual(SquaredLength(), 1);

}
bool Vec2::IsUniform(float const tolerance) const
{
	return Maths::IsNearlyEqual(m_x, m_y, tolerance);
}
bool Vec2::IsUnit(float const lengthSquaredTolerance)  const
{
	return Maths::IsNearlyEqual(SquaredLength(), 1, lengthSquaredTolerance);
}
bool Vec2::IsZero() const
{
	return m_x == 0 && m_y == 0;
}

void Vec2::Normalize() 
{
    float length = Length();
    m_x = m_x/length;
    m_y = m_y/ length;
}
void Vec2::Set (float const _value)
{
	m_x = m_y = _value;
}
void Vec2::Set (float const p_x, float const    p_y)
{
	m_x = p_x;
	m_y = p_y;
}
void Vec2::ToDirectionAndLength (Vec2& _outDirection, float& _outLength)const
{
	_outLength = Length();
	_outDirection.m_x = 	m_x/_outLength;
	_outDirection.m_y = 	m_y/_outLength;
}
bool        Vec2::operator==  (Vec2 const& vector)   const
{
	return m_x == vector.m_x && m_y == vector.m_y;
}
bool        Vec2::operator!=  (Vec2 const& vector)   const
{
	return m_x != vector.m_x || m_y != vector.m_y;
}
Vec2&    Vec2::operator=   (Vec2 const& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	return *this;
}
float       Vec2::operator[]  (unsigned int const    index)    const
{
	if(index < 0 || index > 1)
	{
		throw std::out_of_range("Index is out of range, should be between 0 and 1");
	}
	return (index == 1) ? m_y : m_x;
}
float&      Vec2::operator[]  (unsigned int const    index)
{
	if(index < 0 || index > 1)
	{
		throw std::out_of_range("Index is out of range, should be between 0 and 1");
	}
	return (index == 1) ? m_y : m_x;
}
Vec2     Vec2::operator+   (Vec2 const& vector)   const
{
	return { m_x + vector.m_x , m_y + vector.m_y };
}
Vec2     Vec2::operator+   (float const    value)    const
{
	return {m_x + value, m_y + value};
}
Vec2&    Vec2::operator+=  (Vec2 const& vector)
{
	m_x += vector.m_x;
	m_y += vector.m_y;
	return *this;
}
Vec2&    Vec2::operator+=  (float const    value)
{
	m_x += value;
	m_y += value;
	return *this;
}
Vec2     Vec2::operator-   ()                          const
{
	return { -m_x, -m_y };
}
Vec2     Vec2::operator-   (Vec2 const& vector)   const
{
	return { m_x - vector.m_x , m_y - vector.m_y };

}
Vec2     Vec2::operator-   (float const    value)    const
{
	return { m_x - value , m_y - value };

}
Vec2&    Vec2::operator-=  (Vec2 const& vector)
{
	m_x -= vector.m_x;
	m_y -= vector.m_y;
	return *this;
}
Vec2&    Vec2::operator-=  (float const    value)
{
	m_x -= value;
	m_y -= value;
	return *this;
}
Vec2     Vec2::operator*   (Vec2 const& vector)   const
{
	return { m_x * vector.m_x, m_y * vector.m_y };
}
Vec2     Vec2::operator*   (float const    scale)    const
{
	return { m_x * scale, m_y * scale };
}
Vec2&    Vec2::operator*=  (Vec2 const& vector)
{
	m_x *= vector.m_x;
	m_y *= vector.m_y;
	return *this;
}
Vec2&    Vec2::operator*=  (float const    scale)
{
	m_x *= scale;
	m_y *= scale;
	return *this;
}
Vec2     Vec2::operator/   (Vec2 const& vector)const
{
	return { m_x / vector.m_x, m_y / vector.m_y };
}
Vec2     Vec2::operator/   (float const    scale) const
{
	return { m_x / scale, m_y / scale };
}
Vec2&    Vec2::operator/=  (Vec2 const& vector)
{
	m_x /= vector.m_x;
	m_y /= vector.m_y;
	return *this;
}
Vec2&    Vec2::operator/=  (float const    scale)
{
	m_x /= scale;
	m_y /= scale;
	return *this;
}
//float operator| (Vector3 const& vector) const; /*(C'est un operateur pour le dot product, OPTIONNEL)*/
Vec2 operator+ (float const value, Vec2 const& vector)
{
	return  vector + value;
}
Vec2 operator* (float const scale, Vec2 const& vector)
{
	return vector * scale;
}
