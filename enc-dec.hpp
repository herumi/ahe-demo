#pragma once
#include <mcl/she.hpp>

typedef mcl::she::SecretKey SecretKey;
typedef mcl::she::PublicKey PublicKey;
typedef mcl::she::PrecomputedPublicKey PrecomputedPublicKey;
typedef mcl::she::CipherTextG1 CipherText;
typedef std::vector<CipherText> CipherTextVec;

inline void initAhe()
{
	mcl::she::initG1only(mcl::ecparam::secp256k1, 2048, 1);
}

// global constant
const uint16_t g_port = 40000;

template<class PK>
void encVec(CipherTextVec& ctv, const PK& pub, const uint8_t *p, size_t n)
{
	ctv.resize(n);
	for (size_t i = 0; i < n; i++) {
		pub.enc(ctv[i], p[i]);
	}
}

void decVec(uint8_t *p, size_t  n, const CipherTextVec& ctv, const SecretKey& sec)
{
	for (size_t i = 0; i < n; i++) {
		int v = (int)sec.dec(ctv[i]);
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
			CipherText::mul(t, t, -4);
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
