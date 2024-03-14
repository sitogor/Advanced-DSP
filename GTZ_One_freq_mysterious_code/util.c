/*
 *  ======== util.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

UInt32 time1, time2;

char digit;

int freq1, freq2;
int mag1, mag2;
float gtz_out_float[8], sample_float; // variables for floating point
int sample, gtz_out[8]; // variables for fixed point

char valid_input[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','*','#'};

short coef[8] = {0x6D02, 0x68B1, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x6521, 0x479C}; // Goertzel coefficients fixed-point
float coef_float[8] = {1.703275, 1.635585, 1.562297, 1.482867, 1.163138, 1.008835, 0.790074 , 0.559454}; // Goertzel coefficients floating-point


int freq_mat[16][2] = {
		{697, 1209},
		{697, 1336},
		{697, 1477},
		{697, 1633},
		{770, 1209},
		{770, 1336},
		{770, 1477},
		{770, 1633},
		{852, 1209},
		{852, 1336},
		{852, 1477},
		{852, 1633},
		{941, 1209},
		{941, 1336},
		{941, 1477},
		{941, 1633}
};



void task0_dtmfGen(void)
{



// This task is used to read the symbol from the keyboard and set freq1 and freq2

	System_printf("\n I am in Task 0:\n");
	System_flush();

	Task_sleep(500); // to allow the GTZ to be calculated.


}

void task1_dtmfDetect(void)
{
	int match,x = 0;

	System_printf("\n I am in Task 1 for the first time, please wait:\n");
	System_flush();

	while (1) {
			match = 0;

			while (1) { // error checking loop
				System_printf("\n The code will only generate the 1st frequency component of the chosen character and the algorithm will check for the presence of 697Hz");
				System_printf("\n Enter a permitted character or number (0,1,2,3,4,5,6,7,8,9,A,B,C,D,*,#): \n");
				System_flush();

				scanf(" %c", &digit);

				for(x=0; x<=15; x++){
					if ((char)digit == valid_input[x]){
					match = 1;
					}

				}

				if (match == 1){
					break;
				}
				else {
					System_printf("\n This character is not allowed. \n");
					System_flush();
				}
			}

			switch(digit)// choosing the frequency
			{
				case '1':
					freq1 = freq_mat[0][0];
					freq2 = freq_mat[0][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '2':
					freq1 = freq_mat[1][0];
					freq2 = freq_mat[1][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '3':
					freq1 = freq_mat[2][0];
					freq2 = freq_mat[2][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '4':
					freq1 = freq_mat[4][0];
					freq2 = freq_mat[4][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '5':
					freq1 = freq_mat[5][0];
					freq2 = freq_mat[5][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '6':
					freq1 = freq_mat[6][0];
					freq2 = freq_mat[6][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '7':
					freq1 = freq_mat[8][0];
					freq2 = freq_mat[8][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '8':
					freq1 = freq_mat[9][0];
					freq2 = freq_mat[9][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '9':
					freq1 = freq_mat[10][0];
					freq2 = freq_mat[10][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '0':
					freq1 = freq_mat[13][0];
					freq2 = freq_mat[13][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case 'A':
					freq1 = freq_mat[3][0];
					freq2 = freq_mat[3][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case 'B':
					freq1 = freq_mat[7][0];
					freq2 = freq_mat[7][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case 'C':
					freq1 = freq_mat[11][0];
					freq2 = freq_mat[11][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case 'D':
					freq1 = freq_mat[15][0];
					freq2 = freq_mat[15][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '*':
					freq1 = freq_mat[12][0];
					freq2 = freq_mat[12][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;
				case '#':
					freq1 = freq_mat[14][0];
					freq2 = freq_mat[14][1];
					mag1 = 32768.0;
					mag2 = 32768.0;
					break;

			}




		System_printf("\n You entered the value %c which corresponds to frequency %d Hz and %d Hz \n ", digit, freq1, freq2);
		System_printf("Please wait for the algorithm \n");
		System_flush();

		Task_sleep(500);  // time to calculate the goertzel value

		System_printf("\n Testing against 697Hz, The GTZ for fixed-point is %d \n", gtz_out[0]);
		System_printf("\n Testing against 697Hz, The GTZ for floating-point is %f \n", gtz_out_float[0]);
		System_flush();
		if (gtz_out[0] > 1000)// cheking the fixed point results
			{
			System_printf("\n The frequency 697Hz is detected with fixed-point arithmetic. \n");
			System_flush();
		}
		else {
			System_printf("\n The frequency 697Hz is not detected with fixed-point arithmetic. \n");
			System_flush();
		}
		if (gtz_out_float[0] > 100000)// checking the floating point results
			{
			System_printf("\n The frequency 697Hz is detected with floating-point arithmetic. \n");
			System_flush();
			}
		else {
			System_printf("\n The frequency 697Hz is not detected with floating-point arithmetic. \n");
			System_flush();
		}
		System_printf("\n I am leaving Task 1, please wait to get the next GTZ \n");
		System_flush();
	}

	}






