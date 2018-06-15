#include "../lib/headers/coder.h"
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;
using namespace my;
constexpr unsigned int MEGABYTE = 1024 * 1024;
constexpr unsigned int CHUNK_SIZE = 32 * MEGABYTE; //change this
int main(int argc, char *argv[])
{
	string mode, in_name, out_name;
	if (argc != 4) { cout << "Wrong number of arguments. Format: encode/decode in out"; }
	else
	{
		mode = argv[1], in_name = argv[2], out_name = argv[3];
		if (mode != "encode" && mode != "decode") { cout << "Wrong mode. Format: encode/decode in out"; }
		else
		{
			try
			{
				ifstream in;
				ofstream out;
				clock_t t;
				t = clock();
				in.open(in_name, ios::binary);
				out.open(out_name, ios::binary);
				if (mode == "encode")
				{
					coder encoder;
					bool not_end = true;
					while (not_end) { not_end = encoder.encode(CHUNK_SIZE, in, out); }
				}
				else
				{
					coder decoder;
					bool not_end = true;
					while (not_end) { not_end = decoder.decode(in, out); }
				}
				t = clock() - t;
				cout << "Execution time: " << (float)t / CLOCKS_PER_SEC;

			}
			catch (std::runtime_error e)
			{
				cout << "Error occured:\n" << e.what();
			}
		}
	}
}
