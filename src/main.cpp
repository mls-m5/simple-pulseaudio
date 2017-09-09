#include "common.h"
#include <iostream>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <vector>
#include <random>
#include "pulseengine.h"

using namespace std;

int variant1(string name) {
	auto ret = 0;
	
	cout << "Wellcome!" << endl;
	
	vector<float> buffer(10000);
	
	default_random_engine engine;
	uniform_real_distribution<float> dist(-.1, .1);
	
	for (auto &s: buffer){
		s = dist(engine);
	}
	
	static const pa_sample_spec ss = {
		.format = PA_SAMPLE_FLOAT32,
		.rate = 44100,
		.channels = 1
	};
	
	pa_simple *simple = 0;
	int error;
	if (!(simple = pa_simple_new(nullptr, name.c_str(), PA_STREAM_PLAYBACK, nullptr, "playback", &ss, 
		nullptr, nullptr, &error))) {
		cerr << "Failed to open simple pulseaudio output" << endl;
		cerr << pa_strerror(error);
		return -1;
	}
	
	for (;;) {
		pa_usec_t latency;
		if ((latency = pa_simple_get_latency(simple, &error)) == (pa_usec_t) -1) {
			cerr << pa_strerror(error);
			ret = -1;
			break;
		}
		
		//Here the data should be generated
		if (pa_simple_write(simple, &buffer.front(), buffer.size(), &error) < 0) {
			cerr << "error with writing output" << endl;
			cerr << pa_strerror(error);
			ret = -1;
			break;
		}
		
		//Make sure that everything is played
		if (pa_simple_drain(simple, &error) < 0) {
			cerr << "error with draining" << endl;
			cerr << pa_strerror(error);
			ret = -1;
			break;
		}
	}
	if (simple) {
		pa_simple_free(simple);
	}
}

int variant2(string name) {
	vector<float> buffer(10000);
	
	default_random_engine engine;
	uniform_real_distribution<float> dist(-.1, .1);
	
	for (auto &s: buffer){
		s = dist(engine);
	}
	
	PulseEngine paengine;
	paengine.init(name);
	paengine.push(buffer);
}

int main(int argc, char **argv) {
	variant1(argv[0]);
	//variant2(argv[0]);
}
