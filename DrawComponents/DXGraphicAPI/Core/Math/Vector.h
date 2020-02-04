#pragma once
namespace Math
{
	struct Vector3
	{
		float x, y, z;

		Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
			: x(x), y(y), z(z) {}

		Vector3(const Vector3& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}

		Vector3(Vector3&& v) noexcept
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}

		~Vector3() {}

		Vector3& operator = (const Vector3& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		Vector3 operator + (const Vector3& v) const
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		Vector3 operator - (const Vector3& v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		Vector3 operator *(const float a) const
		{
			return Vector3(x * a, y * a, z * a);
		}

		Vector3 operator /(const float a) const
		{
			return Vector3(x / a, y / a, z / a);
		}

		Vector3 operator -() const
		{
			return Vector3(-x, -y, -z);
		}

		Vector3& operator +=(const Vector3& v)
		{
			*this = *this + v;
			return *this;
		}

		Vector3& operator -=(const Vector3& v)
		{
			*this = *this - v;
			return *this;
		}

		Vector3& operator *=(const float a)
		{
			*this = *this * a;
			return *this;
		}

		Vector3& operator /=(const float a)
		{
			*this = *this / a;
			return *this;
		}

		bool IsEqual(const Vector3& v, float epsilon = 1.0e-5)
		{
			return ((x - v.x) < epsilon && (y - v.y) < epsilon && (z - v.z) < epsilon);
		}
	};

}
