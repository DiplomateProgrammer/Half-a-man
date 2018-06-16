#include "headers/coder.h"
#include <vector>
#include <string>
#include <set>
using std::string;
using std::vector;
using std::multiset;
using std::pair;
using namespace my;
//#pragma comment(linker, "/STACK:1209715200")
namespace
{ 
	inline bool get_bit(const unsigned char &byte, const unsigned char &bit)
	{
		return (byte >> (7 - bit)) % 2;
	}
	void dfs_values(const tree* node, std::vector<bitstring> &res, const bitstring &cur = bitstring())
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
	void dfs_build_tree(const bitstring &code, unsigned char pos, const char &ch, tree *node)
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
	tree* build_tree(const vector<bitstring> &codes)
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
}
struct compare
{
	bool operator()(pair<unsigned int, tree*> a, pair<unsigned int, tree*> b)
	{
		return a.first < b.first;
	}
};

std::pair<tree*, int> coder::read_tree(std::istream &in)
{
	unsigned int numchar = read_bytes(2, in);
	vector<bitstring> codes(256, bitstring());
	if(numchar = 0) { return{ new tree(0), 1 }; }
	if (in.eof() || in.fail() || numchar > 256) { return{ new tree(0), 0 }; }
	for (size_t i = 0; i < numchar; i++)
	{
		unsigned char ch = read_char(in);
		unsigned char num_bits = read_char(in), num_bytes, last_bit;
		num_bytes = (num_bits - 1) / 8;
		last_bit = (num_bits - 1) % 8;
		bitstring cur = bitstring();
		unsigned char byte;
		for (size_t j = 0; j < num_bytes; j++)
		{
			byte = read_char(in);
			if (in.fail()) { return {new tree(0), 0 }; }
			cur.append(byte);
		}
		byte = read_char(in);
		if (in.fail()) { return {new tree(0), 0 }; }
		for (unsigned char j = 0; j <= last_bit; j++)
		{
			bool bit = get_bit(byte, j);
			cur.append(bit);
		}
		codes[ch] = cur;
	}
	return { build_tree(codes), 1};
}
void coder::write_tree(std::istream &in, std::ostream &out, std::vector<bitstring> &codes)
{
	vector<char> buffer(CHUNK_SIZE);
	in.read(buffer.data(), CHUNK_SIZE);
	std::streamsize pos = 0;
	vector<unsigned int> freq(256, 0);
	char ch;
	while (true) 
	{ 
		if (pos == in.gcount())
		{
			in.read(buffer.data(), CHUNK_SIZE);
			pos = 0;
		}
		if (in.gcount() == 0) break;
		ch = buffer[pos];
		pos++;
		freq[ch + 128]++;
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
	if (non_zero == 0)
	{
		print_bytes(0, 2, out);
		codes = vector<bitstring>(256, bitstring());
		return;
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
	dfs_values(root, codes);
	print_bytes(non_zero, 2, out);
	for (size_t i = 0; i < 256; i++)
	{
		if (freq[i] != 0) { out << (unsigned char)i << (unsigned char)codes[i].get_num_bits() << codes[i]; }
	}
}
void coder::test_read(unsigned const int size, std::istream &in)
{
	/*char ch;
	while (true)
	{
	ch = read_char(in);
	if (in.fail()) break;
	}*/
	char *buffer = new char[size];
	in.read(buffer, size);
}
bool coder::encode(std::istream &in, std::ostream &out, const std::vector<bitstring> &codes)
{
	vector<char> buffer(CHUNK_SIZE);
	in.read(buffer.data(), CHUNK_SIZE);
	if (in.gcount() == 0)
	{
		return false;
	}
	bitstring coded = bitstring();
	for (std::streamsize i = 0; i < in.gcount(); i++)
	{
		coded.append(codes[buffer[i] + 128]);
	}
	print_bytes(coded.get_num_bits(), 4, out);
	vector<char> buf = vector<char>(coded.length());
	coded.to_vector(buf);
	out.write(buf.data(), buf.size());
	//out << coded;
	return !in.eof();
}
int coder::decode(std::istream &in, std::ostream &out, tree *root)
{
	unsigned int num_bits = read_bytes(4, in);
	if (root->is_leaf() && !in.fail()) return -1;
	if (in.fail()) return 0;
	unsigned char byte;
	unsigned char cur_bit = 7;
	bool bit;
	tree *cur = root;
	vector<char> buffer(0);
	for (size_t i = 0; i < num_bits; i++)
	{
		cur_bit++;
		if (cur_bit == 8)
		{
			cur_bit = 0;
			byte = read_char(in);
			if (in.fail()) return -1;
		}
		bit = get_bit(byte, cur_bit);
		if (bit == 0) { cur = cur->l; }
		else { cur = cur->r; }
		if (!cur) { return -1; }
		if (cur->is_leaf())
		{
			buffer.push_back(cur->ch);
			cur = root;
		}
	}
	out.write(buffer.data(), buffer.size());
	return 1;
}