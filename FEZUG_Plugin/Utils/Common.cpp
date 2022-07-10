#include "Common.hpp"

string Utils::ssprintf(const char* str, ...) {
	va_list ap1, ap2;
	va_start(ap1, str);
	va_copy(ap2, ap1);
	size_t sz = vsnprintf(NULL, 0, str, ap1) + 1;
	va_end(ap1);
	char* buf = (char*)malloc(sz);
	vsnprintf(buf, sz, str, ap2);
	va_end(ap2);
	string outStr(buf);
	free(buf);
	return outStr;
}

string Utils::StringReplace(string replacestr, string from, string to){
	string str = replacestr;
	if (from.empty()) return replacestr;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}