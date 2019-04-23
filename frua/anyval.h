#pragma once

class anyval {
	void*				data;
	unsigned			size;
public:
	constexpr anyval() : data(0), size(0) {}
	constexpr anyval(const anyval& v) = default;
	template<class T> constexpr anyval(T& v) : data(&v), size(sizeof(T)) {}
	constexpr explicit operator bool() const { return data != 0; }
	constexpr bool operator==(const anyval& v) const { return data==v.data && size==v.size; }
	operator int() const;
	void operator=(const int v) const;
	void				clear() { data = 0; size = 0; }
	template<class T> constexpr bool is(const T& v) const { return data==&v; }
	constexpr const void* ptr() const { return data; }
};
