#ifndef MY_CODER
#define MY_CODER
#include <string>
#include "tree.h"
#include "bitstring.h"
constexpr unsigned int MEGABYTE = 1024 * 1024;
constexpr unsigned int CHUNK_SIZE = 1 * MEGABYTE; //change this
namespace my
{
	struct coder
	{
		coder() = default;
		bool encode(std::istream &in, std::ostream &out, const std::vector<bitstring>&); //format: num_char(2 bytes) (char num_bits bits)* num_bits(4 bytes) bitstring
		bool decode(std::istream &in, std::ostream &out, tree*);
		void write_tree(std::istream &in, std::ostream &out, std::vector<bitstring>&);
		tree* read_tree(std::istream &in);
		void test_read(unsigned const int size, std::istream &in);
	private:
		void dfs_values(const tree*, std::vector<bitstring> &res, const bitstring &cur = bitstring());
		void dfs_build_tree(const bitstring &code, unsigned char pos, const char &ch, tree *node);
		tree* build_tree(const std::vector<bitstring> &codes);
	};
}
#endif