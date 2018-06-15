#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
using namespace std;
constexpr long long MEGABYTE = 1024 * 1024;
void generate_file(long long num_bytes, string file_name)
{
	ofstream out;
	out.open(file_name);
	for (size_t i = 0; i < num_bytes; i++)
	{
		char ch = (rand() % 256) - 128;
		out << ch;
	}
}
void generate_txt_file(long long num_bytes, string file_name)
{
	ofstream out;
	out.open(file_name);
	for (size_t i = 0; i < num_bytes; i++)
	{
		char ch = (rand() % 26) + 'a';
		out << ch;
	}
}
int main()
{
	srand(time(NULL));
	generate_txt_file(100 * MEGABYTE, "test_txt_file.txt");
}