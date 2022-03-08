#include "maths/Quaternion.hpp"


Quaternion const Quaternion::Identity(0, 0, 0, 1);

Quaternion::Quaternion()
	:m_i{0}, m_j{0}, m_k{0}, m_r{1}
{
}

Quaternion::Quaternion(float const p_value)
	:m_i{p_value}, m_j{p_value}, m_k{p_value}, m_r{p_value}
{
}

Quaternion::Quaternion(float const p_i,  float const p_j,  float const p_k,  float const p_r)
	:m_i{p_i}, m_j{p_j}, m_k{p_k}, m_r{p_r}
{
}

Quaternion::Quaternion(Vec4 const& p_vec)
	:m_i{p_vec.m_x}, m_j{p_vec.m_y}, m_k{p_vec.m_z}, m_r{p_vec.m_w}
{
}

Quaternion::Quaternion(Vec3 const& p_axis,  float p_angle)
{
	float axisLength = p_axis.GetMagnitude();
	if (axisLength != 0)
	{
		p_angle *= 0.5f;
		p_axis.GetSafeNormalized();

		float cos = Maths::Cos(p_angle);
		float sin = Maths::Sin(p_angle);

		SetAxis(p_axis * sin);
		m_r = cos;

		GetSafeNormalized();
	}
}

Quaternion::Quaternion(Quaternion const& p_copy)
	:m_i{p_copy.m_i}, m_j{p_copy.m_j}, m_k{p_copy.m_k}, m_r{p_copy.m_r}
{
}

Quaternion Quaternion::ApplyTRS(Mat4x4 p_mat)
{
	return
	{
		p_mat * Vec4{m_i, m_j, m_k, m_r}
	};
}

void Quaternion::GetAxisAndAngle(Vec3* p_outAxis, float* p_outAngle)
{
	if (Maths::Abs(m_r) > 1)
		GetSafeNormalized();
	
	*p_outAngle = 2 * Maths::Acos(m_r);
	float ratio = Maths::Sqrt(1 - m_r * m_r);

	if (ratio > MATH_EPSILON_F)
	{
		p_outAxis->m_x = m_i / ratio;
		p_outAxis->m_y = m_j / ratio;
		p_outAxis->m_z = m_k / ratio;
	}
	else
	{
		p_outAxis->m_x = 1;
	}
}

float Quaternion::GetMagnitude()
{
	return{m_i*m_i + m_j* m_j + m_k*m_k + m_r*m_r};
}

float Quaternion::GetSquaredMagnitude()
{
	return Maths::Sqrt(GetMagnitude());
}

Mat4x4 Quaternion::GetRotationMatrix()
{
	float i2 = m_i * m_i;
	float j2 = m_j * m_j;
	float k2 = m_k * m_k;

	float ik = m_i * m_k;
	float ij = m_i * m_j;
	float jk = m_j * m_k;
	float ir = m_i * m_r;
	float jr = m_j * m_r;
	float kr = m_k * m_r;

	return
	{
		1-2*(j2+k2), 2 * (ij+kr), 2 * (ik-jr), 0,
		2 * (ij-kr), 1-2*(i2+k2), 2 * (jk+ir), 0,
		2 * (ik+jr), 2 * (jk-ir), 1-2*(i2+j2), 0,
		0		   , 0			, 0			 , 1
	};
}

Quaternion Quaternion::GetSafeNormalized()
{
	float length = GetMagnitude();
	if (length == 0)
		length = 1;
	float iLength = 1 / length;

	m_i /= iLength;
	m_j /= iLength;
	m_k /= iLength;
	m_r /= iLength;

	return *this;
}

Quaternion Quaternion::GetUnsafeNormalized()
{
	float length = GetMagnitude();

	m_i /= length;
	m_j /= length;
	m_k /= length;
	m_r /= length;

	return *this;
}

void Quaternion::SetAxis(Vec3 const& p_vec)
{
	m_i = p_vec.m_x;
	m_j = p_vec.m_y;
	m_k = p_vec.m_z;
}


Quaternion Quaternion::GetQuatFromEuler(Vec3 const& p_euler)
{
	return GetQuatFromEuler(p_euler.m_x, p_euler.m_y, p_euler.m_z);
}
Quaternion Quaternion::GetQuatFromEuler(float const& p_pitch, float const& p_yaw, float const& p_roll)
{
	float cosi = Maths::Cos(p_pitch*0.5f);
	float sini = Maths::Sin(p_pitch*0.5f);
	float cosj = Maths::Cos(p_yaw*0.5f);
	float sinj = Maths::Sin(p_yaw*0.5f);
	float cosk = Maths::Cos(p_roll*0.5f);
	float sink = Maths::Sin(p_roll*0.5f);

	return
	{
		sini*cosj*cosk - cosi*sinj*sink,
		cosi*sinj*cosk + sini*cosj*sink,
		cosi*cosj*sink - sini*sinj*cosk,
		cosi*cosj*cosk + sini*sinj*sink
	};
}

Quaternion Quaternion::GetQuatFromRotMat(Mat4x4 p_mat)
{
	if (p_mat.m_mat[0] > p_mat.m_mat[5] && p_mat.m_mat[0] > p_mat.m_mat[10])
	{
		float scale = Maths::Sqrt(1 + p_mat.m_mat[0] - p_mat.m_mat[5] - p_mat.m_mat[10]) * 2;
		return
		{
			0.25f * scale,
			(p_mat.m_mat[4] + p_mat.m_mat[1]) / scale,
			(p_mat.m_mat[2] + p_mat.m_mat[8]) / scale,
			(p_mat.m_mat[9] - p_mat.m_mat[6]) / scale
		};
	}
	else if (p_mat.m_mat[5] > p_mat.m_mat[10])
	{
		float scale = Maths::Sqrt(1 + p_mat.m_mat[5] - p_mat.m_mat[0] - p_mat.m_mat[10]) * 2;
		return
		{
			(p_mat.m_mat[4] + p_mat.m_mat[1]) / scale,
			0.25f * scale,
			(p_mat.m_mat[9] + p_mat.m_mat[6]) / scale,
			(p_mat.m_mat[2] - p_mat.m_mat[8]) / scale
		};
	}
	else
	{
		float scale = Maths::Sqrt(1 + p_mat.m_mat[10] - p_mat.m_mat[5] - p_mat.m_mat[0]) * 2;
		return
		{
			(p_mat.m_mat[2] + p_mat.m_mat[8]) / scale,
			(p_mat.m_mat[9] + p_mat.m_mat[6]) / scale,
			0.25f * scale,
			(p_mat.m_mat[4] - p_mat.m_mat[1]) / scale
		};
	}
}

Quaternion Quaternion::Lerp(Quaternion const& p_q1, Quaternion const& p_q2, float p_alpha)
{
	return
	{
		p_q1.m_i + p_alpha * (p_q2.m_i - p_q1.m_i),
		p_q1.m_j + p_alpha * (p_q2.m_j - p_q1.m_j),
		p_q1.m_k + p_alpha * (p_q2.m_k - p_q1.m_k),
		p_q1.m_r + p_alpha * (p_q2.m_r - p_q1.m_r)
	};
}

Quaternion Quaternion::Nlerp(Quaternion const& p_q1, Quaternion const& p_q2, float p_alpha)
{
	return Lerp(p_q1, p_q2, p_alpha).GetSafeNormalized();
}

Quaternion Quaternion::Slerp(Quaternion const& p_q1, Quaternion p_q2, float p_alpha)
{
	float cosHalfTheta = p_q1.m_i*p_q2.m_i + p_q1.m_j*p_q2.m_j + p_q1.m_k*p_q2.m_k + p_q1.m_r*p_q2.m_r;

	if (cosHalfTheta < 0)
	{
		p_q2 = -p_q2;
		cosHalfTheta = -cosHalfTheta;
	}

	if (cosHalfTheta >= 1)
		return p_q1;

	if (cosHalfTheta > 0.95f)
		return Nlerp(p_q1, p_q2, p_alpha);
	
	float halfTheta = Maths::Acos(cosHalfTheta);
	float sinHalfTheta = Maths::Sqrt(1-cosHalfTheta*cosHalfTheta);

	if (Maths::Abs(sinHalfTheta) < 0.001f)
		return
		{
			p_q1.m_i * 0.5f + p_q2.m_i * 0.5f,
			p_q1.m_j * 0.5f + p_q2.m_j * 0.5f,
			p_q1.m_k * 0.5f + p_q2.m_k * 0.5f,
			p_q1.m_r * 0.5f + p_q2.m_r * 0.5f
		};

	float ratioA = Maths::Sin((1 - p_alpha) * halfTheta) / sinHalfTheta;
	float ratioB = Maths::Sin(p_alpha * halfTheta) / sinHalfTheta;

	return
	{
		p_q1.m_i * ratioA + p_q2.m_i * ratioB,
		p_q1.m_j * ratioA + p_q2.m_j * ratioB,
		p_q1.m_k * ratioA + p_q2.m_k * ratioB,
		p_q1.m_r * ratioA + p_q2.m_r * ratioB
	};
}

Quaternion Quaternion::Vec3FromTo(Vec3 const& p_from, Vec3 const& p_to)
{
	float cos2Theta = Vec3::Dot(p_from, p_to);
	Vec3 cross = Vec3::Cross(p_from, p_to);

	Quaternion result(cross, 1 + cos2Theta);

	return result.GetSafeNormalized();
}

Vec3 Quaternion::Vec3RotateByQuat(Vec3 const& p_vec, Quaternion const& p_quat)
{
	return
	{
		p_vec.m_x * (p_quat.m_i * p_quat.m_i + p_quat.m_r * p_quat.m_r - p_quat.m_j * p_quat.m_j - p_quat.m_k * p_quat.m_k) + p_vec.m_y * (2 * p_quat.m_i * p_quat.m_j - 2 * p_quat.m_r * p_quat.m_k) + p_vec.m_z * (2 * p_quat.m_i * p_quat.m_k + 2 * p_quat.m_r * p_quat.m_j),
		p_vec.m_x * (2 * p_quat.m_r * p_quat.m_k + 2 * p_quat.m_i * p_quat.m_j) + p_vec.m_y * (p_quat.m_r * p_quat.m_r - p_quat.m_i * p_quat.m_i + p_quat.m_j * p_quat.m_j - p_quat.m_k * p_quat.m_k) + p_vec.m_z * (-2 * p_quat.m_r * p_quat.m_i + 2 * p_quat.m_j * p_quat.m_k),
		p_vec.m_x * (-2 * p_quat.m_r * p_quat.m_j + 2 * p_quat.m_i * p_quat.m_k) + p_vec.m_y * (2 * p_quat.m_r * p_quat.m_i + 2 * p_quat.m_j * p_quat.m_k) + p_vec.m_z * (p_quat.m_r * p_quat.m_r - p_quat.m_i * p_quat.m_i - p_quat.m_j * p_quat.m_j + p_quat.m_k * p_quat.m_k)
	};
}

bool        Quaternion::operator==  (Quaternion const& p_vector)   const
{
	return m_i == p_vector.m_i && m_j == p_vector.m_j && m_k == p_vector.m_k && m_r == p_vector.m_r;
}

Quaternion  Quaternion::operator+(Quaternion const& p_quat) const
{
	return
	{
		m_i + p_quat.m_i,
		m_j + p_quat.m_j,
		m_k + p_quat.m_k,
		m_r + p_quat.m_r
	};
}
Quaternion  Quaternion::operator+(float const& p_value) const
{
	return
	{
		m_i + p_value,
		m_j + p_value,
		m_k + p_value,
		m_r + p_value
	};
}
Quaternion& Quaternion::operator+=(Quaternion const& p_quat)
{
	m_i += p_quat.m_i;
	m_j += p_quat.m_j;
	m_k += p_quat.m_k;
	m_r += p_quat.m_r;

	return *this;
}
Quaternion& Quaternion::operator+=(float const& p_value)
{

	m_i += p_value;
	m_j += p_value;
	m_k += p_value;
	m_r += p_value;

	return *this;
}


Quaternion  Quaternion::operator-() const
{
	return
	{
		-m_i,
		-m_j,
		-m_k,
		m_r
	};
}
Quaternion  Quaternion::operator-(Quaternion const& p_quat) const
{
	return
	{
		m_i - p_quat.m_i,
		m_j - p_quat.m_j,
		m_k - p_quat.m_k,
		m_r - p_quat.m_r
	};
}
Quaternion  Quaternion::operator-(float const& p_value) const
{
	return
	{
		m_i - p_value,
		m_j - p_value,
		m_k - p_value,
		m_r - p_value
	};
}
Quaternion& Quaternion::operator-=(Quaternion const& p_quat)
{
	m_i -= p_quat.m_i;
	m_j -= p_quat.m_j;
	m_k -= p_quat.m_k;
	m_r -= p_quat.m_r;

	return *this;
}
Quaternion& Quaternion::operator-=(float const& p_value)
{

	m_i -= p_value;
	m_j -= p_value;
	m_k -= p_value;
	m_r -= p_value;

	return *this;
}

Quaternion  Quaternion::operator*(Quaternion const& p_quat) const
{
	return
	{
		m_i * p_quat.m_r + m_r * p_quat.m_i + m_j * p_quat.m_k - m_k * p_quat.m_j,
		m_j * p_quat.m_r + m_r * p_quat.m_j + m_k * p_quat.m_i - m_i * p_quat.m_k,
		m_k * p_quat.m_r + m_r * p_quat.m_k + m_i * p_quat.m_j - m_j * p_quat.m_i,
		m_r * p_quat.m_r - m_i * p_quat.m_i - m_j * p_quat.m_j - m_k * p_quat.m_k
	};
}
Quaternion  Quaternion::operator*(float const& p_value) const
{
	return
	{
		m_i * p_value + m_j * p_value + m_r * p_value - m_k * p_value,
		m_j * p_value + m_k * p_value + m_r * p_value - m_i * p_value,
		m_k * p_value + m_i * p_value + m_r * p_value - m_j * p_value,
		m_r * p_value - m_i * p_value - m_j * p_value - m_k * p_value
	};
}
Quaternion& Quaternion::operator*=(Quaternion const& p_quat)
{
	float i = m_i, j = m_j, k = m_k, r = m_r;

	m_i = i* p_quat.m_r + r * p_quat.m_i + j * p_quat.m_k - k * p_quat.m_j;
	m_j = j* p_quat.m_r + r * p_quat.m_j + k * p_quat.m_i - i * p_quat.m_k;
	m_k = k* p_quat.m_r + r * p_quat.m_k + i * p_quat.m_j - j * p_quat.m_i;
	m_r = r* p_quat.m_r - i * p_quat.m_i - j * p_quat.m_j - k * p_quat.m_k;

	return *this;
}
Quaternion& Quaternion::operator*=(float const& p_value)
{
	float i = m_i, j = m_j, k = m_k, r = m_r;
	m_i = i * p_value + j * p_value + r * p_value - k * p_value;
	m_j = j * p_value + k * p_value + r * p_value - i * p_value;
	m_k = k * p_value + i * p_value + r * p_value - j * p_value;
	m_r = r * p_value - i * p_value - j * p_value - k * p_value;

	return *this;
}

Quaternion  Quaternion::operator/(Quaternion const& p_quat) const
{
	return
	{
		m_i / p_quat.m_i,
		m_j / p_quat.m_j,
		m_k / p_quat.m_k,
		m_r / p_quat.m_r
	};
}
Quaternion& Quaternion::operator/=(Quaternion const& p_quat)
{
	float i = m_i, j = m_j, k = m_k, r = m_r;
	
	m_i = i / p_quat.m_i;
	m_j = j / p_quat.m_j;
	m_k = k / p_quat.m_k;
	m_r = r / p_quat.m_r;
	
	return *this;
}

Quaternion& Quaternion::operator=(Quaternion const& p_quat)
{
	m_i = p_quat.m_i;
	m_j = p_quat.m_j;
	m_k = p_quat.m_k;
	m_r = p_quat.m_r;
	return *this;
}