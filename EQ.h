#ifndef EQ_H
#define EQ_H

#include "LowPass.h"
#include "LowPass2.h"
#include "BandPass.h"
#include "BandPass2.h"
#include "HighPass.h" 
#include "HighPass2.h"
#include <math.h>

class EQ {
    public:
        EQ(float cutoffFreqLow, float cutoffFreqHigh, float sampleFreq) :
            _lpFilt(cutoffFreqLow, sampleFreq, 1.0f), _bpFilt(cutoffFreqLow, cutoffFreqHigh, sampleFreq, 1.0f), _hpFilt(cutoffFreqHigh, sampleFreq, 1.0f) 
            , _lpFilt2 (cutoffFreqLow, sampleFreq, 1.0f), _bpFilt2(cutoffFreqLow, cutoffFreqHigh, sampleFreq, 1.0f), _hpFilt2(cutoffFreqHigh, sampleFreq, 1.0f) {
            set_cutoff(cutoffFreqLow, cutoffFreqHigh);
            set_gain(1.0f, 1.0f, 1.0f);
            _output = 0.0f;
            _eqMode = 0;
            _cutoffFreqHigh_old = 0.0f;
            _cutoffFreqLow_old = 0.0f;
        }
        float update(float input) {
            float out = 0.0f;
            switch (_eqMode) {
                default: {
                    out = out + _lpFilt.update(input);
                    out = out + _bpFilt.update(input);
                    out = out + _hpFilt.update(input);
                    break;
                }
                case 1: {
                    out = out + _lpFilt2.update(input);
                    out = out + _bpFilt2.update(input);
                    out = out + _hpFilt2.update(input);
                    break;
                }
            }

            return out;
        }
        void set_cutoff(float cutoffFreqLow, float cutoffFreqHigh) {
            float bpHigh = cutoffFreqHigh + 2000.0;
            float bpLow = cutoffFreqLow - 100.0;
            _lpFilt.set_cutoff(cutoffFreqLow);
            _lpFilt2.set_cutoff(cutoffFreqLow);
            _bpFilt.set_cutoff(cutoffFreqLow, cutoffFreqHigh);
            _hpFilt.set_cutoff(cutoffFreqHigh);
            _bpFilt2.set_cutoff(bpLow, bpHigh);
            _hpFilt2.set_cutoff(cutoffFreqHigh);
            _cutoffFreqHigh_old = cutoffFreqHigh;
            _cutoffFreqLow_old = cutoffFreqLow;
        }
        void set_gain(float lpGain, float bpGain, float hpGain) {
            _lpFilt.set_gain(lpGain);
            _bpFilt.set_gain(bpGain);
            _hpFilt.set_gain(hpGain);
            _lpFilt2.set_gain(lpGain);
            _bpFilt2.set_gain(bpGain);
            _hpFilt2.set_gain(hpGain);
        }
        void set_mode(int eqMode) {
            _eqMode = eqMode;
        }

    private:
        float _output;
        LowPass _lpFilt;
        HighPass _hpFilt;
        LowPass2 _lpFilt2;
        BandPass _bpFilt;
        BandPass2 _bpFilt2;
        HighPass2 _hpFilt2;
        int _eqMode;
        float _cutoffFreqLow_old;
        float _cutoffFreqHigh_old;
};

#endif //EQ_H