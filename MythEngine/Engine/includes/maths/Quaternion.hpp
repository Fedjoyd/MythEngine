#ifndef _QUATERNION_HPP_
#define _QUATERNION_HPP_

#include "maths/Mat4x4.hpp"

class Quaternion
{
	public:
		float   m_i = 0,
				m_j = 0,
				m_k = 0,
				m_r = 1;

	// Static members
		static Quaternion const Identity;

		static Quaternion GetQuatFromEuler(Vec3 const& p_euler);
		static Quaternion GetQuatFromEuler(float const& p_pitch, float const& p_yaw, float const& p_roll);
		static Quaternion GetQuatFromRotMat(Mat4x4 p_mat);
		static Quaternion Lerp(Quaternion const& p_q1, Quaternion const& p_q2, float p_alpha);
		static Quaternion Nlerp(Quaternion const& p_q1, Quaternion const& p_q2, float p_alpha);
		static Quaternion Slerp(Quaternion const& p_q1, Quaternion p_q2, float p_alpha);
		static Quaternion Vec3FromTo(Vec3 const& p_from, Vec3 const& p_to);
		static Vec3 Vec3RotateByQuat(Vec3 const& p_vec, Quaternion const& p_quat);

	// Constructors :
		Quaternion();
		Quaternion(float const p_value);
		Quaternion(float const p_i, float const p_j, float const p_k, float const p_r);
		Quaternion(Vec4 const& p_vec);
		Quaternion(Vec3 const& p_axis, float p_angle);
		Quaternion(Quaternion const& p_copy);
		~Quaternion() = default;

	// Member Methods :
		Quaternion ApplyTRS(Mat4x4 p_mat);
		void GetAxisAndAngle(Vec3* p_outAxis, float* p_utAngle);
		float GetMagnitude();
		float GetSquaredMagnitude();
		Mat4x4 GetRotationMatrix();
		Quaternion GetSafeNormalized();
		Quaternion GetUnsafeNormalized();
		void SetAxis(Vec3 const&);

	// Operators
		bool        operator==  (Quaternion const& p_vector)   const;

		Quaternion  operator+(Quaternion const& p_quat) const;
		Quaternion  operator+(float const& p_value) const;
		Quaternion& operator+=(Quaternion const& p_quat);
		Quaternion& operator+=(float const& p_value);

		Quaternion  operator-() const;
		Quaternion  operator-(Quaternion const& p_quat) const;
		Quaternion  operator-(float const& p_value) const;
		Quaternion& operator-=(Quaternion const& p_quat);
		Quaternion& operator-=(float const& p_value);

		Quaternion  operator*(Quaternion const& p_quat) const;
		Quaternion  operator*(float const& p_value) const;
		Quaternion& operator*=(Quaternion const& p_quat);
		Quaternion& operator*=(float const& p_value);

		Quaternion  operator/(Quaternion const& p_quat) const;
		Quaternion& operator/=(Quaternion const& p_quat);

		Quaternion& operator=(Quaternion const& p_quat);
};

#endif