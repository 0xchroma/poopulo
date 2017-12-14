#pragma once

struct buffer;

namespace chroma
{
	struct string : buffer<char>
	{
		using buffer::buffer;

		string(const char* str)
		{
			resize(strlen(str));
			strcpy(data(), str);
		}

		char* c_str() { return data(); }
	};
}