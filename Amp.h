#ifndef AMP_H
#define AMP_H

class Amp {
    public:
        Amp(float g) {
            set_gain(g);
        }

        void set_gain(float g) {
            _gain = g;
        }

        float update(float input) {
            return input * _gain;
        }

    private:
        float _gain;
};

#endif //AMP_H