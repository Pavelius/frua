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
			if(p->isnum()) {
				if(p->is(KindReference)) // ���������� ������ �� �����. �� ������ ��������.
					continue;
				// ���������� ���� ������ �����
				if(writemode)
					file.write(p->ptr(object), p->lenght);
				else
					file.read(p->ptr(object), p->lenght);
			} else if(p->istext()) {
				// ����� ���������� �����������
				for(unsigned i = 0; i < p->count; i++) {
					auto ps = (const char**)p->ptr(object, i);
					if(writemode)
						write_string(*ps);
					else
						*ps = read_string();
				}
			} else if(p->is(KindReference) || p->is(KindEnum)) {
				auto isenum = p->is(KindEnum);
				if(isenum && p->is(KindReference))
					continue; // ���������� ������ �� ������������
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
								return false; // ����� ��� ����� ����?
							p->set(p->ptr(object, i), index);
						} else
							p->set(p->ptr(object, i), (int)pv);
					}
				}
			} else if(p->subtype == KindCFlags) {
				for(unsigned i = 0; i < p->count; i++)
					serial(p->ptr(object, i), p->lenght); // ����� ����������� ������ �������
			}
			else {
				for(unsigned i = 0; i < p->count; i++)
					serial(p->ptr(object, i), p->type); // ����������� ������, ��������� ����� � ����
			}
		}
		return true;
	}
	bool serial_reference(void*& pv, bsdata** ppb) {
		char temp[256];
		// ������� ��������� ���� ������
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
			if(!key) // ������ ������
				return true;
			pb = bsdata::find(key, bsdata::firstenum);
			if(!pb)
				pb = bsdata::find(key, bsdata::first);
		}
		if(!pb)
			return false;
		if(ppb)
			*ppb = pb;
		// ������ ��������� ���� ������
		// ������ ����� ���� ������ ���� ������ ����
		// ��� ����� ���� ��������� ��� ���������� ���� (�� ��������� ������)
		auto pk = pb->meta;
		if(!pk->type->istext() && pk->type->is(KindReference))
			return false;
		if(pk->type->istext() && (pk->type->count > 1))
			return false;
		if(pk->lenght > sizeof(temp))
			return false;
		// ������� �������� ����
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
			// ���� ���� �� ������ ���������� ��� �������
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
	// ������� ������� ����� ����� - ������ ������ ��� �������
	file.stream::write((int)0);
	return result;
}