#include "LowPass.h"
#include <algorithm>

LowPass::LowPass(float fc_Hz, float fs_Hz, float gain) {
    _fs_Hz = fs_Hz;
    set_cutoff(fc_Hz);
    set_gain(gain);
    _output = 0.0f;
}

float LowPass::update(float input) {
    _output = _coeff[0] * input + _coeff[1] * _output;
    return _output * _gain;
}

void LowPass::set_cutoff(float fc_Hz) {
    fc_Hz = std::min(fc_Hz, (0.5f * _fs_Hz));
    fc_Hz = std::max(fc_Hz, 0.0f);
    float alpha = (6.28318530718f * fc_Hz) / _fs_Hz;
    _coeff[0] = alpha / (1.0f + alpha);
    _coeff[1] = 1/ (1.0f + alpha);
}

void LowPass::set_gain(float gain) {
    _gain = gain;
}