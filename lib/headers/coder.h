#ifndef MY_CODER
#define MY_CODER
#include <string>
#include "tree.h"
#include "bitstring.h"
#include <memory>
constexpr unsigned int MEGABYTE = 1024 * 1024;
constexpr unsigned int CHUNK_SIZE = 1*MEGABYTE; //change this
namespace my
{
	namespace coder
	{
		bool encode(std::istream &in, std::ostream &out, const std::vector<bitstring>&); //format: num_char(2 bytes) (char num_bits bits)* num_bits(4 bytes) bitstring
		int decode(std::istream &in, std::ostream &out, tree*);
		void write_tree(std::istream &in, std::ostream &out, std::vector<bitstring>&);
		std::pair<tree*, int> read_tree(std::istream &in);
		void test_read(unsigned const int size, std::istream &in);
	}
}
#endif