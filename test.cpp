#include "lib/headers/coder.h"
#include <sstream>
#include <stdlib.h>
#include <time.h>
using namespace std;
using namespace my;
stringstream start, mid, finale;
coder test_coder;
unsigned int correct = 0, incorrect = 0;
void reset()
{
	start.str(""), mid.str(""), finale.str("");
}
string runtest(string input)
{
	reset();
	start = stringstream(input);                         
	test_coder.encode(start.str().length(), start, mid);
	test_coder.decode(mid, finale);
	if (finale.str().compare(input))
	{
		incorrect++;
		return "Error! Input |" + input + "| ,got |" + finale.str() + "|\n";
	}
	else
	{
		correct++;
		return "Ok\n";
	}
}
string generate_test(unsigned int size)
{
	string ans;
	ans.reserve(size);
	for (unsigned int i = 0; i < size; i++)
	{
		char ch = (rand() % 256) - 128;
		ans.push_back(ch);
	}
	return ans;
}
int main()
{
	cout << runtest("hello");
	cout << runtest(" ");
	cout << runtest("hello world");
	cout << runtest("hello world!!!!!!!");
	cout << runtest("The quick brown fox jumps over the lazy dog");
	srand(time(NULL));
	for (unsigned int i = 0; i < 100; i++)
	{
		unsigned int size = rand() % 1000000;
		cout << runtest(generate_test(size));
	}
	cout << "Correct: " << correct << "\n";
	cout << "Incorrect: " << incorrect << "\n";
}
