/*
 * seq.hpp
 *
 *  Created on: 21.02.2011
 *      Author: vyahhi
 */

#ifndef SEQ_HPP_
#define SEQ_HPP_

#include <string>
#include <functional>
#include <memory>
#include <iostream> // for debug
#include <cstring>
#include <cassert>
#include <cstring>
#include <array>

char complement(char c); // 0123 -> 3210
char nucl(char c); // 0123 -> ACGT

template <size_t _size> // max number of nucleotides
class Seq;

// *****************************************

class Sequence { // runtime length sequence (slow!!!)
public:
	Sequence(const std::string &s);
	~Sequence();
	char operator[](const size_t index) const;
	bool operator==(const Sequence &that) const;
	Sequence& operator!() const;
	/**
	 * start -inclusive, end -exclusive;
	 */
	Sequence substr(int start, int end) const;
//	SeqVarLen operator+ (const SeqVarLen &svl1, const SeqVarLen &svl2) const;
	std::string str() const;
	int size() const;
private:
	Seq<4>* _bytes;
	int _size;
	bool _reverse;
	Sequence(const Sequence *svl, bool reverse); // reverse
};

// *****************************************

template <size_t _size> // max number of nucleotides
class Seq {
private:
	const static size_t _byteslen = (_size / 4) + (_size % 4 != 0); // compile-time constant
	//char _bytes[_byteslen]; // little-endian
	std::array<char,_byteslen> _bytes; // 0 bits overhead
public:
	Seq() {}; // random Seq, use with care!

	Seq(const char* s) {
		char byte = 0;
		int cnt = 6;
		int cur = 0;
		for (size_t pos = 0; pos < _size; ++pos, ++s) { // unsafe!
			switch (*s) {
				case 'C': case '1': byte |= (1 << cnt); break;
				case 'G': case '2': byte |= (2 << cnt); break;
				case 'T': case '3': byte |= (3 << cnt); break;
			}
			cnt -= 2;
			if (cnt < 0) {
				this->_bytes[cur++] = byte;
				cnt = 6;
				byte = 0;
			}
		}
		if (cnt != 6) {
			this->_bytes[cur++] = byte;
		}
	}

	Seq(const Seq<_size> &seq) : _bytes(seq._bytes) {
		//std::cerr << "Hey!" << std::endl;
		//memcpy(_bytes, seq._bytes, _byteslen);
	}

	char operator[] (const size_t index) const { // 0123
		int i = index;
		return ((_bytes[i >> 2] >> ((3-(i%4))*2) ) & 3);
	}

	/*Seq<_size>& operator= (const Seq<_size> &seq) {
		if (this != &seq) {
			memcpy(this->_bytes, seq._bytes, _byteslen);
		}
		return *this;
	}*/

	Seq<_size> operator!() const { // TODO: optimize
		Sequence s = Sequence(this->str());
		return Seq<_size>((!s).str());
	}

	// add nucleotide to the right
	Seq<_size> shift_right(char c) const { // char should be 0123
		assert(c <= 3);
		Seq<_size> res = *this; // copy constructor
		c <<= (((4-(_size%4))%4)*2); // omg >.<
		for (int i = _byteslen - 1; i >= 0; --i) { // don't make it size_t :)
			char rm = (res._bytes[i] >> 6) & 3;
			res._bytes[i] <<= 2;
			//res._bytes[i] &= 252;
			res._bytes[i] |= c;
			c = rm;
		}
		return res;
	}

	// add nucleotide to the left
	Seq<_size> shift_left(char c) const { // char should be 0123
		Seq<_size> res = *this; // copy constructor
		// TODO: clear last nucleotide
		for (size_t i = 0; i < _byteslen; ++i) {
			char rm = res._bytes[i] & 3;
			res._bytes[i] >>= 2;
			//res._bytes[i] &= 63;
			res._bytes[i] |= (c << 6);
			c = rm;
		}
		return res;
	}

	Sequence substring(int from, int to) const { // TODO: optimize
		std::string s = str();
		s = s.substr(from, to - from);
		return Sequence(s);
	}

	// string representation of Seq - only for debug and output purposes
	std::string str() const {
		std::string res(_size, 'A');
		//std::string res = "";
		for (size_t i = 0; i < _size; ++i) {
			res[i] = nucl(this->operator[](i));
		}
		return res;
	}

	static int size() {
		return _size;
	}

	struct hash {
		 size_t operator() (const Seq<_size> &seq) const {
			size_t h = 0;
			for (size_t i = 0; i < _byteslen; ++i) {
				h *= seq._bytes[i]*13;
			}
			return h;
		}
	};

	struct equal_to {
		bool operator() (const Seq<_size> &l, const Seq<_size> &r) const {
			return l._bytes == r._bytes;
			//return 0 == memcmp(l._bytes.data(), r._bytes.data(), _byteslen);
		}
	};

	struct less {
		int operator() (const Seq<_size> &l, const Seq<_size> &r) const {
			return l._bytes < r._bytes;
			//return 0 > memcmp(l._bytes.data(), r._bytes.data(), _byteslen);
		}
	};

	template <int _size2>
	Seq<_size2> head() { // TODO: optimize (Kolya)
		std::string s = str();
		return Seq<_size2>(s.substr(0, _size2).c_str());
	}

	template <int _size2>
	Seq<_size2> tail() const { // TODO: optimize (Kolya)
		std::string s = str();
		return Seq<_size2>(s.substr(_size - _size2, _size2).c_str());
	}

};

// *****************************************


template <int _size> // max number of nucleotides in each read
class MatePair {
public:
	MatePair(const char *s1, const char *s2, const int id_) : id(id_), seq1(s1), seq2(s2) {};
	MatePair(const MatePair &mp) : id(mp.id), seq1(mp.seq1), seq2(mp.seq2) {};
	const static MatePair<_size> null;
public: // make private!
	int id; // consecutive number from input file :)
	Seq<_size> seq1;
	Seq<_size> seq2;
};

template <int _size>
const MatePair<_size> MatePair<_size>::null = MatePair<_size>("", "", -1);

// *****************************************

#endif /* SEQ_HPP_ */
