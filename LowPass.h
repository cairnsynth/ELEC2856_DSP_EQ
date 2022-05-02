#ifndef LOWPASS_H
#define LOWPASS_H

#include <algorithm>                                                        //Needed for min/max

class LowPass {                                                             //Low pass filter class
    public:                                                                 //Public members
        LowPass(float cutoffFreq, float sampleFreq, float gain) {           //Class constructor
            _sampleFreq = sampleFreq;                                       //Initialize sample frequency
            set_cutoff(cutoffFreq);                                         //Initialize coefficients
            set_gain(gain);                                                 //Initialize gain
            _output = 0.0f;                                                 //Initialize output
        }
        float update(float input) {                                         //Function handles updating output based on input
            _output = _coeff[0] * input + _coeff[1] * _output;              //Calculate filtered output
            return _output * _gain;                                         //Return output with gain applied
        }
        void set_cutoff(float cutoffFreq) {                                 //Handles calculating coefficients based on cutoff
            cutoffFreq = std::min(cutoffFreq, (0.5f * _sampleFreq));        //Cutoff = 0.5*sampleFreq if cutoff > 0.5*sampleFreq
            cutoffFreq = std::max(cutoffFreq, 0.0f);                        //Cutoff = 0 if cutoff < 0
            float alpha = (6.28318530718f * cutoffFreq) / _sampleFreq;      //2*PI*cutoff / sample frequency
            _coeff[0] = alpha / (1.0f + alpha);                             //alpha / 1+alpha
            _coeff[1] = 1/ (1.0f + alpha);                                  //1 / 1+alpha
        }
        void set_gain(float gain) {                                         //Handles setting gain
            _gain = gain;
        }

    private:                                                                //Private members
        float _output;
        float _sampleFreq;
        float _coeff[2];
        float _gain;
};





#endif //LOWPASS_H