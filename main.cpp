#include "mbed.h"                                                   //Library Inclusions
#include "rtos.h"
/*
#include "LowPass.h"
#include "HighPass.h"
#include "BandPass.h"
*/
#include "EQ.h"
#include <chrono>
#include <cstdio>

#define PIN_INPUT           PC_0                                    //Pin definitions
#define PIN_OUTPUT          PA_4
#define PIN_INDICATOR       PB_9
#define PIN_LOW_GAIN        PA_1
#define PIN_MID_GAIN        PA_7
#define PIN_HIGH_GAIN       PB_0
#define PIN_LOW_FREQ        PA_0
#define PIN_HIGH_FREQ       PC_1
#define PIN_LOW_MODE        PB_8
#define PIN_HIGH_MODE       PC_9

#define LOW_FREQ_MIN        150.0f                                   //Constant definitions
#define LOW_FREQ_MAX        500.0f
#define HIGH_FREQ_MIN       2000.0f
#define HIGH_FREQ_MAX       10000.0f
#define OFFSET              1.0f/3.3f
#define SAMPLE_RATE         48000.0f

DigitalIn       eqModeIn(PIN_LOW_MODE, PullDown);
AnalogIn        inputSig(PIN_INPUT);                                //Input/output signal objects
AnalogOut       outputSig(PIN_OUTPUT);
DigitalOut      indicator(PIN_INDICATOR);                           //Indicator LED shows program is running
AnalogIn        lowAmplitude_in(PIN_LOW_GAIN);                      //Control input objects
AnalogIn        lowFrequency_in(PIN_LOW_FREQ);
AnalogIn        highAmplitude_in(PIN_HIGH_GAIN);
AnalogIn        highFrequency_in(PIN_HIGH_FREQ);
AnalogIn        midAmplitude_in(PIN_MID_GAIN);
float           input =             0.0f;                           //Input/output signal value variables
float           output =            0.0f;
int             sampleInterval_us = int(1000000/SAMPLE_RATE);        //Sample period in microseconds
int             sampleFreq_actual = 1000000/sampleInterval_us;      //Actual sample frequency given rounded sample period
int             controlFreq =       20000;                          //Number of iterations between control updates
Timer           t;                                                  //Timer variable to keep track of time
EQ              eq(50.0f, 3000.0f, sampleFreq_actual);
void            process_eq();                                       //Function declarations  
void            update_controls();
float           scale(float in, float in_min, float in_max, float out_min, float out_max);

int main()                                                          //Main function
{  
    int ticks = 0;                                                  //Initialize ticks to 0
    t.start();                                                      //Begin timer
    auto us = t.elapsed_time().count();                             //Initialize us to initial time: https://forums.mbed.com/t/how-to-use-elapsed-time-count/10600/4
    auto us_old = us;                                               //Initialize us_old to initial time
    while(true){                                                    
        us = t.elapsed_time().count();                              //Update us to current time passed
        if(us - us_old >= sampleInterval_us) {                      //If time passed >= sampling period
            us_old = us_old + sampleInterval_us;                    //us_old increases by sample period
            process_eq();                                           //Update outputs based on inputs
            ticks++;                                                //Increment ticks
            if(ticks % int(sampleFreq_actual) == 0) {               //Every second
                indicator = !indicator;                             //Toggle indicator LED
                ticks = 0;                                          //reset ticks
                // outputSig = 1.0;                                     
            }
            if(ticks % controlFreq == 0) {                          //If time = control update time
                update_controls();                                  //Update controls
            }
        }
    }
}
void process_eq() {                                                 //Function to handle EQ filtering
    input = inputSig.read() - OFFSET;                               //Read input and remove DC offset 
    output = eq.update(input);                                      //Update each filter with input signal, account for filter modes
    outputSig.write(output + OFFSET);                               //Output filtered signal + DC offset                                                                         
}
void update_controls() {                                            //Function to handle reading control values and updating filters
    int lowCut = scale(lowFrequency_in.read(), 0.0f, 1.0f, LOW_FREQ_MIN, LOW_FREQ_MAX);         //Calculate cutoff frequencies based on control positions
    int highCut = scale(highFrequency_in.read(), 0.0f, 1.0f, HIGH_FREQ_MIN, HIGH_FREQ_MAX);
    eq.set_gain(lowAmplitude_in.read(), midAmplitude_in.read(), highAmplitude_in.read());       //Set filter cutoffs
    eq.set_cutoff(lowCut, highCut);
    eq.set_mode(eqModeIn.read());
}
float scale(float in, float in_min, float in_max, float out_min, float out_max) {               //Function to handle scaling input value based on given input/output range
    float in_percent = (in - in_min) / (in_max - in_min);                                       //Input percentage of input range
    return (in_percent * (out_max - out_min)) + out_min;                                        //Output = input percentage of output range offset by minimum
}