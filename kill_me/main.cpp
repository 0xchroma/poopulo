
namespace chroma
{
	template <typename T>
	struct buffer
	{

	};

	template <typename T>
	struct linked_list
	{

	};

	struct string : buffer<char>
	{

	};

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
		mat_type operator+(mat_type const& lhs, T const& rhs) { return *this; }
		mat_type operator-(mat_type const& lhs, T const& rhs) { return *this; }
		mat_type operator/(mat_type const& lhs, T const& rhs) { return *this; }
		mat_type operator*(mat_type const& lhs, T const& rhs) { return *this; }
	};
}

int main(int, char**)
{
	return 0;
}