#include <iostream>
using namespace std;
int main(void )
{
	char * a[] = {
		"hello",
		"the",
		"world",
		NULL
	};

	char **pa = a;
	pa ++;
	cout << *pa << endl;

}
