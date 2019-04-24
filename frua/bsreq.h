#include "collection.h"

#pragma once

#define	BSREQ(fn) {#fn, (unsigned)&((type*)0)->fn,\
sizeof(bsreq::btype<decltype(type::fn)>::value),\
sizeof(type::fn),\
bsreq::icount<decltype(type::fn)>::value,\
bsmeta<bsreq::btype<decltype(type::fn)>::value>::meta,\
bsreq::iref<decltype(type::fn)>::value,\
bsreq::isubtype<decltype(type::fn)>::value}
#define BSDATA(c, i) static c c##_data_array[i];\
bsdatat<c> bsmeta<c>::data(#c, c##_data_array, bsreq::Scalar);

// Metadata field descriptor
struct bsreq {
	enum subtype_s : unsigned char { Scalar, Enum, ADat, ARef, ARem, CFlags };
	// Get count of reference
	template<class T> struct iref : static_int<0> {};
	template<class T> struct iref<T*> : static_int<1 + iref<T>::value> {};
	template<class T, int N> struct iref<T[N]> : static_int<iref<T>::value> {};
	template<class T> struct iref<T[]> : static_int<iref<T>::value> {};
	// Get type count
	template<class T> struct icount : static_int<1> {};
	template<class T, unsigned N> struct icount<T[N]> : static_int<N> {};
	template<class T> struct icount<T[]> : static_int<0> {};
	template<class T, unsigned N> struct icount<adat<T, N>> : static_int<N> {};
	// Get base type
	template<class T> struct btype { typedef T value; };
	template<class T> struct btype<T*> { typedef T value; };
	template<class T, unsigned N> struct btype<T[N]> { typedef T value; };
	template<class T> struct btype<T[]> { typedef T value; };
	template<class T> struct btype<const T> { typedef T value; };
	template<class T> struct btype<const T*> { typedef T value; };
	// Get subtype
	template<class T> struct isubtype : static_value<subtype_s, __is_enum(T) ? Enum : Scalar> {};
	template<class T> struct isubtype<T*> : static_value<subtype_s, isubtype<T>::value> {};
	template<class T, unsigned N> struct isubtype<T[N]> : static_value<subtype_s, isubtype<T>::value> {};
	template<class T, unsigned N> struct isubtype<adat<T, N>> : static_value<subtype_s, ADat> {};
	template<class T> struct isubtype<aref<T>> : static_value<subtype_s, ARef> {};
	template<class T, class DT> struct isubtype<cflags<T, DT>> : static_value<subtype_s, CFlags> {};
	//
	const char*			id; // field identifier
	unsigned			offset; // offset from begin of class or object
	unsigned			size; // size of single element
	unsigned			lenght; // total size in bytes of all field (array has size*count)
	unsigned			count; // count of elements
	const bsreq*		type; // metadata of element
	unsigned char		reference; // 1+ if reference
	bsreq::subtype_s	subtype;
	//
	operator bool() const { return id != 0; }
	//
	const bsreq*		find(const char* name) const;
	const bsreq*		find(const char* name, unsigned count) const;
	const bsreq*		find(const char* name, const bsreq* type) const;
	static const bsreq	metadata[];
	int					get(const void* p) const;
	const bsreq*		getkey() const;
	bool				issimple() const { return type == 0; }
	bool				match(const void* p, const char* name) const;
	constexpr const char* ptr(const void* data) const { return (const char*)data + offset; }
	constexpr const char* ptr(const void* data, int index) const { return (const char*)data + offset + index * size; }
	void				set(const void* p, int value) const;
	template<class T> constexpr static const bsreq* getmeta() { return T::metadata; }
	template<class T> constexpr static const bsreq* getmetareq() { return getmeta<btype<T>::value>(); }
};
struct bsdata {
	const char*			id;
	const bsreq*		meta;
	bsdata*				next;
	unsigned			count;
	unsigned			maximum;
	unsigned			size;
	void*				data;
	bsreq::subtype_s	subtype;
	static bsdata*		first;
	static bsdata*		firstenum;
	//
	bsdata(const char* id, const bsreq* meta,
		void* data, unsigned size, unsigned count, unsigned maximum,
		bsreq::subtype_s subtype);
	//
	void*				add();
	static bsdata*		find(const char* id, bsdata* first);
	static bsdata*		find(const bsreq* id, bsdata* first);
	const void*			find(const bsreq* id, const char* value) const;
	static bsdata*		findbyptr(const void* object);
	const void*			get(int index) const { return (char*)data + size * index; }
	bool				has(const void* object) const { return object >= data && object < ((char*)data + maximum * size); }
	int					indexof(const void* object) const;
};
template<typename T> struct bsdatat : bsdata {
	template<unsigned N> constexpr bsdatat(const char* id, T(&source)[N], bsreq::subtype_s subtype) : bsdata(id, bsmeta<T>::meta, source, sizeof(T), 0, N, subtype) {}
	T& operator[](int index) { return ((T*)data)[index]; }
	T*					add() { return (T*)bsdata::add(); }
	constexpr const T*	begin() const { return (T*)data; }
	constexpr T*		begin() { return (T*)data; }
	constexpr const T*	end() const { return (T*)data + count; }
};
template<typename T> struct bsmeta {
	typedef T			type;
	static const bsreq	meta[];
	static bsdatat<T>	data;
};
template<> struct bsmeta<int> {
	typedef int			type;
	static const bsreq	meta[];
};
template<> struct bsmeta<const char*> {
	typedef const char*	type;
	static const bsreq	meta[];
};
template<> struct bsmeta<unsigned char> : bsmeta<int> {};
template<> struct bsmeta<char> : bsmeta<int> {};
template<> struct bsmeta<unsigned short> : bsmeta<int> {};
template<> struct bsmeta<short> : bsmeta<int> {};
template<> struct bsmeta<unsigned> : bsmeta<int> {};