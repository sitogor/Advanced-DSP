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

int tdiff,tdiff_final;

int sample, gtz_out[8];
int flag = 0;

short coef[8] =
		{ 0x6D02, 0x68AD, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x3290, 0x23CE }; // goertzel coefficients
void task1_dtmfDetect();
void task2_dtmfGenerate(char* keys);
extern short* buffer;

int freq_mat[2][4] = {{697, 770, 852, 941}, {1209, 1336, 1477, 1633}};

void task1_dtmfDetect() {
	int i, n;
	int max_fixed_index[2];
	int max_fixed0, max_fixed1;


	char pad[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

	char result[8];

	for(n=0;n<8;n++) {
		while (!flag) Task_sleep(210);
		// detects the highest frequencies

		// set the initial maxima to the first elements
		max_fixed0 = gtz_out[0];
		max_fixed1 = gtz_out[4];
		max_fixed_index[0] = 0; max_fixed_index[1] = 0;
		for (i=1; i<=3; i++)
				{
					//fixed point freq from 697 to 941
					if(gtz_out[i]>max_fixed0)
					{
						max_fixed0 = gtz_out[i];
						max_fixed_index[0] = i;
					}

					//fixed point freq from 1209 to 1633
					if(gtz_out[i+4]>max_fixed1)
					{

						max_fixed1 = gtz_out[i+4];
						max_fixed_index[1] = i;
					}
				}


		//f1_fixed = freq_mat[0][max_fixed_index[0]]; //detected values for fixed point case
		//f2_fixed = freq_mat[1][max_fixed_index[1]];
		result[n] = pad[max_fixed_index[0]][max_fixed_index[1]];
		printf("\n The decoded value is: %c \n", result[n]);
		printf("\n pad index pairs: (%d,%d) \n",max_fixed_index[0],max_fixed_index[1]);

		/* TODO 3. Complete code to detect the 8 digits based on the GTZ output */
		/* ========================= */

		// ideal pad value: (0,0), (0,1), (0,2), (1,0), (1,1), (1,2), (2,0), (2,1)
		flag = 0;
	}

	printf("\nDetection finished\n");


	printf("Finished\n");
}
