#include "collection.h"

namespace {
struct A {
	int			value;
	char		name[32];
	constexpr A(): value(0), name() {}
};
struct B {
	typedef void (B::*proc)();
	int			value;
	const char*	name;
	int			size;
	constexpr B() : value(0), name("Test"), size(4) {}
	void apply() {
		size = 10;
	}
};
struct C : B {
	int			match;
	constexpr C() : match(5) {}
	void test1() {
		size = 8;
	}
	void apply() {
		size = 12;
	}
};
}

bool test_array() {
	const int		v1 = 1000;
	agrw<A>			t1;
	for(auto i = 0; i < v1; i++) {
		auto p = t1.add();
		*p = A();
	}
	auto c1 = t1.getcount();
	if(c1 != v1)
		return false;
	B b;
	C c;
	B::proc p = (B::proc)&C::apply;
	(c.*p)();
	return true;
}