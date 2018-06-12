#ifndef MY_CODER
#define MY_CODER
#include <string>
#include "tree.h"
#include "bitstring.h"
namespace my
{
	struct coder
	{
		coder() = default;
		bool encode(unsigned const int size, std::istream &in, std::ostream &out); //format: num_char(2 bytes) (char num_bits bits)* num_bits(4 bytes) bitstring
		bool decode(std::istream &in, std::ostream &out); //sets in to nowskip
	private:
		void dfs_values(const tree*, std::vector<bitstring> &res, const bitstring &cur = bitstring());
		void dfs_build_tree(const bitstring &code, unsigned char pos, const char &ch, tree *node);
		tree* build_tree(const std::vector<bitstring> &codes);
	};
}
#endif