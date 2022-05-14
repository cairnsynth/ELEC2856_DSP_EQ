#ifndef BUFFER_H
#define BUFFER_H

#include <memory>

class Buffer {                                      //Float buffer header file
    public:                                         //Public members
        Buffer(int size);
        void shift_in(float val);
        void multiply(float &out, float* coeff);
        void add(float val);
        void reset();

    private:                                        //Private members
        int _size;
        std::unique_ptr<float[]> _buffer;
};

#endif //BUFFER_H