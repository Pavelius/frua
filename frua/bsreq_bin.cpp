#include "crt.h"
#include "bsreq.h"
#include "io.h"

struct bsdata_bin {
	io::stream&	file;
	bool		writemode;
	void* findobject(bsdata* pb, const bsreq* pk, const char* key) {
		if(!key)
			return 0;
		auto pv = (void*)pb->find(pk, key);
		// Если ключ не найден попытаемся его создать
		if(!pv && pb->count < pb->maximum) {
			pv = pb->add();
			pk->set(pk->ptr(pv), (int)key);
		}
		return pv;
	}
	const char* read_string() {
		unsigned lenght = 0;
		file.read(&lenght, sizeof(lenght));
		if(!lenght)
			return 0;
		char buffer[2048];
		auto p = buffer;
		if(lenght >= (sizeof(buffer) - 1))
			p = new char[lenght + 1];
		file.read(p, lenght);
		p[lenght] = 0;
		auto result = szdup(p);
		if(p != buffer)
			delete p;
		return result;
	}
	void serial(const char*& v) {
		unsigned lenght = 0;
		if(writemode) {
			if(v)
				lenght = zlen(v);
			file.write(&lenght, sizeof(lenght));
			if(lenght)
				file.write(v, lenght);
		} else
			v = read_string();
	}
	void serial(void* v, unsigned lenght) {
		if(writemode)
			file.write(v, lenght);
		else
			file.read(v, lenght);
	}
	void serial(const void* object, const bsreq* type, const bsreq* skip) {
		for(auto p = type; *p; p++) {
			if(p == skip)
				continue;
			// Ссылки на ссылки сразу пропускаем. Их нельзя сохранять.
			if(p->reference >= 2)
				continue;
			if(p->isnum()) {
				if(p->isref()) // Пропускаем ссылки на числа. Их нельзя соранять.
					continue;
				// Записываем весь массив сразу
				if(writemode)
					file.write(p->ptr(object), p->lenght);
				else
					file.read(p->ptr(object), p->lenght);
			} else if(p->istext()) {
				// Текст записываем поэлементно
				for(unsigned i = 0; i < p->count; i++) {
					auto ps = (const char**)p->ptr(object, i);
					serial(*ps);
				}
			} else if(p->isref() || p->subtype==KindEnum) {
				// Пытаемся найти тип
				auto pb = bsdata::find(p->type, bsdata::firstenum);
				if(!pb)
					pb = bsdata::find(p->type, bsdata::first);
				if(!pb)
					continue;
				// Пытаемся найти ключ
				auto pk = pb->meta->getkey();
				if(!pk)
					continue;
				// Записываем ключ
				for(unsigned i = 0; i < p->count; i++) {
					auto pv = p->ptr(object, i);
					if(writemode) {
						void* ppv;
						if(p->subtype == KindEnum) {
							auto i = p->get(pv);
							ppv = (void*)pb->get(i);
						} else
							ppv = (void*)p->get(pv);
						const char* key = 0;
						if(ppv)
							key = (const char*)pk->get(pk->ptr(ppv));
						serial(key);
					} else {
						auto key = read_string();
						auto ppv = findobject(pb, pk, key);
						if(!ppv)
							continue;
						if(p->subtype == KindEnum)
							p->set(pv, pb->indexof(ppv));
						else
							p->set(pv, (int)ppv);
					}
				}
			} else {
				switch(p->subtype) {
				case KindCFlags:
					serial(p->ptr(object), p->lenght);
					break;
				default:
					// Подчиненный объект
					serial(p->ptr(object), p->type, 0);
					break;
				}
			}
		}
	}
	bool read_object() {
		auto object_name = read_string();
		if(!object_name)
			return false;
		auto pb = bsdata::find(object_name, bsdata::firstenum);
		if(!pb)
			pb = bsdata::find(object_name, bsdata::first);
		if(!pb)
			return false;
		auto pk = pb->meta->getkey();
		if(!pk)
			return false;
		auto key = read_string();
		if(!key)
			return false;
		auto pv = findobject(pb, pk, key);
		if(!pv)
			return false;
		serial(pv, pb->meta, pk);
		return true;
	}
	bool write_object(bsdata* pb, const void* object) {
		auto pk = pb->meta->getkey();
		if(!pk)
			return false;
		auto key = (const char*)pk->get(pk->ptr(object));
		if(!key)
			return false;
		serial(pb->id);
		serial(key);
		serial(object, pb->meta, pk);
		return true;
	}
	constexpr bsdata_bin(io::stream& file, bool writemode) : file(file), writemode(writemode) {}
};

int bsdata::read(const char* url) {
	io::file file(url, StreamRead);
	if(!file)
		return 0;
	bsdata_bin e(file, false);
	auto result = 0;
	while(true) {
		if(!e.read_object())
			break;
		result++;
	}
	return result;
}

int bsdata::write(const char* url) {
	io::file file(url, StreamWrite);
	if(!file)
		return 0;
	bsdata_bin e(file, true);
	auto result = 0;
	for(auto pb = bsdata::first; pb; pb = pb->next) {
		for(unsigned i = 0; i < pb->count; i++) {
			if(!e.write_object(pb, pb->get(i)))
				return result;
			result++;
		}
	}
	// Запишем признак конца файла - нулевя строка имя объекта
	file.stream::write((int)0);
	return result;
}