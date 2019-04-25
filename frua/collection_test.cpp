#include "collection.h"

namespace {
struct A {
	int			value;
	char		name[32];
	constexpr A(): value(0), name() {}
};
struct B {
	int			value;
	const char*	name;
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
	return true;
}