#include "collection.h"

#pragma once

#define	BSREQ(c, f) {#f, (unsigned)&((c*)0)->f,\
sizeof(bsreq::btype<decltype(c::f)>::value),\
sizeof(c::f),\
bsreq::icount<decltype(c::f)>::value,\
bsreq::getmetareq<decltype(c::f)>(),\
bsreq::iref<decltype(c::f)>::value}

// Metadata field descriptor
struct bsreq {
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
	template<class T> struct btype<T*> : btype<T> {};
	template<class T, unsigned N> struct btype<T[N]> : btype<T> {};
	template<class T> struct btype<T[]> : btype<T> {};
	template<class T> struct btype<const T> : btype<T> {};
	//
	const char*			id; // field identifier
	unsigned			offset; // offset from begin of class or object
	unsigned			size; // size of single element
	unsigned			lenght; // total size in bytes of all field (array has size*count)
	unsigned			count; // count of elements
	const bsreq*		type; // metadata of element
	unsigned char		reference; // 1+ if reference
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
extern bsreq			number_type[]; // standart integer value
extern bsreq			text_type[]; // stantart zero ending string
template<> constexpr const bsreq* bsreq::getmeta<char>() { return number_type; }
template<> constexpr const bsreq* bsreq::getmeta<unsigned char>() { return number_type; }
template<> constexpr const bsreq* bsreq::getmeta<short>() { return number_type; }
template<> constexpr const bsreq* bsreq::getmeta<unsigned short>() { return number_type; }
template<> constexpr const bsreq* bsreq::getmeta<int>() { return number_type; }
template<> constexpr const bsreq* bsreq::getmeta<unsigned>() { return number_type; }
template<> constexpr const bsreq* bsreq::getmeta<const char*>() { return text_type; }