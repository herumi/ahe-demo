#include <cybozu/socket.hpp>
#include "enc-dec.hpp"

int main()
	try
{
	initAhe();
	cybozu::Socket server;
	server.bind(g_port);
	for (;;) {
		while (!server.queryAccept()) {
		}
		puts("accept");
		cybozu::Socket client;
		server.accept(client);
		{
			uint32_t w, h;
			cybozu::load(w, client);
			cybozu::load(h, client);
			printf("recv w=%u, h=%u\n", w, h);
			puts("recv encrypted image");
			CipherTextVec encY;
			cybozu::load(encY, client);
			puts("compute encrypted edge with encrypted image");
			CipherTextVec edge;
			getEncEdge(edge, encY, w, h);
			puts("send encrypted edge");
			cybozu::save(client, edge);
			puts("end");
		}
	}
} catch (std::exception& e) {
	printf("ERR %s\n", e.what());
	return 1;
}

