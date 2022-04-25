#include "mbed.h"
#include "rtos.h"
#include "LowPass.h"
#include "HighPass.h"
#include "Amp.h"
#include <chrono>
#include <cstdint>
#include <cstdlib>

static BufferedSerial pc(USBTX, USBRX);

AnalogIn inputSig(PC_0);
AnalogOut  outputSig(PA_4);
float input;
float output;

DigitalOut indicator(PA_5);

AnalogIn lowAmplitude_in(PA_0);
AnalogIn lowFrequency_in(PA_1);
AnalogIn highAmplitude_in(PB_0);
AnalogIn highFrequency_in(PC_1);

const float offset = 0.3030303f; 
const float pi     = 3.14159265358979;

float lowAmp;
float lowFreq = 100.0f;

float sampleFreq = 48000.0f;
int sampleInterval_us(int(1000000/sampleFreq));
int sampleFreq_actual(1000000/sampleInterval_us);

int controlFreq(1000);

float highAmp;
float highFreq = 100.0f;

LowPass lpFilt(lowFreq, sampleFreq, 1.0f);
HighPass hpFilt(highFreq, sampleFreq, 1.0f);

Timer t;

void process_eq();

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
                lpFilt.set_gain(lowAmplitude_in.read());
                hpFilt.set_gain(highAmplitude_in.read());
            }
        }
        
    }
}

void process_eq() {
    input = inputSig.read() - offset;
    output = hpFilt.update(input) + lpFilt.update(input);
    outputSig.write(output + offset);
}