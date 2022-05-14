#ifndef EQ_H                            //Prevent multiple inclusions
#define EQ_H

#include "LowPass.h"                    //Include filter classes
#include "LowPass2.h"
#include "BandPass.h"
#include "BandPass2.h"
#include "HighPass.h" 
#include "HighPass2.h"
#include <math.h>

class EQ {                              //EQ Class
    public:                             //Public Members
        EQ(float cutoffFreqLow, float cutoffFreqHigh, float sampleFreq) :       //Class constructor, filters are initialized here
            _lpFilt(cutoffFreqLow, sampleFreq, 1.0f), _bpFilt(cutoffFreqLow, cutoffFreqHigh, sampleFreq, 1.0f), _hpFilt(cutoffFreqHigh, sampleFreq, 1.0f) 
            , _lpFilt2 (cutoffFreqLow, sampleFreq, 1.0f), _bpFilt2(cutoffFreqLow, cutoffFreqHigh, sampleFreq, 1.0f), _hpFilt2(cutoffFreqHigh, sampleFreq, 1.0f) {
            set_cutoff(cutoffFreqLow, cutoffFreqHigh);                          //Initialize coefficients
            set_gain(1.0f, 1.0f, 1.0f);                                         //Initialize gain
            _output = 0.0f;                                                     //Initialize output to 0
            _eqMode = 0;                                                        //Initialize filter mode
        }
        float update(float input) {                                             //Handles filtering input
            float out = 0.0f;                                                   //Temp output variable
            switch (_eqMode) {                                                  //EQ mode; 0 = 1st order, 1 = 2nd order
                default: {  
                    out = out + _lpFilt.update(input);                          //Update filters
                    out = out + _bpFilt.update(input);
                    out = out + _hpFilt.update(input);
                    break;
                }
                case 1: {
                    out = out + _lpFilt2.update(input);                         //Update filters
                    out = out + _bpFilt2.update(input);
                    out = out + _hpFilt2.update(input);
                    break;
                }
            }
            return out;
        }
        void set_cutoff(float cutoffFreqLow, float cutoffFreqHigh) {            //Handles updating coefficients
            float bpHigh = cutoffFreqHigh + 2000.0;                             //Create overlap in band pass and high/low pass
            float bpLow = cutoffFreqLow - 100.0;
            _lpFilt.set_cutoff(cutoffFreqLow);                                  //Set coefficients of member filters
            _lpFilt2.set_cutoff(cutoffFreqLow);
            _bpFilt.set_cutoff(cutoffFreqLow, cutoffFreqHigh);
            _hpFilt.set_cutoff(cutoffFreqHigh);
            _bpFilt2.set_cutoff(bpLow, bpHigh);
            _hpFilt2.set_cutoff(cutoffFreqHigh);
        }
        void set_gain(float lpGain, float bpGain, float hpGain) {               //Handles setting gain of filters
            _lpFilt.set_gain(lpGain);                                           //Set gain of member filters
            _bpFilt.set_gain(bpGain);
            _hpFilt.set_gain(hpGain);
            _lpFilt2.set_gain(lpGain);
            _bpFilt2.set_gain(bpGain);
            _hpFilt2.set_gain(hpGain);
        }
        void set_mode(int eqMode) {                                             //Set filter type of EQ
            _eqMode = eqMode;
        }
    private:                                                                    //Private members
        float _output;
        LowPass _lpFilt;
        HighPass _hpFilt;
        LowPass2 _lpFilt2;
        BandPass _bpFilt;
        BandPass2 _bpFilt2;
        HighPass2 _hpFilt2;
        int _eqMode;
};
#endif //EQ_H