#include <stdio.h>
#include <stdlib.h>

#define STR(x) #x #x #x
#define TEXT(x) "class" #x "Info"

/*
#define CLASS_NAME(name) class#name
#define OBJECT_NAME(name) object_#name
#define MERGE(x,y)  x##y##x
*/


int main(int argc, char *argv[])
{
	char *m_str;
	char *object_1;
	char *object_2;

	m_str = STR(abc);
	printf("\nStr:: %s\n", m_str );

	m_str = TEXT(abc);
	printf("\nStr:: %s\n", m_str );

	/*
	//m_str = CLASS_NAME(primary_school);
	m_str = "CLASS_NAME(primary_school)";
	printf("\nStr:: %s\n", m_str );

	//m_str = MERGE(you, me);
	m_str = "MERGE(you, me)";
	printf("\nStr:: %s\n", m_str );
	*/
}
