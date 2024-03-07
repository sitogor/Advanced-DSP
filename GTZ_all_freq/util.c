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
#include <time.h>
#include <stdlib.h>

#include <math.h>
#include "gtz.h"

#define threshold 1000

UInt32 time1, time2;

char digit;
int freq1, freq2;
int mag1, mag2;

int sample, gtz_out[2][4], start, tdiff_fixed, tdiff_float;
float sample_float, gtz_out_float[2][4];

short coef[8] = {0x6D02, 0x68B1, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x3290, 0x23CE}; // Goertzel coefficients
float coef_float[8] = {1.703275, 1.635585, 1.562297, 1.482867, 1.163138, 1.008835, 0.790074 , 0.559454};

int freq_mat[2][4] = {{697, 770, 852, 941}, {1209, 1336, 1477, 1633}};
char symbol_mat[4][4][1] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};



void task0_dtmfGen(void)
{

	int mismatch = 0;
// This task is used to read the symbol from the keyboard and set freq1 and freq2
	while(1)
	{


		System_printf("\n Enter a permitted character or number\n 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, *, #: \n");
		System_flush();

		scanf(" %c", &digit);



		switch(digit)
		{
			case '1':
				freq1 = freq_mat[0][0];
				freq2 = freq_mat[1][0];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '2':
				freq1 = freq_mat[0][0];
				freq2 = freq_mat[1][1];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '3':
				freq1 = freq_mat[0][0];
				freq2 = freq_mat[1][2];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case 'A':
				freq1 = freq_mat[0][0];
				freq2 = freq_mat[1][3];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '4':
				freq1 = freq_mat[0][1];
				freq2 = freq_mat[1][0];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '5':
				freq1 = freq_mat[0][1];
				freq2 = freq_mat[1][1];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '6':
				freq1 = freq_mat[0][1];
				freq2 = freq_mat[1][2];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case 'B':
				freq1 = freq_mat[0][1];
				freq2 = freq_mat[1][3];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '7':
				freq1 = freq_mat[0][2];
				freq2 = freq_mat[1][0];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '8':
				freq1 = freq_mat[0][2];
				freq2 = freq_mat[1][1];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '9':
				freq1 = freq_mat[0][2];
				freq2 = freq_mat[1][2];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case 'C':
				freq1 = freq_mat[0][2];
				freq2 = freq_mat[1][3];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '*':
				freq1 = freq_mat[0][3];
				freq2 = freq_mat[1][0];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '0':
				freq1 = freq_mat[0][3];
				freq2 = freq_mat[1][1];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case '#':
				freq1 = freq_mat[0][3];
				freq2 = freq_mat[1][2];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			case 'D':
				freq1 = freq_mat[0][3];
				freq2 = freq_mat[1][3];
				mag1 = 32768.0;
				mag2 = 32768.0;
				break;
			default:
				System_printf("Error! The character you provided is not correct. Please wait and ignore the following results...");
				System_flush();
				mag1 = 0;
				mag2 = 0;
				mismatch = 1;
				break;
		}

		if (mismatch == 0){
		System_printf("\n You entered the value %c which corresponds to frequency %d and %d \n ", digit, freq1, freq2);
		System_printf("Please wait for the algorithm \n");
		System_flush();
		}
		Task_sleep(500); // to allow the GTZ to be calculated.


	}
}
void task1_dtmfDetect(void)
{
	int f1_fixed=0, f2_fixed=0, f1_float=0, f2_float=0, x, max_fixed0, max_fixed1;
	float max_float0, max_float1;
	int max_fixed_index[2], max_float_index[2];
	char dig_fixed, dig_float;


	while (1)
	{


		Task_sleep(500);  /* 0.25 second for 125us tick */

		// detects the highest frequencies

		// set the initial maxima to the first elements
		max_fixed0 = gtz_out[0][0];
		max_fixed1 = gtz_out[1][0];
		max_float0 = gtz_out_float[0][0];
		max_float1 = gtz_out_float[1][0];
		max_fixed_index[0] = 0; max_fixed_index[1] = 0;
		max_float_index[0] = 0; max_float_index[1] = 0;

		// iterates through the gtz values to find the indecies of the highest values
		for (x=1; x<=3; x++)
		{
			//fixed point freq from 697 to 941
			if(gtz_out[0][x]>max_fixed0)
			{
				System_printf("The goertzel out for 697-941Hz x = %d is: %d", x, gtz_out[x]);
				max_fixed0 = gtz_out[0][x];
				max_fixed_index[0] = x;
			}

			//fixed point freq from 1209 to 1633
			if(gtz_out[1][x]>max_fixed1)
			{
				System_printf("The goertzel out for 1209-1633Hz x = %d is: %d", x, gtz_out[x]);
				max_fixed1 = gtz_out[1][x];
				max_fixed_index[1] = x;
			}

			//float freq from 697 to 941
			if(gtz_out_float[0][x]>max_float0)
			{
				max_float0 = gtz_out_float[0][x];
				max_float_index[0] = x;
			}

			//float freq from 1209 to 1633
			if(gtz_out_float[1][x]>max_float1)
			{
				max_float1 = gtz_out_float[1][x];
				max_float_index[1] = x;
			}
		}

		f1_fixed = freq_mat[0][max_fixed_index[0]]; //detected values for fixed point case
		f2_fixed = freq_mat[1][max_fixed_index[1]];
		dig_fixed = symbol_mat[max_fixed_index[0]][max_fixed_index[1]][0];
		f1_float = freq_mat[0][max_float_index[0]]; // detected values for floating point case
		f2_float = freq_mat[1][max_float_index[1]];
		dig_float = symbol_mat[max_float_index[0]][max_float_index[1]][0];



		System_printf("\n\nFixed point:\n");
		System_printf("\n The GTZ for 697 Hz is %d\n", gtz_out[0][0]);
		System_printf("\n The GTZ for 770 Hz is %d\n", gtz_out[0][1]);
		System_printf("\n The GTZ for 852 Hz is %d\n", gtz_out[0][2]);
		System_printf("\n The GTZ for 941 Hz is %d\n", gtz_out[0][3]);
		System_printf("\n The GTZ for 1209 Hz is %d\n", gtz_out[1][0]);
		System_printf("\n The GTZ for 1336 Hz is %d\n", gtz_out[1][1]);
		System_printf("\n The GTZ for 1477 Hz is %d\n", gtz_out[1][2]);
		System_printf("\n The GTZ for 1633 Hz is %d\n", gtz_out[1][3]);
		System_printf("\n\nFloating point:\n");
		System_printf("\n The GTZ for 697 Hz is %f\n", gtz_out_float[0][0]);
		System_printf("\n The GTZ for 770 Hz is %f\n", gtz_out_float[0][1]);
		System_printf("\n The GTZ for 852 Hz is %f\n", gtz_out_float[0][2]);
		System_printf("\n The GTZ for 941 Hz is %f\n", gtz_out_float[0][3]);
		System_printf("\n The GTZ for 1209 Hz is %f\n", gtz_out_float[1][0]);
		System_printf("\n The GTZ for 1336 Hz is %f\n", gtz_out_float[1][1]);
		System_printf("\n The GTZ for 1477 Hz is %f\n", gtz_out_float[1][2]);
		System_printf("\n The GTZ for 1633 Hz is %f\n", gtz_out_float[1][3]);
		System_printf("\n number of cycles for the N=205, fixed: %d\n", tdiff_fixed);
		System_printf("\n number of cycles for the N=205, float: %d\n\n", tdiff_float);
		System_printf("\nThe frequencies detected (using fixed point): %d, %d, thus the button is: %c\n", f1_fixed, f2_fixed, dig_fixed);
		System_printf("\nThe frequencies detected (using floating point): %d, %d, thus the button is: %c\n", f1_float, f2_float, dig_float);
		System_flush();

	}
}
