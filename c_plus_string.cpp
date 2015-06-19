#include <string>
#include <iostream>
using namespace std;
int main()
{
	string empty;
	string medium = "I'm in primary school";
	string larger = "I'll start working on it, how about your progress";

	cout << "empty.size() = " << empty.size() << endl;
	cout << "medium.size() = " << medium.size() << endl;
	cout << "larger.size() = " << larger.size() << endl;

	cout << "empty.capacity() = " << empty.capacity() << endl;
	cout << "medium.capacity() = " << medium.capacity() << endl;
	cout << "larger.capacity() = " << larger.capacity() << endl;

	empty.resize(88);
	cout << "empty.capacity() = " << empty.capacity() << endl;

	empty.reserve(128);
	cout << "empty.capacity() = " << empty.capacity() << endl;
}
