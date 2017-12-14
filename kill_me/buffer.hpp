#pragma once

namespace chroma
{
	template <typename T>
	class buffer
	{
		size_t array_size = 1;
		T* data = nullptr;

	public:
		buffer() : data(new T[1])
		{
		}

		buffer(std::initializer_list<T> list) : data(new T[1])
		{
			for (auto& i : list)
				push_back(i);
		}

		void push_back(T obj)
		{
			resize(++array_size);
			data[array_size - 1] = obj;
		}

		void clear()
		{
			delete[] data;
			data = new T[1];
			array_size = 1;
		}

		T* begin() { return &data[0]; }
		T* end() { return &data[array_size]; }
		T* data() { return data; }

		size_t size() const { return array_size; }
		size_t element_size() const { return sizeof(T); }

	protected:
		void resize(size_t new_size) // resize in chunks to minimize memory calls
		{
			auto new_data = new T[new_size];

			if (new_size > array_size)
				memcpy(new_data, data, array_size * sizeof(T));
			else
				memcpy(new_data, data, new_size * sizeof(T));

			delete[] data;
			data = new_data;
			array_size = new_size;
		}
	};
}