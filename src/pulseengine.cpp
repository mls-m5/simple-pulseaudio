#include "pulseengine.h"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <iostream>
#include <exception>
#include <iostream>


using namespace std;


void PulseEngine::init(string name) {
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
        throw __FILE__;
    }
}

void PulseEngine::push(vector<float> &buffer) {
    pa_usec_t latency;
    auto simple = (pa_simple*)this->container;
    
    int error = 0;
    if ((latency = pa_simple_get_latency(simple, &error)) == (pa_usec_t) -1) {
        cerr << pa_strerror(error);
        throw __FILE__;
    }
    
    //Here the data should be generated
    if (pa_simple_write(simple, &buffer.front(), buffer.size(), &error) < 0) {
        cerr << "error with writing output" << endl;
        cerr << pa_strerror(error);
        throw __FILE__;
    }
    
    //Make sure that everything is played
    if (pa_simple_drain(simple, &error) < 0) {
        cerr << "error with draining" << endl;
        cerr << pa_strerror(error);
        throw __FILE__;
    }
}

PulseEngine::~PulseEngine() {
    auto simple = (pa_simple*)this->container;
    
	if (simple) {
		pa_simple_free(simple);
	}
}