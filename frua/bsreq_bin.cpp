#include "crt.h"
#include "bsreq.h"
#include "io.h"

struct bsdata_bin {
	io::stream&	file;
	bool		writemode;
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
	void write_string(const char* v) {
		unsigned lenght = 0;
		if(v)
			lenght = zlen(v);
		file.write(&lenght, sizeof(lenght));
		if(lenght != 0)
			file.write(v, lenght);
	}
	void serial(void* v, unsigned lenght) {
		if(writemode)
			file.write(v, lenght);
		else
			file.read(v, lenght);
	}
	bool serial(const void* object, const bsreq* records) {
		for(auto p = records; *p; p++) {
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
					if(writemode)
						write_string(*ps);
					else
						*ps = read_string();
				}
			} else if(p->isref() || p->subtype == KindEnum) {
				auto isenum = (p->subtype == KindEnum);
				if(isenum && p->isref())
					continue; // Пропускаем ссылки на перечисления
				for(unsigned i = 0; i < p->count; i++) {
					void* pv = 0;
					if(writemode) {
						if(isenum) {
							auto pb = bsdata::find(p->type, bsdata::firstenum);
							if(!pb)
								pb = bsdata::find(p->type, bsdata::first);
							if(!pb)
								continue;
							auto index = p->get(p->ptr(object, i));
							pv = (void*)pb->get(index);
						} else
							pv = (void*)p->get(p->ptr(object, i));
						if(!serial_reference(pv, 0))
							return false;
					} else {
						bsdata* pb = 0;
						if(!serial_reference(pv, &pb))
							return false;
						if(!pb)
							return false;
						if(isenum) {
							auto index = pb->indexof(pv);
							if(index == -1)
								return false; // Когда это может быть?
							p->set(p->ptr(object, i), index);
						} else
							p->set(p->ptr(object, i), (int)pv);
					}
				}
			} else if(p->subtype == KindCFlags)
				serial(p->ptr(object), p->lenght); // Флаги переносятся особым образом
			else
				serial(p->ptr(object), p->type); // Подчиненный объект, указанный прямо в теле
		}
		return true;
	}
	bool serial_reference(void*& pv, bsdata** ppb) {
		char temp[256];
		// Вначале определим базу данных
		bsdata* pb = 0;
		if(writemode) {
			pb = bsdata::findbyptr(pv, bsdata::firstenum);
			if(!pb)
				pb = bsdata::findbyptr(pv, bsdata::first);
			if(!pb) {
				file.write(&pb, sizeof(pb));
				return true;
			}
			write_string(pb->id);
		} else {
			pv = 0;
			auto key = read_string();
			if(!key) // Пустая ссылка
				return true;
			pb = bsdata::find(key, bsdata::firstenum);
			if(!pb)
				pb = bsdata::find(key, bsdata::first);
		}
		if(!pb)
			return false;
		if(ppb)
			*ppb = pb;
		// Теперь определим ключ ссылки
		// Ключем может быть только ОДНО первое поле
		// Оно может быть текстовое или скалярного типа (не ссылочный объект)
		auto pk = pb->meta;
		if(!pk->type->istext() && pk->type->isref())
			return false;
		if(pk->type->istext() && (pk->type->count > 1 || pk->reference > 1))
			return false;
		if(pk->lenght > sizeof(temp))
			return false;
		// Запишем ключевое поле
		if(writemode) {
			if(pk->istext()) {
				auto v = (const char*)pk->get(pk->ptr(pv));
				write_string(v);
			} else
				file.write(pk->ptr(pv), pk->lenght);
		} else {
			if(pk->istext()) {
				*((const char**)temp) = read_string();
				pv = (void*)pb->find(pk, *((const char**)temp));
			} else {
				file.read(temp, pk->lenght);
				pv = (void*)pb->find(pk, temp, pk->lenght);
			}
			// Если ключ не найден попытаемся его создать
			if(!pv && pb->count < pb->maximum) {
				pv = pb->add();
				memcpy(pk->ptr(pv), temp, pk->lenght);
			}
		}
		return pv != 0;
	}
	bool read_object() {
		bsdata* pb = 0;
		void* pv = 0;
		if(!serial_reference(pv, &pb))
			return false;
		if(!pb)
			return false;
		return serial(pv, pb->meta + 1);
	}
	bool write_object(bsdata* pb, const void* pv) {
		auto cpv = (void*)pv;
		if(!serial_reference(cpv, 0))
			return false;
		return serial(pv, pb->meta + 1);
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