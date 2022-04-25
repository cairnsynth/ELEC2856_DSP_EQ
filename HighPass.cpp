#include "HighPass.h"
#include <algorithm>

HighPass::HighPass(float fc_Hz, float fs_Hz, float gain) {
    _fs_Hz = fs_Hz;
    set_cutoff(fc_Hz);
    set_gain(gain);
    _output = 0.0f;
    _inputLast = 0.0f;
}

float HighPass::update(float input) {
    _output = _coeff * (input - _inputLast + _output);
    _inputLast = input;
    return _output * _gain;
}

void HighPass::set_cutoff(float fc_Hz) {
    fc_Hz = std::min(fc_Hz, (0.5f * _fs_Hz));
    fc_Hz = std::max(fc_Hz, 0.0f);
    float alpha = (6.28318530718f * fc_Hz) / _fs_Hz;
    _coeff = 1.0f / (1.0f + alpha);
}

void HighPass::set_gain(float gain) {
    _gain = gain;
}