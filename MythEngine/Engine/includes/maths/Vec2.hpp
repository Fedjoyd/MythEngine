#ifndef _VECTOR_2_HPP_
#define _VECTOR_2_HPP_
#include <stdexcept>
#include "Maths.hpp"
#include <iostream>

class Vec2
{
public:
    static Vec2 const Right;
    static Vec2 const Left;
    static Vec2 const Up;
    static Vec2 const Down;
    static Vec2 const One;
    static Vec2 const Zero;

    float   m_x, 
            m_y;

    Vec2();
    Vec2( Vec2 from, Vec2 to );
    Vec2( float x, float y );
    Vec2( float xy );
    Vec2( Vec2 const& copy );
    ~Vec2() = default;

    //Methods static :
	static float AngleDeg   (Vec2 const& from, Vec2 const& to); //(Angle SIGNE (entre -180 et 180) en degree !)
	static float AngleRad   (Vec2 const& from, Vec2 const& to); //(Angle SIGNE (entre -180 et 180) en radian !)

	static Vec2 Clamp	           (Vec2 const& value, Vec2 const& min, Vec2 const& max);
    static Vec2 ClampMagnitude      (Vec2 const& vector, float const minMagnitude, float const    maxMagnitude);
    static Vec2 ClampMaxMagnitude   (Vec2 const& vector, float const maxMagnitude);
    static Vec2 ClampMinMagnitude   (Vec2 const& vector, float const minMagnitude);

    static float Distance            (Vec2 const& a, Vec2 const& b);
    static float DistanceSquared     (Vec2 const& a, Vec2 const& b);
    static float Dot                 (Vec2 const& a, Vec2 const& b);

    static bool IsCollinear     (Vec2 const& a,Vec2 const& b);
    static bool IsNearlyEqual   (Vec2 const& a, Vec2 const& b,float const tolerance = MATH_EPSILON_F);
    static bool IsOrthogonal    (Vec2 const& a,Vec2 const& b);

    static Vec2 Lerp                (Vec2 const& a,Vec2 const& b,float const alpha);
    static Vec2 Max                 (Vec2 const& a, Vec2 const& b);
    static Vec2 Min                 (Vec2 const& a, Vec2 const& b);
    static Vec2 MoveTowards         (Vec2 const& current, Vec2 const& target,float const maxDistanceDelta);
    static Vec2 Project             (Vec2 const& vector, Vec2 const& onTarget);
    static Vec2 ProjectOnNormal     (Vec2 const& vector, Vec2 const& onNormal);
    static Vec2 Reflect             (Vec2 const& direction, Vec2 const& normal);
    static Vec2 ReflectInvert       (Vec2 const& direction, Vec2 const& normal);
    static Vec2 RotateDeg           (Vec2 const& vector, float const angleDeg);
    static Vec2 RotateRad           (Vec2 const& vector, float const angleRad);

    static float SignedAngleDeg      (Vec2 const& from, Vec2 const& to);
    static float SignedAngleRad      (Vec2 const& from, Vec2 const& to);

    //- Member Methods :
    Vec2 GetAbs          () const;

    float GetAbsMax         () const;
    float GetAbsMin         () const;
    float Length            () const;
    float SquaredLength     () const;
    float GetMax            () const;
    float GetMin            () const;

    Vec2 GetPerpendicular        () const;
    Vec2 GetSign                 () const;
    Vec2 GetSafeNormalized       (float const tolerance = MATH_EPSILON_F) const;
    Vec2 GetUnsafeNormalized     () const;

    bool IsNearlyZero   (float const    tolerance = MATH_EPSILON_F) const;
    bool IsNormalized   () const;
    bool IsUniform      (float const tolerance = MATH_EPSILON_F) const;
    bool IsUnit         (float const lengthSquaredTolerance = MATH_EPSILON_F)  const;
    bool IsZero         () const;

    void Normalize              ();
    void Set                    (float const value);
    void Set                    (float const x, float const    y);
    void ToDirectionAndLength   (Vec2& outDirection, float& outLength)const;

    //- Operateurs
    bool        operator==  (Vec2 const& vector)   const;
    bool        operator!=  (Vec2 const& vector)   const;

    Vec2&    operator=   (Vec2 const& other);

    float       operator[]  (unsigned int const    index)    const;
    float&      operator[]  (unsigned int const    index);

    Vec2     operator+   (Vec2 const& vector)   const;
    Vec2     operator+   (float const    value)    const;
    Vec2&    operator+=  (Vec2 const& vector);
    Vec2&    operator+=  (float const    value);

    Vec2     operator-   ()                          const;
    Vec2     operator-   (Vec2 const& vector)   const;
    Vec2     operator-   (float const    value)    const;
    Vec2&    operator-=  (Vec2 const& vector);
    Vec2&    operator-=  (float const    value);

    Vec2     operator*   (Vec2 const& vector)   const;
    Vec2     operator*   (float const    scale)    const;
    Vec2&    operator*=  (Vec2 const& vector);
    Vec2&    operator*=  (float const    scale);

    Vec2     operator/   (Vec2 const& vector)   const;
    Vec2     operator/   (float const    scale)    const;
    Vec2&    operator/=  (Vec2 const& vector);
    Vec2&    operator/=  (float const    scale);

};

Vec2 operator+ (float const value, Vec2 const& vector);
Vec2 operator* (float const scale, Vec2 const& vector);


#endif