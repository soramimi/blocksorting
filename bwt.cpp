#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <climits>
#include <cassert>

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>

namespace {
struct Sequence {
	const std::vector<uint8_t> &_in;
	int _offset;

	Sequence() = delete;
	Sequence(const std::vector<uint8_t> &in, int offset)
		: _in(in), _offset(offset)
	{
	}

	uint8_t operator[](uint k) const
	{
		const size_t idx = (_offset + k) % _in.size(); // size() O(1)
		return _in[idx];
	}

	friend bool operator < (const Sequence &a, const Sequence &b)
	{
		assert(a._in.size() == b._in.size() && "error on the size");

		for (size_t i = 0; i < a._in.size(); i++) {
			if (a[i] == b[i]) continue;
			return a[i] < b[i];
		}

		assert(false && "ill-formed call, undefined behavior");
		return false;
	}
};
}

namespace bwt {

struct Data {
	std::vector<uint8_t> array;
	int index = 0;
};

//
// Encode
//
void encode(const std::vector<uint8_t> &dat, Data *out)
{
	std::vector<int> indices (dat.size());
	std::iota(indices.begin(), indices.end(), 0);

	std::sort(indices.begin(), indices.end(), [&] (int lhs, int rhs) {
		Sequence a { dat, lhs };
		Sequence b { dat, rhs };
		return a < b;
	});

	const uint idx = std::distance(indices.begin(), std::find(indices.begin(), indices.end(), 0)); // O(n)

	out->array.resize(dat.size());
	const uint last = dat.size() - 1;
	for(uint i = 0; i < dat.size(); i++) {
		Sequence curr { dat, indices[i] };
		out->array[i] = curr[last];
	}

	out->index = idx;
}

//
// Decode
//
void decode(Data const &in, Data *out)
{
	std::vector<int> indices ( in.array.size() );
	std::iota(indices.begin(), indices.end(), 0);

	std::stable_sort(indices.begin(), indices.end(), [&](int a, int b){ return in.array[a] < in.array[b]; });

	const auto lp = [&] (int i) { return in.array[indices[i]]; };
	int p = in.index;

	out->array.resize(in.array.size());
	for(uint i = 0; i < in.array.size(); i++) {
		out->array[i] = lp(p);
		p = indices[p];
	}
}
}

int main()
{
	std::vector<uint8_t> input;

	char const *p = "abracadabra";
	int n = strlen(p);

	input.resize(n);
	memcpy(input.data(), p, n);

	auto Print = [](bwt::Data const &t){
		for (size_t i = 0; i < t.array.size(); i++) {
			putchar(t.array[i]);
		}
		putchar('\n');
	};

	bwt::Data out1;
	bwt::Data out2;
	bwt::encode(input, &out1);
	bwt::decode(out1, &out2);
	Print(out1);
	Print(out2);

	return 0;
}
