#include "collection.h"

#pragma once

#define	BSHIN(fn,hint) {#fn, (unsigned)&((data_type*)0)->fn,\
sizeof(bsreq::btype<decltype(data_type::fn)>::value),\
sizeof(data_type::fn),\
bsreq::icount<decltype(data_type::fn)>::value,\
bsmeta<bsreq::btype<decltype(data_type::fn)>::value>::meta,\
bsreq::iref<decltype(data_type::fn)>::value,\
bsreq::isubtype<decltype(data_type::fn)>::value,\
hint}
#define	BSREQ(fn) BSHIN(fn,0)
#define BSENUMB(e) e##_info bsmeta<e##_info>::elements[]
#define BSDATA(c, i) c bsmeta<c>::elements[i];\
bsdatat<c> bsmeta<c>::data(#c, bsmeta<c>::elements, KindScalar);
#define DECLENUM(e) template<> struct bsmeta<e##_s> : bsmeta<e##_info> {}
#define assert_enum(e, last) static_assert(sizeof(bsmeta<e##_info>::elements) / sizeof(bsmeta<e##_info>::elements[0]) == last + 1, "Invalid count of " #e " elements");\
const bsreq bsmeta<e##_info>::meta[] = {BSREQ(id), BSREQ(name), {}};\
bsdatat<e##_info> bsmeta<e##_info>::data(#e, bsmeta<e##_info>::elements, KindEnum);

enum bstype_s : unsigned char { KindScalar, KindEnum, KindADat, KindARef, KindARem, KindCFlags };

// Metadata field descriptor
struct bsreq {
	typedef const char* (*proc_text)(const void* object, char* result, const char* result_max);
	typedef int (*proc_num)(const void* object);
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
	template<class T> struct btype<aref<T>> : btype<T> {};
	template<class T, unsigned N> struct btype<adat<T,N>> : btype<T> {};
	template<class T, class DT> struct btype<cflags<T, DT>> : btype<T> {};
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
	const bsreq*		hint_type; // type for user defined algoritm
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
	const void*			find(const bsreq* id, const void* value, unsigned size) const;
	static bsdata*		findbyptr(const void* object, bsdata* first);
	const void*			get(int index) const { return (char*)data + size * index; }
	static const char*	getstring(const void* object, const bsreq* type, const char* id);
	bool				has(const void* object) const { return object >= data && object < ((char*)data + maximum * size); }
	int					indexof(const void* object) const;
	static int			read(const char* url);
	static int			write(const char* url);
};
template<typename T> struct bsdatat : bsdata {
	template<unsigned N> constexpr bsdatat(const char* id, T(&source)[N], bstype_s subtype) :
		bsdata(id, bsmeta<T>::meta, source, sizeof(T),
		((subtype == KindEnum) ? N : 0), N, subtype) {}
	T& operator[](int index) { return ((T*)data)[index]; }
	T*					add() { return (T*)bsdata::add(); }
	constexpr const T*	begin() const { return (T*)data; }
	constexpr T*		begin() { return (T*)data; }
	constexpr const T*	end() const { return (T*)data + count; }
};
template<typename T> struct bsmeta {
	typedef T			data_type;
	static T			elements[];
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
template<class T> const char* getstr(const T e) { return bsmeta<T>::data[e].name; }
template<> struct bsmeta<unsigned char> : bsmeta<int> {};
template<> struct bsmeta<char> : bsmeta<int> {};
template<> struct bsmeta<unsigned short> : bsmeta<int> {};
template<> struct bsmeta<short> : bsmeta<int> {};
template<> struct bsmeta<unsigned> : bsmeta<int> {};