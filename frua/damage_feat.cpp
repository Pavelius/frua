#include "main.h"

damage_feat_info damage_feat_data[] = {{"SaveHalf", "������� �� ��������"},
{"NoSave", "��� ��������"},
};
assert_enum(damage_feat, NoSave);
getstr_enum(damage_feat);