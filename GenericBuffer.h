#ifndef GENERICBUFFER_H
#define GENERICBUFFER_H

#include <memory>
#include <iostream>

template <class T>                                  //Templating to make GenericBuffer work with any data type
class GenericBuffer {
    public:
        GenericBuffer(int size) : _buffer(std::unique_ptr<T[]>(new T[size])), _size(size) {
            reset();                                //Init Buffer values
        }
        void shift_in(T val) {                      //Shift buffer and insert value
            for(int i = _size-1; i > 0; i--) {      //Loop through buffer values
                _buffer[i] = _buffer[i-1];          //Value = previous value
            }
            _buffer[0] = val;                       //
        }
        T multiply(T* coeff) {      //pass reference to float in main rather than local var
            T result = 0.0;
            for(int i = 0; i < _size; i++) {
                result = result + (_buffer[i] * coeff[i]);
            }
            return result;
        }
        void add(T val) {
            _buffer[0] = val;
        }
        void reset()  {
            for(int i = 0; i < _size; i++){
                _buffer[i] = T();
            }
        }
        T get(int i) {
            return _buffer[i];
        }

        void print() {
            std::cout << "buf " << _buffer[0] << " " << _buffer[1] << " " << _buffer[2] << std::endl;
        }

    private:
        int _size;
        std::unique_ptr<T[]> _buffer;
};

#endif //BUFFER_H