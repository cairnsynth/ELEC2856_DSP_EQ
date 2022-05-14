#ifndef GENERICBUFFER_H
#define GENERICBUFFER_H

#include <memory>
#include <iostream>

template <class T>                                  //Templating to make GenericBuffer work with any data type
class GenericBuffer {                               //Header only class as templating used
    public:
        GenericBuffer(int size) : _buffer(std::unique_ptr<T[]>(new T[size])), _size(size) {
            reset();                                //Init Buffer values
        }
        void shift_in(T val) {                      //Shift buffer and insert value
            for(int i = _size-1; i > 0; i--) {      //Loop through buffer values
                _buffer[i] = _buffer[i-1];          //Value = previous value
            }
            _buffer[0] = val;                       
        }
        T multiply(T* coeff) {                      //Pass by reference to avoid multiple uses of same memory location
            T result = 0.0;
            for(int i = 0; i < _size; i++) {
                result = result + (_buffer[i] * coeff[i]);
            }
            return result;
        }
        void add(T val) {                           //Add value to start of buffer
            _buffer[0] = val;
        }
        void reset()  {                             //Set buffer values to 0
            for(int i = 0; i < _size; i++){
                _buffer[i] = T();
            }
        }
        T get(int i) {                              //Return specific buffer value, used for debugging
            return _buffer[i];
        }

        void print() {                              //Print buffer, used for debugging
            std::cout << "buf " << _buffer[0] << " " << _buffer[1] << " " << _buffer[2] << std::endl;
        }

    private:                                        //Private members
        int _size;
        std::unique_ptr<T[]> _buffer;
};

#endif //BUFFER_H