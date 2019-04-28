#include "main.h"

item_feat_info item_feat_data[] = {{"Consumable", "Расходывает заряды"},
{"BodyPart", "Часть тела"},
};
assert_enum(item_feat, BodyPart);
getstr_enum(item_feat);