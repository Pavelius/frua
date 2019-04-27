#pragma once

template<class T> struct plugin {
	const void*			key;
	const T*			value;
	plugin*				next;
	static plugin*		first;
	plugin(const void* key, const T* value) : key(key), value(value) {
		auto pf = &first;
		while(*pf)
			pf = &((*pf)->next);
		*pf = this;
	}
	static const T* get(const void* key) {
		for(auto p = first; p; p = p->next) {
			if(p->key == key)
				return p->value;
		}
		return 0;
	}
};
template<class T> plugin<T>* plugin<T>::first;