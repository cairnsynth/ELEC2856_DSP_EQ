#ifndef HIGHPASS_H
#define HIGHPASS_H

class HighPass {
    public:
        HighPass(float fc_Hz, float fs_Hz, float gain);

        float update(float input);
        void set_cutoff(float fc_Hz);
        void set_gain(float gain);

    private:
        float _inputLast;
        float _output;
        float _fs_Hz;
        float _coeff;
        float _gain;
};





#endif //HIGHPASS_H