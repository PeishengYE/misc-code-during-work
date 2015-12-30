#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;
int main()
{
	float a = 1.0f;
	cout << (int)a << endl;
	cout << &a << endl;
	cout << (int&)a << endl;
	cout << boolalpha << ((int)a == (int&) a) << endl;
	/* Question 1:  what's the output? */
	float b = 0.0f;
	cout <<  (int)b << endl;
	cout << &b << endl;
	cout << (int&) b << endl;
	cout << boolalpha << ((int)b == (int&)b) << endl;
	/* Question 2:  what's the output? */
	return 0;
}
