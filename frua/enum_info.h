#pragma once

struct name_info {
	const char*				id;
	const char*				name;
};
struct enum_info {
	const void*				data;
	unsigned				size;
	int						i1, i2;
	constexpr enum_info() : data(0), size(0), i1(0), i2(0) {}
	constexpr enum_info(const void* data, int i1, int i2, unsigned size) : data(data), size(size),
		i1(i1), i2(i2) {}
	constexpr bool operator==(const enum_info& e) const { return data == e.data && size == e.size; }
	const char*				get(int index) const;
	const name_info*		getni(int index) const;
	constexpr const name_info* begin() const { return (name_info*)((char*)data + size * (i1 + 1)); }
	constexpr const name_info* end() const { return (name_info*)((char*)data + size * (i2 + 1)); }
};
