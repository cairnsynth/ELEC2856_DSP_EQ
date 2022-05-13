#include "Buffer.h"
#include <memory>

Buffer::Buffer(int size) : _buffer(std::unique_ptr<float[]>(new float[size])), _size(size) {
    reset();
}
//https://isocpp.org/wiki/faq/templates?fbclid=IwAR17ciBdE4g_A_qOewR__2Vr8kIjXyQWosqkMINAD8iAONO-gvSXUfHiEa0#templates-defn-vs-decl

void Buffer::shift_in(float val) {                   //Shift buffer and insert value
    for(int i = _size-1; i > 0; i--) {
        _buffer[i] = _buffer[i-1];
    }
    _buffer[0] = val;
}

void Buffer::multiply(float &out, float* coeff) {
    out = 0.0f;
    for(int i = 0; i < _size; i++) {
        out = out + (_buffer[i] * coeff[i]);
    }
}

void Buffer::add(float val) {
    _buffer[0] = val;
}

void Buffer::reset()  {
    for(int i = 0; i < _size; i++){
        _buffer[i] = 0.0f;
    }
}