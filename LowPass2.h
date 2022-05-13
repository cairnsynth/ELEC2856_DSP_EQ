#ifndef LOWPASS2_H
#define LOWPASS2_H

#include <algorithm>
#include <math.h>
#include "Buffer.h"
#include <iostream>

class LowPass2 {
    public:
        LowPass2(float cutoffFreq, float sampleFreq, float gain) : _bufferIn(3), _bufferOut(3) {
            _sampleFreq = sampleFreq;                                       //Initialize sample freq
            _q = 1.0f;                                                      //Q must be initialized before coefficients are calcualted
            _output = 0.0f;                                                 //Initialize output
            set_cutoff(cutoffFreq);                                         //Initialize coefficients
            set_gain(gain);                                                 //Initialize gain
        }
        float update(float input) {                                         //Update output based on input
            _bufferIn.shift_in(input);                                      //Shift input into in buffer
            _bufferOut.shift_in(0.0f);                                      //Shift 0 into out buffer
            float inMult = 0.0f;
            _bufferIn.multiply(inMult, _inCoeff);                    //Mult and sum input with in coefficients
            float outMult = 0.0f;
            _bufferOut.multiply(outMult, _outCoeff);                       //Subtract sum of outputs * out coefficients
            float result = inMult - outMult;
            _bufferOut.add(result);                                         //Add output to head of out buffer
            return result * _gain * _gainCorrection;                        //Return result
        }  
        void set_cutoff(float cutoffFreq) {                                 //Handles coeff calculation
            cutoffFreq = std::min(cutoffFreq, (0.5f * _sampleFreq));        //Cutoff = 0.5*sampleFreq if cutoff > 0.5*sampleFreq
            cutoffFreq = std::max(cutoffFreq, 0.0f);                        //Cutoff = 0 if cutoff < 0
            float theta = 6.28318530718f * (cutoffFreq / _sampleFreq);      //2*PI*fn
            float k = 1 / tan(theta/2);                                     //Equations adapted from https://www.musicdsp.org/en/latest/Filters/38-lp-and-hp-filter.html
            _inCoeff[0] = 1.0f / (1.0f + (k / _q) + (k * k));               //a0 = 1/(1 + k/Q) + k^2
            _inCoeff[1] = 2.0f * _inCoeff[0];                               //a1 = 2 * a0
            _inCoeff[2] = _inCoeff[0];                                      //a2 = a0
            _outCoeff[0] = 0.0f;                                            //b0 = 0
            _outCoeff[1] = 2.0f * (1 - (k * k)) * _inCoeff[0];              //b1 = 2 * (1 - k^2) * a0
            _outCoeff[2] = (1.0f - (k/_q) + (k*k)) * _inCoeff[0];           //b2 = (1 - k/Q + k^2) * a0
        }
        void set_gain(float gain) {
            _gain = gain;
        }
        void set_q(float q) {
            _q = q;
        }

    private:
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


#endif //LOWPASS2_H