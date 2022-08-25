#pragma once
#include <iostream>
#define LOG(msg) \
    std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl 

#define GL_LOG(msg) std::cout << "[OPENGL] : "<< msg << std::endl; 