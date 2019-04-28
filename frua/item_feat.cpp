#include "main.h"

item_feat_info item_feat_data[] = {{"UseWhenHit", "Используется когда попали"},
{"BodyPart", "Часть тела"},
};
assert_enum(item_feat, BodyPart);
getstr_enum(item_feat);