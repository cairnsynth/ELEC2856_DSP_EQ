#ifndef BANDPASS_H
#define BANDPASS_H

#include <algorithm>
#include "LowPass.h"
#include "HighPass.h"

class BandPass {
    public:
        BandPass(float fcl_Hz, float fch_Hz, float fs_Hz, float gain) : _lpFilt(fch_Hz, fs_Hz, 1.0f), _hpFilt(fcl_Hz, fs_Hz, 1.0f) {
            _fs_Hz = fs_Hz;
            set_cutoff(fcl_Hz, fch_Hz);
            set_gain(gain);
            _output = 0.0f;
            _inputLast = 0.0f;

        }
        float update(float input) {
            /*
            _lowOut = _coeffLP[0] * input + _coeffLP[1] * _lowOut;  
            _output = _coeffHP * (_lowOut - _inputLast + _output);
            _inputLast = _lowOut;
            return _output * _gain;
            */
            float lowOut = _lpFilt.update(input);
            return _hpFilt.update(lowOut);
        }
        void set_cutoff(float fcl_Hz, float fch_Hz) {
            /*
            fch_Hz = std::min(fch_Hz, (0.5f * _fs_Hz));
            fch_Hz = std::max(fch_Hz, 0.0f);
            float alpha = (6.28318530718f * fch_Hz) / _fs_Hz;
            _coeffLP[0] = alpha / (1.0f + alpha);
            _coeffLP[1] = 1/ (1.0f + alpha);
            
            fcl_Hz = std::min(fcl_Hz, (0.5f * _fs_Hz));
            fcl_Hz = std::max(fcl_Hz, 0.0f);
            alpha = (6.28318530718f * fcl_Hz) / _fs_Hz;
            _coeffHP = 1.0f / (1.0f + alpha);
            */

            _lpFilt.set_cutoff(fch_Hz);
            _hpFilt.set_cutoff(fcl_Hz);
        }
        void set_gain(float gain) {
            _gain = gain;
        }

    private:
        float _inputLast;
        float _output;
        float _fs_Hz;
        float _coeffLP[2];
        float _coeffHP;
        float _gain;

        LowPass _lpFilt;
        HighPass _hpFilt;
};





#endif //BANDPASS_H