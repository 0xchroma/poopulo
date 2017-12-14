#pragma once

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned long;
using u64 = unsigned long long;

using i8 = signed char;
using i16 = signed short;
using i32 = signed long;
using i64 = signed long long;

namespace chroma
{
	template <typename T>
	struct vec2
	{
		T x, y = 0;
	};

	template <typename T>
	struct vec3
	{
		T x, y, z = 0;
	};

	template <typename T>
	struct vec4
	{
		T x, y, z, w = 0;
	};

	template <typename T>
	struct matrix
	{
		using mat_type = matrix<T>;

	private:
		vec4<T> m[4];

	public:
	};
}