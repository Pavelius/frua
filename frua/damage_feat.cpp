#include "main.h"

damage_feat_info damage_feat_data[] = {{"SaveHalf", "������� �� ��������"},
{"NoSave", "��� ��������"},
{"AttackAdditional", "��������������"},
{"AttackOnCritical", "�����������"},
};
assert_enum(damage_feat, AttackOnCritical);
getstr_enum(damage_feat);