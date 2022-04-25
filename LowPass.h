#ifndef LOWPASS_H
#define LOWPASS_H

class LowPass {
    public:
        LowPass(float fc_Hz, float fs_Hz, float gain);
        float update(float input);
        void set_cutoff(float fc_Hz);
        void set_gain(float gain);

    private:
        float _output;
        float _fs_Hz;
        float _coeff[2];
        float _gain;
};





#endif //LOWPASS_H