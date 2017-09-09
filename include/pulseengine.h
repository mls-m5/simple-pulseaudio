#pragma once
#include <vector>
#include <string>


class PulseEngine {
public:
    ~PulseEngine();
    void init(std::string name);
    void push(std::vector<float> &buffer);
    
    void *container = nullptr;
};