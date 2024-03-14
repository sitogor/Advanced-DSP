#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "gtz.h"

#define SAMPLING_RATE 8000.0  // Replace with your actual sampling rate
#define NUM_TARGETS 8
#define NUM_SAMPLES 205
#define TICK_PERIOD (1/SAMPLING_RATE)


// Global variables
float magnitudes[NUM_TARGETS];
float samples[NUM_SAMPLES];

int Flag = 1;
char digit;
char key;
int i,n,N;
float tick;

float coef_float[8] = {1.703275, 1.635585, 1.562297, 1.482867, 1.163138, 1.008835, 0.790074 , 0.559454};
char pad[4][4] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
float targetFrequencies[NUM_TARGETS] = {697.0, 770.0, 852.0, 941.0, 1209.0, 1336.0, 1477.0, 1633.0};

int freq1, freq2;
int mag1, mag2;

float max_float0, max_float1;
int max_float_index[2];

int mismatch = 0;

// Function prototypes
void goertzel_multi(const float samples[], int numSamples, float targetFrequencies[]);
void Frequency_detection(float magnitudes[], int mismatch);
void Generate_tones(char digit);

// Function to perform the Goertzel algorithm for multiple frequencies
void goertzel_multi(const float samples[], int numSamples, float targetFrequencies[]) {
// to be completed
	for (i = 0; i < NUM_TARGETS; i ++)
	{
		float Goertzel_Value = 0;
		float delay;
		float delay_1 = 0;
	    float delay_2 = 0;
		float prod1, prod2, prod3;

		for (n = 0; n < numSamples ; n++) {

			prod1 = delay_1*coef_float[i];
			delay = samples[n] + prod1 - delay_2;
			delay_2 = delay_1;
			delay_1 = delay;
			//printf("\n prod1 = %f\n", prod1);

			if (n==numSamples-1)
			{
			prod1 = delay_1 * delay_1;
			prod2 = delay_2 * delay_2;
			prod3 = delay_1 * coef_float[i];
			prod3 = prod3 * delay_2;
			Goertzel_Value = prod1 + prod2 - prod3;
			delay_1 = delay_2 = 0;
			magnitudes[i] = Goertzel_Value/10000000000;
			// printf("\n magnitude %d is: %f", i, magnitudes[i]);
			}
	}

}
}
// Function to detect the maximum frequency and interpret the key pressed
void Frequency_detection(float magnitudes[], int mismatch) {
// to be completed
	max_float0 = magnitudes[0], max_float1 = magnitudes[4];
	max_float_index[0] = 0; max_float_index[1] = 0;

	for (i=1 ; i<=3; i++){
		{
			//fixed point freq from 697 to 941
			if(magnitudes[i]>max_float0)
			{
				max_float0 = magnitudes[i];
				max_float_index[0] = i;
			}

			//fixed point freq from 1209 to 1633
			if(magnitudes[i+4]>max_float1)
			{

				max_float1 = magnitudes[i+4];
				max_float_index[1] = i;
			}
		}


	}
	key = pad[max_float_index[0]][max_float_index[1]];

	if (mismatch == 0){
		printf("\n\nFloating point:\n");
		printf("\n The GTZ for 697 Hz is %f\n", magnitudes[0]);
		printf("\n The GTZ for 770 Hz is %f\n", magnitudes[1]);
		printf("\n The GTZ for 852 Hz is %f\n", magnitudes[2]);
		printf("\n The GTZ for 941 Hz is %f\n", magnitudes[3]);
		printf("\n The GTZ for 1209 Hz is %f\n", magnitudes[4]);
		printf("\n The GTZ for 1336 Hz is %f\n", magnitudes[5]);
		printf("\n The GTZ for 1477 Hz is %f\n", magnitudes[6]);
		printf("\n The GTZ for 1633 Hz is %f\n", magnitudes[7]);
		printf("\nThe key you pressed is: %c\n", key);
	}
	mismatch = 0;
}

// Function to generate tones based on the pressed key
void Generate_tones(char digit)		 {
// to be completed

	//debawy can write your case/conditional statement here for freq1,freq2

	// tick function Clock_getTicks() = generates a uint_32 on each call corresponding to the number of clock ticks elapsed
	// when the clock period is set to 10ms, Clock_getTicks() outputs: 10, 20, 30, 40, 50...
	// we need a function that generates 205 ticks for the 205 samples

	switch(digit)
		{
			case '1':
				freq1 = targetFrequencies[0];
				freq2 = targetFrequencies[4];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '2':
				freq1 = targetFrequencies[0];
				freq2 = targetFrequencies[5];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '3':
				freq1 = targetFrequencies[0];
				freq2 = targetFrequencies[6];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case 'A':
				freq1 = targetFrequencies[0];
				freq2 = targetFrequencies[7];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '4':
				freq1 = targetFrequencies[1];
				freq2 = targetFrequencies[4];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '5':
				freq1 = targetFrequencies[1];
				freq2 = targetFrequencies[5];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '6':
				freq1 = targetFrequencies[1];
				freq2 = targetFrequencies[6];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case 'B':
				freq1 = targetFrequencies[1];
				freq2 = targetFrequencies[7];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '7':
				freq1 = targetFrequencies[2];
				freq2 = targetFrequencies[4];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '8':
				freq1 = targetFrequencies[2];
				freq2 = targetFrequencies[5];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '9':
				freq1 = targetFrequencies[2];
				freq2 = targetFrequencies[6];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case 'C':
				freq1 = targetFrequencies[2];
				freq2 = targetFrequencies[7];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '*':
				freq1 = targetFrequencies[3];
				freq2 = targetFrequencies[4];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '0':
				freq1 = targetFrequencies[3];
				freq2 = targetFrequencies[5];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '#':
				freq1 = targetFrequencies[3];
				freq2 = targetFrequencies[6];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case 'D':
				freq1 = targetFrequencies[3];
				freq2 = targetFrequencies[7];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			default:
				printf("\n Error! The character you provided is not correct. Please wait and ignore the following results... \n");
				mag1 = 0;
				mag2 = 0;
				mismatch = 1;
				break;
		}


	for(N = 0; N < NUM_SAMPLES ; N++){
		tick = (float)N;

		samples[N] =  mag1*sin(2.0*PI*freq1*TICK_PERIOD*tick) + mag2*sin(2.0*PI*freq2*TICK_PERIOD*tick);
		//printf("\n sample %d: %f \n", N, samples[N]);
	}

}

int main() {
	while(1){

        printf("\n Type a key and press return:\n");
        scanf("\n%c", &digit);


        Generate_tones(digit);

        // Example usage

        // Call the function to calculate magnitudes for each frequency
        goertzel_multi(samples, NUM_SAMPLES, targetFrequencies);

        // Detect the maximum frequency and interpret the key pressed
        Frequency_detection(magnitudes, mismatch);


	}
}
