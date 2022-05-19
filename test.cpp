#include "EQ.h"
#include "GenericBuffer.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>

#define     SAMPLE_RATE 48000.0f
#define     PI 3.1415f

int         sampleInterval_us = int(1000000.0f/SAMPLE_RATE);        //Sample period in microseconds
int         sampleFreq_actual = 1000000.0f/sampleInterval_us;       //Actual sample frequency given rounded sample period
EQ          eq(300.0f, 4000.0f, sampleFreq_actual);                 //Initialize EQ

float       measure(float freq);                                              //Function Declaration
void        write_csv(std::list<std::string> outV, std::string filePath, std::string headers);
void        test_eq(EQ &eq);
void        test_g_buffer();

class SignalGen {                                                       //Signal generator class
    public:
        SignalGen(float freq) {                                         //Constructor
            _freq = freq;
        }

        float step(float time) {
            _sample = sin(2*PI * time * _freq);
            return _sample;
        }

    private:
        float _freq;
        float _sample;

};

class RMS {
    public:
        RMS() {
            reset();
        }
        void update(float input) {
            _sum = _sum + (input * input);
            _samples++;
        }
        float get() {
            return sqrt(_sum/_samples);
        }
        void reset() {
            _sum = 0;
            _samples = 0;
        }
        private:
            float _sum;
            int _samples;
};

int main() {
    //eq.set_cutoff(1000.0, 1000.0);
    // float startFreq = 1.0f;
    // float endFreq = 20000.0f;
    // float freqInterval = 50.0f;

    // std::list<std::string> outs;

    // for(float freq = startFreq; freq <= endFreq; freq += freqInterval) {
    //     std::string tmpS;
    //     tmpS = std::to_string(freq);

    //     eq.set_gain(1.0f, 1.0f, 1.0f);
    //     tmpS = tmpS + "," + std::to_string(measure(freq));
    //     eq.set_gain(1.0f, 0.0f, 0.0f);
    //     tmpS = tmpS + "," + std::to_string(measure(freq));
    //     eq.set_gain(0.0f, 1.0f, 0.0f);
    //     tmpS = tmpS + "," + std::to_string(measure(freq));
    //     eq.set_gain(0.0f, 0.0f, 1.0f);
    //     tmpS = tmpS + "," + std::to_string(measure(freq));

    //     outs.push_back(tmpS);
    // }
    // write_csv(outs, "C://Users//12luk//Desktop//EQ_outputs.txt");

    test_eq(eq);
    // test_eq_response(eq);
    //test_g_buffer();
}

float measure(float freq) {
    SignalGen sig(freq);
    RMS rms;
    float time = 0.0f;
    float interval = 1/SAMPLE_RATE;

    while(time < 0.5f) {
        if(time < 0.2f) {
            rms.reset();
        }

        float sample = sig.step(time);
        sample = eq.update(sample);
        rms.update(sample);
        time += interval;
    }
    return rms.get();
}
void test_eq(EQ &eq) {
    float startFreq = 1.0f;
    float endFreq = 20000.0f;
    float freqInterval = 50.0f;

    std::list<std::string> outs;
    for(float freq = startFreq; freq <= endFreq; freq += freqInterval) {
        std::string tmpS;
        tmpS = std::to_string(freq);

        eq.set_gain(1.0f, 1.0f, 1.0f);
        tmpS = tmpS + "," + std::to_string(measure(freq));
        eq.set_gain(1.0f, 0.0f, 0.0f);
        tmpS = tmpS + "," + std::to_string(measure(freq));
        eq.set_gain(0.0f, 1.0f, 0.0f);
        tmpS = tmpS + "," + std::to_string(measure(freq));
        eq.set_gain(0.0f, 0.0f, 1.0f);
        tmpS = tmpS + "," + std::to_string(measure(freq));

        outs.push_back(tmpS);
    }
    write_csv(outs, "C://Users//12luk//Desktop//EQ_outputs2.txt", "Freq,EQ Out,LP Out,BP Out,HP Out\n");
}
void test_g_buffer() {
    GenericBuffer<float> testBuffer(3);
    float testCoeff[3] = {10.0f, 10.0f, 10.0f};
    for(int i = 0; i < 3; i++) {
        testBuffer.shift_in(i);
    }
    for(int i = 0; i < 3; i++) {
        float bufferOut = testBuffer.get(i);
        std::cout << bufferOut << std::endl;
    }
    std::cout << testBuffer.multiply(testCoeff) << std::endl;
}


void write_csv(std::list<std::string> outV, std::string filePath, std::string headers) {
    std::ofstream outFile;
    outFile.open(filePath);

    if(!outFile.is_open()) {
        std::cerr << "FILE COULD NOT BE CREATED" << std::endl;
    }

    outFile << headers;

    for(std::string s : outV) {
        outFile << s << std::endl;
    }
    outFile.close();
}

