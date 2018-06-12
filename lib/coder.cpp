#include "headers/coder.h"
#include <vector>
#include <string>
#include <set>
using std::string;
using std::vector;
using std::multiset;
using std::pair;
using namespace my;
struct compare
{
	bool operator()(pair<unsigned int, tree*> a, pair<unsigned int, tree*> b)
	{
		return a.first < b.first;
	}
};
inline bool get_bit(const unsigned char &byte, const unsigned char &bit)
{
	return (byte >> (7 - bit)) % 2;
}
void coder::dfs_values(const tree* node, std::vector<bitstring> &res, const bitstring &cur)
{
	if (!node) return;
	if (node->is_leaf())
	{
		res[node->ch + 128] = cur;
	}
	else
	{
		bitstring left(cur), right(cur);
		left.append(false), right.append(true);
		dfs_values(node->l, res, left);
		dfs_values(node->r, res, right);
	}
}
void coder::dfs_build_tree(const bitstring &code, unsigned char pos, const char &ch, tree *node)
{
	if (pos >= code.get_num_bits()) 
	{
		node->ch = ch;
		return; 
	}
	if (code.get_bit(pos) == 0)
	{
		if (!node->l)
		{
			tree *new_node = new tree(0);
			node->l = new_node;
	    }
		dfs_build_tree(code, pos + 1, ch, node->l);
	}
	else
	{
		if (!node->r)
		{
			tree *new_node = new tree(0);
			node->r = new_node;
		}
		dfs_build_tree(code, pos + 1, ch, node->r);
	}
}
tree* coder::build_tree(const vector<bitstring> &codes)
{
	tree *root = new tree(0);
	for (int i = 0; i < 256; i++)
	{
		if (codes[i].length() > 0) { dfs_build_tree(codes[i], 0, i - 128, root); }
	}
	return root;
}
void print_bytes(unsigned int num, unsigned char num_bytes, std::ostream &out)
{
	vector<unsigned char> bytes(num_bytes, 0);
	for (unsigned char i = 0; i < num_bytes; i++)
	{
		bytes[i] = num % 256;
		num /= 256;
	}
	for (unsigned char i = num_bytes - 1; i < num_bytes; i--)
	{
		out << bytes[i];
	}
}
unsigned char read_char(std::istream &in)
{
	char ch = in.get();
	return *reinterpret_cast<unsigned char*>(&ch);
}
unsigned int read_bytes(unsigned char num_bytes, std::istream &in)
{
	unsigned int ans = 0;
	for (unsigned char i = 0; i < num_bytes; i++)
	{
		ans *= 256;
		ans += read_char(in);
	}
	return ans;
}
bool coder::encode(unsigned const int size, std::istream &in, std::ostream &out)
{
	char *buffer = new char[size];
	in.read(buffer, size);
	if (in.gcount() == 0) return false;
	vector<unsigned int> freq(256, 0);
	for(size_t i = 0; i < in.gcount(); i++)
	{
		freq[buffer[i] + 128]++;
	}
	multiset<pair<unsigned int, tree*>, compare> heap;
	unsigned short non_zero = 0;
	for (unsigned int i = 0; i < 256; i++)
	{
		if (freq[i] != 0)
		{
			tree *new_tree = new tree(i - 128);
			non_zero++;
			heap.insert({ freq[i], new_tree });
		}
	}
	while (heap.size() > 1)
	{
		pair<unsigned int, tree*> first, second;
		first = *heap.begin();
		heap.erase(heap.begin());
		second = *heap.begin();
		heap.erase(heap.begin());
		tree *new_tree = new tree(0);
		new_tree->l = first.second;
		new_tree->r = second.second;
		heap.insert({ first.first + second.first, new_tree });
	}
	tree *root = heap.begin()->second;
	if (root->is_leaf())
	{
		tree *new_root = new tree(0);
		new_root->l = root;
		root = new_root;
	}
	vector<bitstring> codes = vector<bitstring>(256);
	dfs_values(root, codes);
	print_bytes(non_zero, 2, out);
	for (size_t i = 0; i < 256; i++)
	{
		if (freq[i] != 0) { out << (unsigned char)i << (unsigned char)codes[i].get_num_bits() << codes[i]; }
	}
	bitstring coded = bitstring();
	for (size_t i = 0; i < in.gcount(); i++)
	{
		coded.append(codes[buffer[i] + 128]);
	}
	print_bytes(coded.get_num_bits(), 4, out);
	out << coded; 
	delete_tree(root);
	delete[] buffer;
	return !in.eof();
}
bool coder::decode(std::istream &in, std::ostream &out)
{
	unsigned int numchar = read_bytes(2, in);
	if (in.eof() || numchar == 0) { return false; }
	vector<bitstring> codes(256, bitstring());
	for (size_t i = 0; i < numchar; i++)
	{
		unsigned char ch = read_char(in);
		char cur_char = (int)ch - 128;
		unsigned char num_bits = read_char(in), num_bytes, last_bit;
		num_bytes = (num_bits - 1) / 8;
		last_bit = (num_bits - 1) % 8;
		bitstring cur = bitstring();
		unsigned char byte;
		for (size_t j = 0; j < num_bytes; j++)
		{
			byte = read_char(in);
			cur.append(byte);
		}
		byte = read_char(in);
		for (unsigned char j = 0; j <= last_bit; j++)
		{
			bool bit = get_bit(byte, j);
			cur.append(bit);
		}
		codes[ch] = cur;
	}
	tree *root = build_tree(codes);
	unsigned int num_bits = read_bytes(4, in);
	unsigned char byte;
	unsigned char cur_bit = 7;
	bool bit;
	tree *cur = root;
	for (size_t i = 0; i < num_bits; i++)
	{
		cur_bit++;
		if (cur_bit == 8)
		{
			cur_bit = 0;
			byte = read_char(in);
		}
		bit = get_bit(byte, cur_bit);
		if (bit == 0) { cur = cur->l; }
		else { cur = cur->r; }
		if (cur->is_leaf())
		{
			out << cur->ch;
			cur = root;
		}
	}
	delete_tree(root);
	return true;
}