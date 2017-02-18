#define MCL_USE_AHE192
#include <mcl/ahe.hpp>
#include <cybozu/serializer.hpp>
#include <vector>

using namespace mcl::ahe192;

typedef std::vector<CipherText> CipherTextVec;

// global constant
const uint16_t g_port = 40000;

void encVec(CipherTextVec& ctv, const PublicKey& pub, const uint8_t *p, size_t n)
{
	ctv.resize(n);
	for (size_t i = 0; i < n; i++) {
		pub.enc(ctv[i], p[i], mcl::getRandomGenerator());
	}
}

void decVec(uint8_t *p, size_t  n, const CipherTextVec& ctv, const SecretKey& sec)
{
	for (size_t i = 0; i < n; i++) {
		int v = sec.dec(ctv[i]);
		v = (v < 0) ? 0 : (v > 255) ? 255 : v;
		p[i] = uint8_t(255 - v);
	}
}

void getEncEdge(CipherTextVec& edge, const CipherTextVec& encY, size_t w, size_t h)
{
	edge.resize(w * h);
	for (size_t y = h - 2; y > 0; y--) {
		for (size_t x = w - 2; x > 0; x--) {
			size_t i = x + y * w;
			CipherText& t = edge[i];
			t = encY[i];
			t.mul(-4);
			t.add(encY[i + 1]);
			t.add(encY[i - 1]);
			t.add(encY[i + w]);
			t.add(encY[i - w]);
		}
	}
}

namespace cybozu {

template<class OutputStream>
void save(OutputStream& os, const CipherText& c)
{
	os.write(&c, sizeof(c));
}

template<class InputStream>
void load(CipherText& c, InputStream& is)
{
	is.read(&c, sizeof(c));
}

} // cybozu
