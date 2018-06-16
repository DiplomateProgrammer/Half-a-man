#ifndef MY_BITSTRING
#define MY_BITSTRING
#include <iostream>
#include <vector>
namespace my
{
	struct bitstring
	{
		bitstring();
		bitstring(const bitstring&);
		bitstring(bool bit);
		void append(bool bit);
		void append(const bitstring &other);
		void append(unsigned char byte);
		size_t length() const;
		unsigned char get_last_bit() const;
		unsigned int get_num_bits() const;
		bool get_bit(unsigned int byte, unsigned char bit) const;
		bool get_bit(unsigned int bit) const;
		void to_vector(std::vector<char>&) const;
	private:
		friend std::ostream& operator<<(std::ostream &os, const bitstring& bs);
		std::vector<unsigned char> storage;
		unsigned char last_bit;
	};
}
#endif