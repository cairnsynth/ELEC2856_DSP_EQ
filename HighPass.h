#ifndef HIGHPASS_H
#define HIGHPASS_H

#include <algorithm>                                                    //Needed for min/max                                            

class HighPass {                                                        //High pass filter class
    public:                                                             //Public members
        HighPass(float cutoffFreq, float sampleFreq, float gain) {      //Class constructor
            _sampleFreq = sampleFreq;                                   //Initialize sample frequency
            set_cutoff(cutoffFreq);                                     //Initialize coefficient
            set_gain(gain);                                             //Initialize gain
            _output = 0.0f;                                             //Initialize output
            _inputLast = 0.0f;                                          //Initialize old input
        }

        float update(float input) {                                     //Handles updating output based on new and old input
            _output = _coeff * (input - _inputLast + _output);          //calcualtes filtered output
            _inputLast = input;                                         //Sets old input
            return _output * _gain;                                     //Returns output with gain applied
        }
        void set_cutoff(float cutoffFreq) {                             //Handles calculating coefficients
            cutoffFreq = std::min(cutoffFreq, (0.5f * _sampleFreq));    //Cutoff = 0.5*sampleFreq if cutoff > 0.5*sampleFreq
            cutoffFreq = std::max(cutoffFreq, 0.0f);                    //Cutoff = 0 if cutoff < 0
            float alpha = (6.28318530718f * cutoffFreq) / _sampleFreq;  //2*PI*cutoff / sample frequency
            _coeff = 1.0f / (1.0f + alpha);                             //1 / 1+alpha
        }
        void set_gain(float gain) {                                     //Handles setting gain 
            _gain = gain;
        }

    private:                                                            //Private members
        float _inputLast;
        float _output;
        float _sampleFreq;
        float _coeff;
        float _gain;
};





#endif //HIGHPASS_H