#ifndef BANDPASS_H                     
#define BANDPASS_H

#include <algorithm>                                    //Inclusions
#include "LowPass.h"
#include "HighPass.h"

class BandPass {                                        //Band pass filter class
    public:                                             //Public members
        BandPass(float lowCutoff, float highCutoff, float sampleFreq, float gain) :         //Class constructor
            _lpFilt(highCutoff, sampleFreq, 1.0f), _hpFilt(lowCutoff, sampleFreq, 1.0f) {   //Explicit initialization of filters
            _sampleFreq = sampleFreq;                                                       //Initialize sample frequency
            set_cutoff(lowCutoff, highCutoff);                                              //Initialize cutoff frequency
            set_gain(gain);                                                                 //Initialize gain
            _output = 0.0f;                                                                 //Initialize output
        }
        float update(float input) {                     //Function handles updating output of filters
            float lowOut = _lpFilt.update(input);       //Input is passed through low pass
            return _hpFilt.update(lowOut) * _gain;      //Low pass output is passed through high pass, gain applied, returned
        }
        void set_cutoff(float fcl_Hz, float fch_Hz) {   //Function handles setting cutoff frequency
            _lpFilt.set_cutoff(fch_Hz);                 //Set cutoff of low pass
            _hpFilt.set_cutoff(fcl_Hz);                 //Set cutoff of highpass
        }
        void set_gain(float gain) {                     //Function handles setting gain of band pass
            _gain = gain;
        }
    private:                                            //Private members
        float _output;
        float _sampleFreq;
        float _gain;
        LowPass _lpFilt;
        HighPass _hpFilt;
};
#endif //BANDPASS_H