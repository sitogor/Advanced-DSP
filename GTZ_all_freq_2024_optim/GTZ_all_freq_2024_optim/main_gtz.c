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
#include <c6x.h>

void clk_SWI_Read_Data(UArg arg0);
void clk_SWI_GTZ_All_Freq(UArg arg0);

void clk_SWI_GTZ_0697Hz(UArg arg0);
void clk_SWI_GTZ_0770Hz(UArg arg0);
void clk_SWI_GTZ_0852Hz(UArg arg0);
void clk_SWI_GTZ_0941Hz(UArg arg0);
void clk_SWI_GTZ_1209Hz(UArg arg0);
void clk_SWI_GTZ_1336Hz(UArg arg0);
void clk_SWI_GTZ_1477Hz(UArg arg0);
void clk_SWI_GTZ_1633Hz(UArg arg0);

void clk_SWI_GTZ_0697_0777Hz(UArg arg0);
void clk_SWI_GTZ_0852_941Hz(UArg arg0);
void clk_SWI_GTZ_1209_1336Hz(UArg arg0);
void clk_SWI_GTZ_1477_1633Hz(UArg arg0);

extern void task0_dtmfGen(void);
extern void task1_dtmfDetect(void);
extern int mag1, mag2, freq1, freq2;
extern int sample, tdiff, tdiff_final, gtz_out[8],gtz_out_otis[8],tdiff2, tdiff_final2;
extern short coef[8];
extern int flag;
extern short input_samp1,input_samp2;
extern long long prod1_0697_0777;
extern int prod1_0697,prod1_0777;
short data[NO_OF_SAMPLES];
short *buffer;
extern int coeffpair;
extern int prod1f1, prod1f2,prod2f1, prod2f2,prod3f1, prod3f2; // for debugging purposes
extern int prod1660Hz, prod2660Hz,prod3660Hz;

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

	Clock_create(clk_SWI_GTZ_0697Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0770Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0852Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0941Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1209Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1336Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1477Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1633Hz, TIMEOUT, &clkParams, NULL);

	Clock_create(clk_SWI_GTZ_0697_0777Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0852_941Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1209_1336Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1477_1633Hz, TIMEOUT, &clkParams, NULL);

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


	//sample = (int) mag1*sin(2.0*PI*freq1*TICK_PERIOD*tick) + mag2*sin(2.0*PI*freq2*TICK_PERIOD*tick); // sampling for fixed point
	//sample = sample >>12;
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

	// TODO 1. Complete the feedback loop of the GTZ algorithm
	// =========================
	input = (short) sample;
	prod1 = (delay_1*coef[0])>>14;

	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	// =========================
	N++;

	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff =  stop-start;

	if (N == 206) {
		prod1_0697 = prod1;
	   	//Record start time
		start = Timestamp_get32();

		// TODO 2. Complete the feedforward loop of the GTZ algorithm
		 //=========================
		prod1660Hz = prod1 = (delay_1 * delay_1);
		prod2660Hz = prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[0]) >>14;
		prod3660Hz=prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3);
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out_otis[0]= Goertzel_Value;
		Goertzel_Value = 0;
		// gtz_out[..] = ...
		// =========================
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final =  stop-start;
	}
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

	// TODO 1. Complete the feedback loop of the GTZ algorithm
	// =========================
	input = (short) sample;
	prod1 = (delay_1*coef[1])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	// =========================
	N++;

	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff =  stop-start;

	if (N == 206) {
		prod1_0777 = prod1;
	   	//Record start time
		start = Timestamp_get32();

		// TODO 2. Complete the feedforward loop of the GTZ algorithm
		// =========================
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[1]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3);
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out_otis[1]= Goertzel_Value;
		Goertzel_Value = 0;
		 //gtz_out[..] = ...
		 //=========================
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = stop-start;
	}
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

	 //TODO 1. Complete the feedback loop of the GTZ algorithm
	// =========================
	input = (short) sample;
	prod1 = (delay_1*coef[2])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	// =========================
	N++;

	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff =  stop-start;

	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		// TODO 2. Complete the feedforward loop of the GTZ algorithm
		// =========================
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[2]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3);
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out_otis[2]= Goertzel_Value;
		Goertzel_Value = 0;
		// gtz_out[..] = ...
		// =========================
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final =  stop-start;
	}
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

	 //TODO 1. Complete the feedback loop of the GTZ algorithm
	// =========================
	input = (short) sample;
	prod1 = (delay_1*coef[3])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	 //=========================
	N++;

	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff =  stop-start;

	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		// TODO 2. Complete the feedforward loop of the GTZ algorithm
		// =========================
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[3]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3);
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out_otis[3]= Goertzel_Value;
		Goertzel_Value = 0;
		 //gtz_out[..] = ...
		// =========================
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final =  stop-start;
	}
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

	 //TODO 1. Complete the feedback loop of the GTZ algorithm
	// =========================
	input = (short) sample;
	prod1 = (delay_1*coef[4])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	// =========================
	N++;

	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = stop-start;

	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		// TODO 2. Complete the feedforward loop of the GTZ algorithm
		// =========================
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[4]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3);
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out_otis[4]= Goertzel_Value;
		Goertzel_Value = 0;
		// gtz_out[..] = ...
		// =========================
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final =  stop-start;
	}
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

	 //TODO 1. Complete the feedback loop of the GTZ algorithm
	 //=========================
	input = (short) sample;
	prod1 = (delay_1*coef[5])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	// =========================
	N++;

	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff =  stop-start;

	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		// TODO 2. Complete the feedforward loop of the GTZ algorithm
		// =========================
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[5]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3);
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out_otis[5]= Goertzel_Value;
		Goertzel_Value = 0;
		// gtz_out[..] = ...
	//	 =========================
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = stop-start;
	}
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

	 //TODO 1. Complete the feedback loop of the GTZ algorithm
	 //=========================
	input = (short) sample;
	prod1 = (delay_1*coef[6])>>14;
	delay = input + (short)prod1 - delay_2;
	delay_2 = delay_1;
	delay_1 = delay;
	 //=========================
	N++;

	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff = stop-start;

	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		// TODO 2. Complete the feedforward loop of the GTZ algorithm
		 //=========================
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[6]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3);
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out_otis[6]= Goertzel_Value;
		Goertzel_Value = 0;
		 //gtz_out[..] = ...
		 //=========================
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = stop-start;
	}
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

	// TODO 1. Complete the feedback loop of the GTZ algorithm
	// =========================
	input = (short) sample;
	prod1 = _mpy((short)coef[7], delay_1);
	prod1 = _sshvr(prod1, 14);
	delay = _sadd(input,(short)prod1);
	delay = _ssub(delay,delay_2);
	delay_2 = delay_1;
	delay_1 = delay;
	// =========================
	N++;

	//Record stop time
	stop = Timestamp_get32();
	//Record elapsed time
	tdiff =  stop-start;

	if (N == 206) {
	   	//Record start time
		start = Timestamp_get32();

		// TODO 2. Complete the feedforward loop of the GTZ algorithm
		// =========================
		prod1 = (delay_1 * delay_1);
		prod2 = (delay_2 * delay_2);
		prod3 = (delay_1 * coef[7]) >> 14;
		prod3 = ((short) prod3) * delay_2;
		Goertzel_Value = (prod1 + prod2 - prod3);
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out_otis[7]= Goertzel_Value;
		Goertzel_Value = 0;
		// gtz_out[..] = ...
		// =========================
		flag = 1;
		N = 0;

		//Record stop time
		stop = Timestamp_get32();
		//Record elapsed time
		tdiff_final = stop-start;

	}
}

void clk_SWI_GTZ_0697_0777Hz(UArg arg0) {
	// define variables for timing
	static int start2, stop2;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start2 = Timestamp_get32();
	// concatenate coef[0] and coef[1]
	coeffpair = (coef[1] << 16) | (coef[0] & 0xFFFF); // checked
	static int Goertzel_Value2 = 0,Goertzel_Value1 = 0;
	short input = (short) (sample);
	static int inputpair;

	static int delay;
	static int delay_1 = 0;
	static int delay_2 = 0;
    //int prod1f1, prod1f2,prod2f1, prod2f2,prod3f1, prod3f2;
	long long  prod1, prod2, prod3;

	 //TODO 1. Complete the feedback loop of the GTZ algorithm
	 //=========================
	inputpair = (input << 16) | (input & 0xFFFF); // checked
	input_samp1 = inputpair;
	prod1 = _mpy2ll(delay_1,coeffpair); // checked
	//prod1f1 = _mpy(delay_1,coef[0]);
	//prod1f2 = _mpyh(delay_1,coef[1]);
	//prod1 =  (prod1f2 << 32 | prod1f1 & 0xFFFFFFFF);

	prod1 = _dshr(prod1,14); //checked         what is this shift for

	//prod1 = (prod1f1 << 32) | (prod1f2 & 0xFFFFFFFF);
	delay = _add2(inputpair,((short)(uint32_t)(_hill(prod1)) << 16 | ((short)(uint32_t)(_loll(prod1))) & 0xFFFF));

	delay = _sub2(delay,delay_2);

	// =========================

	delay_2 = delay_1;
	delay_1 = delay;
	N++;
	 //Record stop time
	stop2 = Timestamp_get32();
	//Record elapsed time
	tdiff2 = stop2-start2;

	if (N == 206){
		prod1_0697_0777 = prod1;

		//Record start time
		start2 = Timestamp_get32();
		prod1f1 =  _mpy (delay_1, delay_1) ;
		prod1f2 =  _mpyh (delay_1, delay_1);
		prod2f1 =  _mpy (delay_2, delay_2) ;
		prod2f2 =  _mpyh (delay_2, delay_2);
		prod3f1 =  _mpy (delay_1, coeffpair)>>14;
		prod3f2 =  _mpyh (delay_1, coeffpair)>>14;
		prod3f1 =  (short)prod3f1 * (short)(uint32_t)(delay_2 & 0xFFFF);
		prod3f2 =  (short)prod3f2 * (short)(uint32_t)((delay_2 >> 16) & 0xFFFF);

		Goertzel_Value1 = prod1f1 + prod2f1 - prod3f1;
		Goertzel_Value2 = prod1f2 + prod2f2 - prod3f2;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[0]= Goertzel_Value1;
		gtz_out[1]= Goertzel_Value2;
		Goertzel_Value1 = 0;
		Goertzel_Value2 = 0;
	//Record stop time

		stop2 = Timestamp_get32();
		//Record elapsed time
		tdiff_final2 = stop2 - start2;
		flag = 1;
		N = 0;
	}
}

/*void clk_SWI_GTZ_0697_0777Hz(UArg arg0) {
	// define variables for timing
	static int start2, stop2;
	start2 = Timestamp_get32();
	// define feedback times as N
	static int N = 0;
	short input = (short) (sample);
	__x128_t  inputarray =  _ito128((int)input,(int)input,(int)input,(int)input);
	__x128_t  coefarray =  _ito128((int)coef[3]>>14,(int)coef[2]>>14,(int)coef[1]>>14,(int)coef[0]>>14);
	static __x128_t  delay;
	static __x128_t  delay_1 = 0;

	static __x128_t  delay_2 = 0;
	__x128_t prod1,prod2,prod3;
	prod1 = _qmpy32(delay_1,coefarray);
	prod1 = _ito128(_get32_128(prod1, 3)>>14,_get32_128(prod1, 2)>>14,_get32_128(prod1, 1)>>14,_get32_128(prod1, 0)>>14);
	delay = _ito128((int)input + _get32_128(prod1, 3) - _get32_128(delay_2, 3),(int)input + _get32_128(prod1, 2) - _get32_128(delay_2, 2),(int)input + _get32_128(prod1, 1) - _get32_128(delay_2, 1),(int)input + _get32_128(prod1, 0) - _get32_128(delay_2, 0));
	delay_2 = delay_1;
	delay_1 = delay;

	 //Record stop time
	N++;
	stop2 = Timestamp_get32();
	//Record elapsed time
	tdiff2 = stop2-start2;
	if (N == 206){
		prod1_0697_0777 = _get32_128(prod1,0);
		//Record start time
		start2 = Timestamp_get32();


		stop2 = Timestamp_get32();
		//Record elapsed time
		tdiff_final2 = stop2 - start2;
		flag = 1;
		N = 0;
		}
}*/

void clk_SWI_GTZ_0852_941Hz(UArg arg0){
	// define variables for timing
	static int start2, stop2;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start2 = Timestamp_get32();
	// concatenate coef[0] and coef[1]
	coeffpair = (coef[3] << 16) | (coef[2] & 0xFFFF); // checked
	static int Goertzel_Value2 = 0,Goertzel_Value1 = 0;
	short input = (short) (sample);
	static int inputpair;

	static int delay;
	static int delay_1 = 0;
	static int delay_2 = 0;
    //int prod1f1, prod1f2,prod2f1, prod2f2,prod3f1, prod3f2;
	long long  prod1, prod2, prod3;

	 //TODO 1. Complete the feedback loop of the GTZ algorithm
	 //=========================
	inputpair = (input << 16) | (input & 0xFFFF); // checked
	input_samp2 = inputpair;
	prod1 = _mpy2ll(delay_1,coeffpair); // checked
	//prod1f1 = _mpy(delay_1,coef[0]);
	//prod1f2 = _mpyh(delay_1,coef[1]);
	//prod1 =  (prod1f2 << 32 | prod1f1 & 0xFFFFFFFF);

	prod1 = _dshr(prod1,14); //checked

	//prod1 = (prod1f1 << 32) | (prod1f2 & 0xFFFFFFFF);
	delay = _add2(inputpair,((short)(uint32_t)(_hill(prod1)) << 16 | ((short)(uint32_t)(_loll(prod1))) & 0xFFFF));

	delay = _sub2(delay,delay_2);

	// =========================

	delay_2 = delay_1;
	delay_1 = delay;
	N++;
	 //Record stop time
	stop2 = Timestamp_get32();
	//Record elapsed time
	tdiff2 = stop2-start2;

	if (N == 206){
		prod1_0697_0777 = prod1;

		//Record start time
		start2 = Timestamp_get32();
		prod1f1 =  _mpy (delay_1, delay_1) ;
		prod1f2 =  _mpyh (delay_1, delay_1);
		prod2f1 =  _mpy (delay_2, delay_2) ;
		prod2f2 =  _mpyh (delay_2, delay_2);
		prod3f1 =  _mpy (delay_1, coeffpair)>>14;
		prod3f2 =  _mpyh (delay_1, coeffpair)>>14;
		prod3f1 =  (short)prod3f1 * (short)(uint32_t)(delay_2 & 0xFFFF);
		prod3f2 =  (short)prod3f2 * (short)(uint32_t)((delay_2 >> 16) & 0xFFFF);

		Goertzel_Value1 = prod1f1 + prod2f1 - prod3f1;
		Goertzel_Value2 = prod1f2 + prod2f2 - prod3f2;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[2]= Goertzel_Value1;
		gtz_out[3]= Goertzel_Value2;
		Goertzel_Value1 = 0;
		Goertzel_Value2 = 0;
	//Record stop time

		stop2 = Timestamp_get32();
		//Record elapsed time
		tdiff_final2 = stop2 - start2;
		flag = 1;
		N = 0;
	}


}



void clk_SWI_GTZ_1209_1336Hz(UArg arg0){
	// define variables for timing
	static int start2, stop2;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start2 = Timestamp_get32();
	// concatenate coef[0] and coef[1]
	coeffpair = (coef[5] << 16) | (coef[4] & 0xFFFF); // checked
	static int Goertzel_Value2 = 0,Goertzel_Value1 = 0;
	short input = (short) (sample);
	static int inputpair;

	static int delay;
	static int delay_1 = 0;
	static int delay_2 = 0;
    //int prod1f1, prod1f2,prod2f1, prod2f2,prod3f1, prod3f2;
	long long  prod1, prod2, prod3;

	 //TODO 1. Complete the feedback loop of the GTZ algorithm
	 //=========================
	inputpair = (input << 16) | (input & 0xFFFF); // checked
	prod1 = _mpy2ll(delay_1,coeffpair); // checked
	//prod1f1 = _mpy(delay_1,coef[0]);
	//prod1f2 = _mpyh(delay_1,coef[1]);
	//prod1 =  (prod1f2 << 32 | prod1f1 & 0xFFFFFFFF);

	prod1 = _dshr(prod1,14); //checked

	//prod1 = (prod1f1 << 32) | (prod1f2 & 0xFFFFFFFF);
	delay = _add2(inputpair,((short)(uint32_t)(_hill(prod1)) << 16 | ((short)(uint32_t)(_loll(prod1))) & 0xFFFF));

	delay = _sub2(delay,delay_2);

	// =========================

	delay_2 = delay_1;
	delay_1 = delay;
	N++;
	 //Record stop time
	stop2 = Timestamp_get32();
	//Record elapsed time
	tdiff2 = stop2-start2;

	if (N == 206){
		prod1_0697_0777 = prod1;

		//Record start time
		start2 = Timestamp_get32();
		prod1f1 =  _mpy (delay_1, delay_1) ;
		prod1f2 =  _mpyh (delay_1, delay_1);
		prod2f1 =  _mpy (delay_2, delay_2) ;
		prod2f2 =  _mpyh (delay_2, delay_2);
		prod3f1 =  _mpy (delay_1, coeffpair)>>14;
		prod3f2 =  _mpyh (delay_1, coeffpair)>>14;
		prod3f1 =  (short)prod3f1 * (short)(uint32_t)(delay_2 & 0xFFFF);
		prod3f2 =  (short)prod3f2 * (short)(uint32_t)((delay_2 >> 16) & 0xFFFF);

		Goertzel_Value1 = prod1f1 + prod2f1 - prod3f1;
		Goertzel_Value2 = prod1f2 + prod2f2 - prod3f2;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[4]= Goertzel_Value1;
		gtz_out[5]= Goertzel_Value2;
		Goertzel_Value1 = 0;
		Goertzel_Value2 = 0;
	//Record stop time

		stop2 = Timestamp_get32();
		//Record elapsed time
		tdiff_final2 = stop2 - start2;
		flag = 1;
		N = 0;
	}

}


void clk_SWI_GTZ_1477_1633Hz(UArg arg0){
	// define variables for timing
	static int start2, stop2;

	// define feedback times as N
	static int N = 0;
   	//Record start time
	start2 = Timestamp_get32();
	// concatenate coef[0] and coef[1]
	coeffpair = (coef[7] << 16) | (coef[6] & 0xFFFF); // checked
	static int Goertzel_Value2 = 0,Goertzel_Value1 = 0;
	short input = (short) (sample);
	static int inputpair;

	static int delay;
	static int delay_1 = 0;
	static int delay_2 = 0;
    //int prod1f1, prod1f2,prod2f1, prod2f2,prod3f1, prod3f2;
	long long  prod1, prod2, prod3;

	 //TODO 1. Complete the feedback loop of the GTZ algorithm
	 //=========================
	inputpair = (input << 16) | (input & 0xFFFF); // checked
	prod1 = _mpy2ll(delay_1,coeffpair); // checked
	//prod1f1 = _mpy(delay_1,coef[0]);
	//prod1f2 = _mpyh(delay_1,coef[1]);
	//prod1 =  (prod1f2 << 32 | prod1f1 & 0xFFFFFFFF);

	prod1 = _dshr(prod1,14); //checked

	//prod1 = (prod1f1 << 32) | (prod1f2 & 0xFFFFFFFF);
	delay = _add2(inputpair,((short)(uint32_t)(_hill(prod1)) << 16 | ((short)(uint32_t)(_loll(prod1))) & 0xFFFF));

	delay = _sub2(delay,delay_2);

	// =========================

	delay_2 = delay_1;
	delay_1 = delay;
	N++;
	 //Record stop time
	stop2 = Timestamp_get32();
	//Record elapsed time
	tdiff2 = stop2-start2;

	if (N == 206){
		prod1_0697_0777 = prod1;

		//Record start time
		start2 = Timestamp_get32();
		prod1f1 =  _mpy (delay_1, delay_1) ;
		prod1f2 =  _mpyh (delay_1, delay_1);
		prod2f1 =  _mpy (delay_2, delay_2) ;
		prod2f2 =  _mpyh (delay_2, delay_2);
		prod3f1 =  _mpy (delay_1, coeffpair)>>14;
		prod3f2 =  _mpyh (delay_1, coeffpair)>>14;
		prod3f1 =  (short)prod3f1 * (short)(uint32_t)(delay_2 & 0xFFFF);
		prod3f2 =  (short)prod3f2 * (short)(uint32_t)((delay_2 >> 16) & 0xFFFF);

		Goertzel_Value1 = prod1f1 + prod2f1 - prod3f1;
		Goertzel_Value2 = prod1f2 + prod2f2 - prod3f2;
		N = 0;
		delay_1 = delay_2 = 0;
		gtz_out[6]= Goertzel_Value1;
		gtz_out[7]= Goertzel_Value2;
		Goertzel_Value1 = 0;
		Goertzel_Value2 = 0;
	//Record stop time

		stop2 = Timestamp_get32();
		//Record elapsed time
		tdiff_final2 = stop2 - start2;
		flag = 1;
		N = 0;
	}
}
