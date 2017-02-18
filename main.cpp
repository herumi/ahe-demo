#ifdef _MSC_VER
#pragma comment(lib, "opencv_world310.lib")
#pragma warning(disable: 4819)
#endif
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include "enc-dec.hpp"
#include <cybozu/socket.hpp>
#include <cybozu/option.hpp>

/*
	0 : B
	1 : G
	2 : R
*/
void getMono(uint8_t *dst, const uint8_t *src, size_t w, size_t h)
{
	for (size_t i = 0; i < w * h; i++) {
		uint32_t b = src[i * 3 + 0];
		uint32_t g = src[i * 3 + 1];
		uint32_t r = src[i * 3 + 2];
		uint32_t y = (r * 3 + g * 6 + b) / 10;
		dst[i] = uint8_t(y);
	}
}

void getEdge(uint8_t *dst, const uint8_t *src, size_t w, size_t h)
{
	for (size_t y = h - 2; y > 0; y--) {
		for (size_t x = w - 2; x > 0; x--) {
			int v = src[x + y * w] * (-4) + src[x + y * w - 1] + src[x + y * w + 1] + src[x + (y - 1) * w] + src[x + (y + 1) * w];
			v = (v < 0) ? 0 : (v > 255) ? 255 : v;
			dst[x + y * w] = 255 - uint8_t(v);
		}
	}
}

int main(int argc, char *argv[])
	try
{
	int port;
	std::string server;
	cybozu::Option opt;
	opt.appendOpt(&port, g_port, "p", " : port");
	opt.appendOpt(&server, "localhost", "s", " : server");
	opt.appendHelp("h", " : show this message");
	if (!opt.parse(argc, argv)) {
		opt.usage();
		return 1;
	}

	const char *winName = "window";
	const double rate = 0.25;
	cv::VideoCapture cap;
	if (!cap.open(0)) {
		puts("can't open camera");
		return 1;
	}
	const uint32_t w = uint32_t(cap.get(cv::CAP_PROP_FRAME_WIDTH) * rate);
	const uint32_t h = uint32_t(cap.get(cv::CAP_PROP_FRAME_HEIGHT) * rate);
	printf("w =%u\n", w);
	printf("h =%u\n", h);
    cv::namedWindow(winName, CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);

	initAhe();
	SecretKey sec;
	initSecretKey(sec);
	const int range = 4 * 256;
	sec.setCache(-range, range);
	const PublicKey& pub = sec.getPublicKey();

	bool doEdge = false;
	cv::Mat org, image;
	cv::Mat mono = cv::Mat::zeros(h, w, CV_8UC1);
	cv::Mat e = cv::Mat::zeros(h, w, CV_8UC1);
	bool stop = false;

	for (;;) {
		if (!stop) {
			cap >> org;
			cv::resize(org, image, cv::Size(), rate, rate);
			getMono(mono.data, image.data, w, h);
		}

		if (doEdge) {
			getEdge(e.data, mono.data, w, h);
			memcpy(mono.data, e.data, w * h);
		}
	    cv::imshow(winName, mono);
	    int key = cv::waitKey(1);
	    switch (key) {
		case 'q':
			return 0;
		case 't':
			doEdge = !doEdge;
			break;
		case 'c':
			stop = false;
			break;
		case 'e':
			puts("enc");
#if 1
			{
				CipherTextVec encY;
				encVec(encY, pub, mono.data, w * h);
				puts("edge");
				cybozu::Socket client;
				client.connect(server, port);

				puts("send size");
				cybozu::save(client, w);
				cybozu::save(client, h);
				puts("send encrypted image");
				cybozu::save(client, encY);
				puts("recv encrypted edge");
				CipherTextVec edge;
				cybozu::load(edge, client);
				puts("dec");
				decVec(mono.data, w * h, edge, sec);
				puts("end");
			}
#else
			getEdge(e.data, mono.data, w, h);
			memcpy(mono.data, e.data, w * h);
#endif
			stop = true;
			break;
		}
	}
} catch (std::exception& e) {
	printf("ERR %s\n", e.what());
	return 1;
}
