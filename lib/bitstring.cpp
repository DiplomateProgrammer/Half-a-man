#include "headers/bitstring.h"
using namespace my;
using std::vector;
bitstring::bitstring()
{
	storage = vector<unsigned char>(0);
	last_bit = 7;
}
bitstring::bitstring(bool bit)
{
	storage = vector<unsigned char>(1, bit * 128);
	last_bit = 0;
}
bitstring::bitstring(const bitstring &other)
{
	storage = vector<unsigned char>(other.storage);
	last_bit = other.last_bit;
}
size_t bitstring::length() const
{
	return storage.size();
}
unsigned char bitstring::get_last_bit() const
{
	return last_bit;
}
bool bitstring::get_bit(unsigned int byte, unsigned char bit) const
{
	return (storage[byte] >> (7 - bit)) % 2;
}
bool bitstring::get_bit(unsigned int bit) const
{
	return (storage[bit / 8] >> (7 - bit % 8)) % 2;
}
unsigned int bitstring::get_num_bits() const
{
	if (length() == 1) return last_bit + 1;
	else return (length() - 1) * 8 + (last_bit + 1);
}
void bitstring::append(bool bit)
{
	if (last_bit < 7)
	{
		last_bit++;
	}
	else
	{
		storage.push_back(0);
		last_bit = 0;
	}
	storage[length() - 1] += bit << (7 - last_bit);
}
void bitstring::append(const bitstring &other)
{
	if (length() == 0)
	{
		*this = bitstring(other);
		return;
	}
	for (size_t i = 0; i < other.length() - 1; i++)
	{
		storage[length() - 1] += other.storage[i] >> (last_bit + 1);
		storage.push_back(other.storage[i] << (7 - last_bit));
	}
	storage[length() - 1] += other.storage[other.length() - 1] >> (last_bit + 1);
	if (last_bit + other.last_bit + 1 > 7)
	{
		storage.push_back(other.storage[other.length() - 1] << (7 - last_bit));
	}
	last_bit = (last_bit + other.last_bit + 1) % 8;
}
void bitstring::append(unsigned char byte)
{
	if (length() == 0) storage.push_back(byte);
	else
	{
		storage.back() += byte >> (last_bit + 1);
		storage.push_back(byte << (7 - last_bit));
	}
}
std::ostream& my::operator<<(std::ostream& os, const bitstring &bs)
{
	for (auto i : bs.storage) 
	{ 
		os << i; 
	}
	return os;
}