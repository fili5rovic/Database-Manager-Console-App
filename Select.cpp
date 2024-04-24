#include "Select.h"

const regex Select::regexPattern = regex(R"(^\s*select\s+((?:\w+|\*)(\s*,\s*(?:\w+|\*))*)\s+from\s+(\w+)+)", regex_constants::icase);
