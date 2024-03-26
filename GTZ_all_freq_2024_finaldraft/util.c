/*
 *  ======== util.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
//#pragma DATA_ALIGN(prod, 16);
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <xdc/runtime/Types.h>
#include <xdc/runtime/Timestamp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"
int start_2,stop_2, diff;
int freq1, freq2;
int mag1, mag2;
int tdiff,tdiff_final,tdiff2,tdiff_final2;
long long prod1_0697_0777;
int prod1_0697,prod1_0777;
int sample, gtz_out[8];
int flag = 0;

int prod1f1, prod1f2,prod2f1, prod2f2,prod3f1, prod3f2; // for debugging purposes move
int prod1660Hz, prod2660Hz,prod3660Hz;

short coef[8] =
		{ 0x6D02, 0x68AD, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x3290, 0x23CE}; // goertzel coefficients
void task1_dtmfDetect();
void task2_dtmfGenerate(char* keys);
extern short* buffer;
int coeffpair;

int freq_mat[2][4] = {{697, 770, 852, 941}, {1209, 1336, 1477, 1633}};
double sample64;
void task1_dtmfDetect() {
	int i, n, j;
	int max_fixed_index[2];
	int max_fixed0, max_fixed1;
/*
	// testing the functionality of the mpy2 is it dotp or split results to pair of 32 bit
	short a=24, b=-23, c= 8, d = 2;
	int a_b, c_d;
	long long prod;
	int prodshorted;

	//_nassert((int) prod % 8 == 0);
	a_b = a * b;
	c_d = c*d;
	prod = _mpy2ll(0x00180008,0xffe90002);
	a_b product fffffdd8   c_d short  10

	a short 18 b short ffe9 c short  8 d short 2

	mpy2ll result fffffdd800000010

	// testing the concatanation of input pair, concatanating p
	int pair;
	pair = (b << 16) | (b & 0xFFFF);
	printf("\n a_b product %x   c_d short  %x  \n", a_b,c_d);
	printf("\n a short %hx b short %hx c short  %hx d short %hx\n", a,b,c,d);
	printf("\n  mpy2ll result %llx\n", prod);
	//printf("\n pair of b %x\n", pair);

	// testing dshr shifting >>14
	prod = _mpy2ll(0x00180008,0x68AD6D02);
	a_b = (a * 0x68AD)>>14;
	c_d = (c*0x6D02)>>14;
	prod = _dshr(prod,14);
	printf("\n a_b product shifted by 14 to right %x   c_d product shifted by 14 to right  %x  \n", a_b,c_d);
	printf("\n mpy2ll applied and shifting  %.16llx  \n",prod);
	 a_b product shifted by 14 to right 27   c_d product shifted by 14 to right  d

 mpy2ll applied and shifting  000000270000000d
	prod = 0xfffffdd800000010;

	prodshorted = (((short)_hi(prod))<< 16  |  ((short)_hi(prod))& 0xFFFF );
	//printf("\n  prod with 32 pair  %.16llx\n", prod);
	//printf("\n MSB 32 bit  %x LSB 32 bit %x\n", _hill(prod),_loll(prod));
	// testing shorting prod every 32 pair to give 16 short pairs in case of negative and positive numbers

	//printf("\n coefficient pair %x\n", coeffpair); // testing coefficient pair worked successfully
*/
	char pad[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

	char result[8];

	start_2 = Timestamp_get32();

	for(n=0;n<8;n++) {
		while (!flag) Task_sleep(210);

		printf("\n number of clock cycles in 1 for loop iteration (optimized): %d  \n", tdiff2);
		printf("\n including feedforward loop:%d  \n", tdiff_final2);

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

		result[n] = pad[max_fixed_index[0]][max_fixed_index[1]];

		printf("The decoded value is: %c \n", result[n]);
		for (j = 0; j<8 ; j++){
			printf("%d \n", gtz_out[j]);
		}
		printf("pad index pairs: (%d,%d) \n",max_fixed_index[0],max_fixed_index[1]);


		/* TODO 3. Complete code to detect the 8 digits based on the GTZ output */
		/* ========================= */
		// ideal character detected: 1, 2, 3, 4, 5, 6, 7, 8
		// ideal pad index pairs: (0,0), (0,1), (0,2), (1,0), (1,1), (1,2), (2,0), (2,1)
		flag = 0;
	}
	stop_2 = Timestamp_get32();
	diff = stop_2 -  start_2;

	printf("%d Total clock cycles elapsed: \n", diff);

	printf("Detection finished\n");


	printf("Finished\n");
}
