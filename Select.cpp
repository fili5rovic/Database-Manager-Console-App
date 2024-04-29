#include "Select.h"

//const regex Select::regexPattern = regex(R"(^\s*select\s+((?:\w+|\*)(\s*,\s*(?:\w+|\*))*)\s+from\s+(\w+)+)", regex_constants::icase);
const regex Select::regexPattern = regex(R"(^\s*select\s+((?:\w+|\*)(?:\s*,\s*(?:\w+|\*))*)\s+from\s+(\w+)+\s*(?:where\s+((\w+)\s*(\=|\<\>|\!\=)\s*('\w+'|"\w+"|\w+)(?:\s+(and|or)\s*(\w+)\s*(\=|\<\>|\!\=)\s*('\w+'|"\w+"|\w+))*))?)", regex_constants::icase);
