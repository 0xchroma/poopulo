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

	struct Sample
	{
		float l;
		float r;
	};

	enum class Note {
		C1 = 0, C_1, D1, D_1, E1, F1, F_1, G1, G_1, A1, A_1, B1,
		C2, C_2, D2, D_2, E2, F2, F_2, G2, G_2, A2, A_2, B2,
		C3, C_3, D3, D_3, E3, F3, F_3, G3, G_3, A3, A_3, B3,
		C4, C_4, D4, D_4, E4, F4, F_4, G4, G_4, A4, A_4, B4,
		C5, C_5, D5, D_5, E5, F5, F_5, G5, G_5, A5, A_5, B5,
		C6, C_6, D6, D_6, E6, F6, F_6, G6, G_6, A6, A_6, B6,
		C7, C_7, D7, D_7, E7, F7, F_7, G7, G_7, A7, A_7, B7,
		C8, C_8, D8, D_8, E8, F8, F_8, G8, G_8, A8, A_8, B8,
		C9, C_9, D9, D_9, E9, F9, F_9, G9, G_9, A9, A_9, B9,
		C10, C_10, D10, D_10, E10, F10, F_10, G10, G_10, A10, A_10, B10,
	};
}