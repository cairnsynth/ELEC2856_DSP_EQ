#include "mbed.h"
#include "rtos.h"
#include "LowPass.h"
#include "HighPass.h"
#include "BandPass.h"
#include "Amp.h"
#include <chrono>
#include <cstdint>
#include <cstdlib>

#define PIN_INPUT      PC_0
#define PIN_OUTPUT     PA_4
#define PIN_INDICATOR  PA_5
#define PIN_LOW_GAIN   PA_1
#define PIN_MID_GAIN   PA_7
#define PIN_HIGH_GAIN  PB_0
#define PIN_LOW_FREQ   PA_0
#define PIN_HIGH_FREQ  PC_1

#define LOW_FREQ_MIN   20.0f
#define LOW_FREQ_MAX   500.0f
#define HIGH_FREQ_MIN  1000.0f
#define HIGH_FREQ_MAX  10000.0f

static BufferedSerial pc(USBTX, USBRX);

AnalogIn inputSig(PIN_INPUT);
AnalogOut  outputSig(PIN_OUTPUT);
float input;
float output;

DigitalOut indicator(PIN_INDICATOR);

AnalogIn lowAmplitude_in(PIN_LOW_GAIN);
AnalogIn lowFrequency_in(PIN_LOW_FREQ);
AnalogIn highAmplitude_in(PIN_HIGH_GAIN);
AnalogIn highFrequency_in(PIN_HIGH_FREQ);
AnalogIn midAmplitude_in(PIN_MID_GAIN);

const float offset = 0.3030303f; 
const float pi     = 3.14159265358979;

float lowFreq = 50.0f;

float sampleFreq = 48000.0f;
int sampleInterval_us(int(1000000/sampleFreq));
int sampleFreq_actual(1000000/sampleInterval_us);

int controlFreq = 10000;

float highFreq = 150.0f;

LowPass lpFilt(lowFreq, sampleFreq_actual, 1.0f);
HighPass hpFilt(highFreq, sampleFreq_actual, 1.0f);
BandPass bpFilt(lowFreq, highFreq, sampleFreq_actual, 1.0f);

Timer t;

void process_eq();
void update_controls();
float scale(float in, float in_min, float in_max, float out_min, float out_max);

int main()
{  
    int ticks = 0;
    t.start();
    auto us = t.elapsed_time().count();                     //https://forums.mbed.com/t/how-to-use-elapsed-time-count/10600/4
    auto us_old = t.elapsed_time().count();
    
    while(true){
        us = t.elapsed_time().count();
        if(us - us_old >= sampleInterval_us) {
            us_old = us;


            process_eq();

            ticks++;
            if(ticks % int(sampleFreq_actual) == 0) {
                indicator = !indicator;
                ticks = 0;
            }

            if(ticks % controlFreq == 0) {
                update_controls();
            }
        }
        
    }
}

void process_eq() {
    input = inputSig.read() - offset;
    //output = bpFilt.update(input);
    output = hpFilt.update(input) + lpFilt.update(input) + bpFilt.update(input);
    outputSig.write(output + offset);
}

void update_controls() {
    lpFilt.set_gain(lowAmplitude_in.read());
    hpFilt.set_gain(highAmplitude_in.read());
    bpFilt.set_gain(midAmplitude_in.read());

    int lowCut = scale(lowFrequency_in.read(), 0.0f, 1.0f, LOW_FREQ_MIN, LOW_FREQ_MAX);
    int highCut = scale(highFrequency_in.read(), 0.0f, 1.0f, HIGH_FREQ_MIN, HIGH_FREQ_MAX);

    lpFilt.set_cutoff(lowCut);
    hpFilt.set_cutoff(highCut);
    bpFilt.set_cutoff(lowCut, highCut);
/*
    printf("\n\n\n\n\n\n\n\n\n");
    printf("Low Cut   = %i\n", lowCut);
    printf("High Cut  = %i\n", highCut);
*/
}

float scale(float in, float in_min, float in_max, float out_min, float out_max) {
    float in_percent = in_min / (in / (in_max - in_min));
    return (in_percent * (out_max - out_min)) + out_min;
}