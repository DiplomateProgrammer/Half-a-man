#include "../lib/headers/coder.h"
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;
using namespace my;
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
					vector<bitstring> codes(256, bitstring());
					/*encoder.test_read(CHUNK_SIZE, in);
					in.close();
					in.open(in_name, ios::binary);*/
					coder::write_tree(in, out, codes);
					in.close();
					in.open(in_name, ios::binary);
					bool not_end = true;
					while (not_end) { not_end = coder::encode(in, out, codes); }
				}
				else
				{
					tree *dic = coder::read_tree(in);
					bool not_end = true;
					if (dic->is_leaf()) cout << "Incorrect input";
					while (not_end && dic->correct()) 
					{ not_end = coder::decode(in, out, dic); }
					delete_tree(dic);
				}
				t = clock() - t;
				cout << "Execution time: " << (float)t / CLOCKS_PER_SEC;

			}
			catch (const std::runtime_error &e)
			{
				cout << "Error occured:\n" << e.what();
			}
		}
	}
}
