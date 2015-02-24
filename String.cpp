#include "String.h"

String::String()
{
	m_len = 1;
	m_str =  new char[1];
	m_str[0] = '\0';

}

String::String(char *str)
{

	delete[] m_str;
	m_len = 1+ strlen(str);
	m_str = new char[m_len]; 
	strcpy(m_str, str);

}

String::~String()
{
	delete [] m_str;
	m_str = NULL;
	m_len = 0;

}

String::operator = (String &str)
{
	delete[] m_str;
	m_len = strlen(str) + 1;
	m_str = new char [m_len];
	strcpy(m_str, str);

}
