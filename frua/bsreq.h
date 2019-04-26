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
bsdatat<c> bsmeta<c>::data(#c, c##_data_array, KindScalar);
#define DECLENUM(e) template<> struct bsmeta<e##_s> : bsmeta<e##_info> {}

enum bstype_s : unsigned char { KindScalar, KindEnum, KindADat, KindARef, KindARem, KindCFlags };

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
	template<> struct btype<const char*> { typedef const char* value; };
	template<class T> struct btype<T*> : btype<T> {};
	template<class T, unsigned N> struct btype<T[N]> : btype<T> {};
	template<class T> struct btype<T[]> : btype<T> {};
	template<class T> struct btype<const T> : btype<T> {};
	template<class T> struct btype<const T*> : btype<T> {};
	// Get subtype
	template<class T> struct isubtype : static_value<bstype_s, __is_enum(T) ? KindEnum : KindScalar> {};
	template<class T> struct isubtype<T*> : static_value<bstype_s, isubtype<T>::value> {};
	template<class T, unsigned N> struct isubtype<T[N]> : static_value<bstype_s, isubtype<T>::value> {};
	template<class T, unsigned N> struct isubtype<adat<T, N>> : static_value<bstype_s, KindADat> {};
	template<class T> struct isubtype<aref<T>> : static_value<bstype_s, KindARef> {};
	template<class T, class DT> struct isubtype<cflags<T, DT>> : static_value<bstype_s, KindCFlags> {};
	//
	const char*			id; // field identifier
	unsigned			offset; // offset from begin of class or object
	unsigned			size; // size of single element
	unsigned			lenght; // total size in bytes of all field (array has size*count)
	unsigned			count; // count of elements
	const bsreq*		type; // metadata of element
	unsigned char		reference; // 1+ if reference
	bstype_s			subtype;
	//
	operator bool() const { return id != 0; }
	//
	const bsreq*		find(const char* name) const;
	const bsreq*		find(const char* name, unsigned count) const;
	const bsreq*		find(const char* name, const bsreq* type) const;
	static const bsreq	metadata[];
	int					get(const void* p) const;
	const bsreq*		getkey() const;
	bool				is(bstype_s v) const { return subtype == v; }
	bool				isnum() const;
	bool				isref() const { return reference > 0; }
	bool				istext() const;
	bool				match(const void* p, const char* name) const;
	constexpr char*		ptr(const void* data) const { return (char*)data + offset; }
	constexpr char*		ptr(const void* data, int index) const { return (char*)data + offset + index * size; }
	void				set(const void* p, int value) const;
};
struct bsdata {
	const char*			id;
	const bsreq*		meta;
	bsdata*				next;
	unsigned			count;
	unsigned			maximum;
	unsigned			size;
	void*				data;
	bstype_s			subtype;
	static bsdata*		first;
	static bsdata*		firstenum;
	//
	bsdata(const char* id, const bsreq* meta,
		void* data, unsigned size, unsigned count, unsigned maximum,
		bstype_s subtype);
	//
	void*				add();
	static bsdata*		find(const char* id, bsdata* first);
	static bsdata*		find(const bsreq* id, bsdata* first);
	const void*			find(const bsreq* id, const char* value) const;
	static bsdata*		findbyptr(const void* object);
	const void*			get(int index) const { return (char*)data + size * index; }
	bool				has(const void* object) const { return object >= data && object < ((char*)data + maximum * size); }
	int					indexof(const void* object) const;
	static int			read(const char* url);
	static int			write(const char* url);
};
template<typename T> struct bsdatat : bsdata {
	template<unsigned N> constexpr bsdatat(const char* id, T(&source)[N], bstype_s subtype) :
		bsdata(id, bsmeta<T>::meta, source, sizeof(T), (subtype == KindEnum) ? N : 0, N, subtype) {}
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
struct bsval {
	void*				data;
	const bsreq*		type;
	constexpr bsval() : data(0), type(0) {}
	constexpr bsval(void* data, const bsreq* type) : data(data), type(type) {}
	constexpr explicit operator bool() const { return data != 0; }
	int					get() const { return type->get(type->ptr(data)); }
	void				set(int value) const { type->set(type->ptr(data), value); }
};
template<> struct bsmeta<unsigned char> : bsmeta<int> {};
template<> struct bsmeta<char> : bsmeta<int> {};
template<> struct bsmeta<unsigned short> : bsmeta<int> {};
template<> struct bsmeta<short> : bsmeta<int> {};
template<> struct bsmeta<unsigned> : bsmeta<int> {};