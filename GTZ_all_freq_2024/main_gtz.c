/*
 *  ======== gtz.c ========
 */


/* type your names, emails and group number here */

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

void clk_SWI_Read_Data(UArg arg0);
//void clk_SWI_GTZ_All_Freq(UArg arg0);

void clk_SWI_GTZ_0697Hz(UArg arg0);
void clk_SWI_GTZ_0770Hz(UArg arg0);
void clk_SWI_GTZ_0852Hz(UArg arg0);
void clk_SWI_GTZ_0941Hz(UArg arg0);
void clk_SWI_GTZ_1209Hz(UArg arg0);
void clk_SWI_GTZ_1336Hz(UArg arg0);
void clk_SWI_GTZ_1477Hz(UArg arg0);
void clk_SWI_GTZ_1633Hz(UArg arg0);


extern void task0_dtmfGen(void);
extern void task1_dtmfDetect(void);
extern int mag1, mag2, freq1, freq2;
extern int sample, tdiff, tdiff_final, gtz_out[8];
extern short coef[8];
extern int flag;

short data[NO_OF_SAMPLES];
short *buffer;

/*
 *  ======== main ========
 */



int main() {
	System_printf("\n System Start\n");
	System_flush();

	/* Read binary data file */
	FILE* fp = fopen("../data.bin", "rb");
	if(fp==0) {
		System_printf("Error: Data file not found\n");
		System_flush();
		return 1;
	}
	fread(data, 2, NO_OF_SAMPLES, fp);
	buffer = (short*)malloc(2*8*10000);


	/* Create a Clock Instance */
	Clock_Params clkParams;

	/* Initialise Clock Instance with time period and timeout (system ticks) */
	Clock_Params_init(&clkParams);
	clkParams.period = 1;
	clkParams.startFlag = TRUE;

	/* Instantiate ISR for tone generation  */
	Clock_create(clk_SWI_Read_Data, TIMEOUT, &clkParams, NULL);

	/* Instantiate 8 parallel ISRs for each of the eight Goertzel coefficients */
	//Clock_create(clk_SWI_GTZ_All_Freq, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0697Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0770Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0852Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0941Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1209Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1336Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1477Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1633Hz, TIMEOUT, &clkParams, NULL);
	/* Start SYS_BIOS */
	BIOS_start();
}

/*
 *  ====== clk_SWI_Generate_DTMF =====
 *  Dual-Tone Generation
 *  ==================================
 */
void clk_SWI_Read_Data(UArg arg0) {
	static int tick;
	tick = Clock_getTicks();
	sample = data[tick%NO_OF_SAMPLES];
//	sample = (int) mag1*sin(2.0*PI*freq1*TICK_PERIOD*tick) + (int) mag2*sin(2.0*PI*freq2*TICK_PERIOD*tick); // sampling for fixed point
//	sample = sample >>12;
}

/*
 *  ====== clk_SWI_GTZ =====
 *  gtz sweep
 *  ========================
 */
void clk_SWI_GTZ_0697Hz(UArg arg0) {
	// define variables for timing
	static int start, stop;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start = Timestamp_get32();

	static int Goertzel_Value = 0;
	short input = (short) (sample);

	static short delay;
	static short delay_1 = 0;
	static short delay_2 = 0;
	int prod1, prod2, prod3;

	/* TODO 1. Complete the feedback loop of the GTZ algorithm*/
	/* ========================= */
	input = (short) sample;
	prod1 = (delay_1*coef[0])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	/* ========================= */
	N++;



	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		/* TODO 2. Complete the feedforward loop of the GTZ algorithm*/
		/* ========================= */
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[0]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3)>>7;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[0]= Goertzel_Value;
		Goertzel_Value = 0;
		/* gtz_out[..] = ... */
		/* ========================= */
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = tdiff + stop-start;
	}
	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = tdiff + stop-start;
}

void clk_SWI_GTZ_0770Hz(UArg arg0) {
	// define variables for timing
	static int start, stop;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start = Timestamp_get32();

	static int Goertzel_Value = 0;
	short input = (short) (sample);

	static short delay;
	static short delay_1 = 0;
	static short delay_2 = 0;
	int prod1, prod2, prod3;

	/* TODO 1. Complete the feedback loop of the GTZ algorithm*/
	/* ========================= */
	input = (short) sample;
	prod1 = (delay_1*coef[1])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	/* ========================= */
	N++;



	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		/* TODO 2. Complete the feedforward loop of the GTZ algorithm*/
		/* ========================= */
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[1]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3)>>7;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[1]= Goertzel_Value;
		Goertzel_Value = 0;
		/* gtz_out[..] = ... */
		/* ========================= */
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = tdiff + stop - start;
	}
	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = tdiff + stop - start;
}

void clk_SWI_GTZ_0852Hz(UArg arg0) {
	// define variables for timing
	static int start, stop;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start = Timestamp_get32();

	static int Goertzel_Value = 0;
	short input = (short) (sample);

	static short delay;
	static short delay_1 = 0;
	static short delay_2 = 0;
	int prod1, prod2, prod3;

	/* TODO 1. Complete the feedback loop of the GTZ algorithm*/
	/* ========================= */
	input = (short) sample;
	prod1 = (delay_1*coef[2])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	/* ========================= */
	N++;

	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		/* TODO 2. Complete the feedforward loop of the GTZ algorithm*/
		/* ========================= */
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[2]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3)>>7;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[2]= Goertzel_Value;
		Goertzel_Value = 0;
		/* gtz_out[..] = ... */
		/* ========================= */
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = tdiff + stop - start;
	}
	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = tdiff + stop - start;
}

void clk_SWI_GTZ_0941Hz(UArg arg0) {
	// define variables for timing
	static int start, stop;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start = Timestamp_get32();

	static int Goertzel_Value = 0;
	short input = (short) (sample);

	static short delay;
	static short delay_1 = 0;
	static short delay_2 = 0;
	int prod1, prod2, prod3;

	/* TODO 1. Complete the feedback loop of the GTZ algorithm*/
	/* ========================= */
	input = (short) sample;
	prod1 = (delay_1*coef[3])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	/* ========================= */
	N++;



	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		/* TODO 2. Complete the feedforward loop of the GTZ algorithm*/
		/* ========================= */
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[3]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3)>>7;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[3]= Goertzel_Value;
		Goertzel_Value = 0;
		/* gtz_out[..] = ... */
		/* ========================= */
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = tdiff + stop - start;
	}
	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = tdiff + stop - start;
}

void clk_SWI_GTZ_1209Hz(UArg arg0) {
	// define variables for timing
	static int start, stop;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start = Timestamp_get32();

	static int Goertzel_Value = 0;
	short input = (short) (sample);

	static short delay;
	static short delay_1 = 0;
	static short delay_2 = 0;
	int prod1, prod2, prod3;

	/* TODO 1. Complete the feedback loop of the GTZ algorithm*/
	/* ========================= */
	input = (short) sample;
	prod1 = (delay_1*coef[4])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	/* ========================= */
	N++;



	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		/* TODO 2. Complete the feedforward loop of the GTZ algorithm*/
		/* ========================= */
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[4]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3)>>7;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[4]= Goertzel_Value;
		Goertzel_Value = 0;
		/* gtz_out[..] = ... */
		/* ========================= */
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = tdiff + stop - start;
	}
	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = tdiff + stop - start;
}

void clk_SWI_GTZ_1336Hz(UArg arg0) {
	// define variables for timing
	static int start, stop;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start = Timestamp_get32();

	static int Goertzel_Value = 0;
	short input = (short) (sample);

	static short delay;
	static short delay_1 = 0;
	static short delay_2 = 0;
	int prod1, prod2, prod3;

	/* TODO 1. Complete the feedback loop of the GTZ algorithm*/
	/* ========================= */
	input = (short) sample;
	prod1 = (delay_1*coef[5])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	/* ========================= */
	N++;


	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		/* TODO 2. Complete the feedforward loop of the GTZ algorithm*/
		/* ========================= */
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[5]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3)>>7;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[5]= Goertzel_Value;
		Goertzel_Value = 0;
		/* gtz_out[..] = ... */
		/* ========================= */
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = tdiff + stop - start;
	}
	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = tdiff + stop - start;
}

void clk_SWI_GTZ_1477Hz(UArg arg0) {
	// define variables for timing
	static int start, stop;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start = Timestamp_get32();

	static int Goertzel_Value = 0;
	short input = (short) (sample);

	static short delay;
	static short delay_1 = 0;
	static short delay_2 = 0;
	int prod1, prod2, prod3;

	/* TODO 1. Complete the feedback loop of the GTZ algorithm*/
	/* ========================= */
	input = (short) sample;
	prod1 = (delay_1*coef[6])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	/* ========================= */
	N++;


	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		/* TODO 2. Complete the feedforward loop of the GTZ algorithm*/
		/* ========================= */
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[6]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3)>>7;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[6]= Goertzel_Value;
		Goertzel_Value = 0;
		/* gtz_out[..] = ... */
		/* ========================= */
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = tdiff + stop - start;
	}
	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = tdiff + stop - start;
}

void clk_SWI_GTZ_1633Hz(UArg arg0) {
	// define variables for timing
	static int start, stop;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start = Timestamp_get32();

	static int Goertzel_Value = 0;
	short input = (short) (sample);

	static short delay;
	static short delay_1 = 0;
	static short delay_2 = 0;
	int prod1, prod2, prod3;

	/* TODO 1. Complete the feedback loop of the GTZ algorithm*/
	/* ========================= */
	input = (short) sample;
	prod1 = (delay_1*coef[7])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	/* ========================= */
	N++;

	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		/* TODO 2. Complete the feedforward loop of the GTZ algorithm*/
		/* ========================= */
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[7]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3)>>7;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[7]= Goertzel_Value;
		Goertzel_Value = 0;
		/* gtz_out[..] = ... */
		/* ========================= */
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = tdiff + stop - start;
	}
	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = tdiff + stop - start;
}
