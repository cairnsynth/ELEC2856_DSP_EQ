#ifndef HIGHPASS2_H
#define HIGHPASS2_H

#include <algorithm>
#include <math.h>
#include "Buffer.h"
#include <iostream>

class HighPass2 {                                                           //2nd order high pass filter
    public:
        HighPass2(float cutoffFreq, float sampleFreq, float gain) : _bufferIn(3), _bufferOut(3) {   //Initialize buffers
            _sampleFreq = sampleFreq;                                       //Initialize members
            _q = 0.7f;
            _output = 0.0f;
            set_cutoff(cutoffFreq);
            set_gain(gain);
        }
        float update(float input) {                                         //Update output based on input
            _bufferIn.shift_in(input);                                      //Shift input into in buffer
            _bufferOut.shift_in(0.0f);                                      //Shift 0 into out buffer
            float inMult = 0.0f;
            _bufferIn.multiply(inMult, _inCoeff);                           //Mult and sum input with in coefficients
            float outMult = 0.0f;
            _bufferOut.multiply(outMult, _outCoeff);                        //Subtract sum of outputs * out coefficients
            float result = inMult - outMult;
            _bufferOut.add(result);                                         //Add output to head of out buffer
            return result * _gain * _gainCorrection;  

        }
        void set_cutoff(float cutoffFreq) {
            cutoffFreq = std::min(cutoffFreq, (0.5f * _sampleFreq));        //Cutoff = 0.5*sampleFreq if cutoff > 0.5*sampleFreq
            cutoffFreq = std::max(cutoffFreq, 0.0f);  
            float theta = 6.28318530718f * (cutoffFreq / _sampleFreq);      //Coefficient calculations: https://www.musicdsp.org/en/latest/Filters/38-lp-and-hp-filter.html
            float k = tan(theta/2);
            _inCoeff[0] = 1.0f / (1.0f + (k/_q) + (k*k));
            _inCoeff[1] = -2.0f * _inCoeff[0];
            _inCoeff[2] = _inCoeff[0];
            _outCoeff[0] = 0.0f;
            _outCoeff[1] = 2.0f * ((k*k) - 1.0f) * _inCoeff[0];
            _outCoeff[2] = (1.0f - (k/_q) + (k*k)) * _inCoeff[0];
        }
        void set_gain(float gain) {                                         //Set gain
            _gain = gain;
        }
        void set_q(float q) {                                               //Set Q/resonance
            _q = q;
        }

    private:                                                                //Private members
        float _output;
        float _sampleFreq;
        float _inCoeff[3];
        float _outCoeff[3];
        float _gain;
        float _gainCorrection = 1.0f;
        float _q;
        Buffer _bufferIn;
        Buffer _bufferOut;
};

#endif //HIGHPASS2_h