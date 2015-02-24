#ifndef __C_STRING__
#define __C_STRING__
#include <cstring>
#include <iostream>
typedef uint32_t unsigned int;
typedef uint16_t unsigned short;
typedef uint8_t unsigned char;
class String{

public:
	String();
	~String();
	String(char *str);
	String(String &str);
	String & operator = (String &str);
	String & operator = (char *str);
	String & operator += (char *str);
private: 
	uint32_t m_len;
	uint8_t *m_str;


};

#endif
