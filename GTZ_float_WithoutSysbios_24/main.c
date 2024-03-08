#include <stdio.h>
#include <math.h>
#include <time.h>
#define SAMPLING_RATE 8000  // Replace with your actual sampling rate
#define NUM_TARGETS 8
#define NUM_SAMPLES 8000
#define TICK_PERIOD (1/SAMPLING_RATE)

// Global variables
float magnitudes[NUM_TARGETS];
float samples[NUM_SAMPLES];
int Flag = 1;
char digit;

int i,n;

float coef_float[8] = {1.703275, 1.635585, 1.562297, 1.482867, 1.163138, 1.008835, 0.790074 , 0.559454};
char symbol_mat[4][4][1] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};

int freq1, freq2;
int mag1, mag2;

UInt32 time1, time2;


// Function prototypes
void goertzel_multi(const float samples[], int numSamples, float targetFrequencies[]);
void Frequency_detection(float magnitudes[]);
void Generate_tones(char digit);

// Function to perform the Goertzel algorithm for multiple frequencies
void goertzel_multi(const float samples[], int numSamples, float targetFrequencies[]) {
// to be completed
}
// Function to detect the maximum frequency and interpret the key pressed
void Frequency_detection(float magnitudes[]) {
// to be completed
												}

// Function to generate tones based on the pressed key
void Generate_tones(char digit)		 {
// to be completed

	//debawy can write your case/conditional statement here for freq1,freq2

	// tick function Clock_getTicks() = generates a uint_32 on each call corresponding to the number of clock ticks elapsed
	// when the clock period is set to 10ms, Clock_getTicks() outputs: 10, 20, 30, 40, 50...
	// we need a function that generates 205 ticks for the 205 samples
	mag1 = 32768.0, mag2 = 32768.0;
	for(tick = 0; tick < NUM_SAMPLES ; tick++){
		samples[i] =  mag1*sin(2.0*PI*freq1*TICK_PERIOD*tick) + mag2*sin(2.0*PI*freq2*TICK_PERIOD*tick);
	}

}

int main() {
    while (1) {
        printf("Type a key and press return:\n");
        scanf("\n%c", &digit);
        Generate_tones(digit);

        // Example usage
        float targetFrequencies[NUM_TARGETS] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};

        // Call the function to calculate magnitudes for each frequency
        goertzel_multi(samples, NUM_SAMPLES, targetFrequencies);

        // Detect the maximum frequency and interpret the key pressed
        Frequency_detection(magnitudes);
    }


}
