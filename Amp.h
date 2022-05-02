#ifndef AMP_H
#define AMP_H

class Amp {                             //Basic amplifier class
    public:                             //Public members
        Amp(float g) {                  //Constructor
            set_gain(g);                //Initialize gain
        }

        void set_gain(float g) {        //Handles setting gain
            _gain = g;
        }

        float update(float input) {     //Handles updating output given an input
            return input * _gain;
        }

    private:                            //Private members
        float _gain;
};

#endif //AMP_H