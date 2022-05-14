#include "Buffer.h"
#include <memory>
//Float buffer class
Buffer::Buffer(int size) : _buffer(std::unique_ptr<float[]>(new float[size])), _size(size) {        //Initialize buffer array with size specified
    reset();
}

void Buffer::shift_in(float val) {                  //Shift buffer and insert value
    for(int i = _size-1; i > 0; i--) {              //Loop through array backwards
        _buffer[i] = _buffer[i-1];
    }
    _buffer[0] = val;                               //Insert new value at start of buffer
}

void Buffer::multiply(float &out, float* coeff) {   //Multiply buffer by coefficient array
    out = 0.0f;
    for(int i = 0; i < _size; i++) {                
        out = out + (_buffer[i] * coeff[i]);        //Output = sum (buffer [i] * coefficient[i])
    }
}

void Buffer::add(float val) {                       //Add value to start of buffer
    _buffer[0] = val;
}

void Buffer::reset()  {                             //Reset buffer values to 0
    for(int i = 0; i < _size; i++){
        _buffer[i] = 0.0f;
    }
}