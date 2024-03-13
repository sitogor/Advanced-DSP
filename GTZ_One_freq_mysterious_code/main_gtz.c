/*
 *  ======== gtz.c ========
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

void clk_SWI_Generate_DTMF(UArg arg0);
void clk_SWI_GTZ_0697Hz(UArg arg0);
void clk_SWI_GTZ_FLOAT_0697Hz(UArg arg0);
void clk_SWI_GTZ(UArg arg0);

extern void task0_dtmfGen(void);
extern void task1_dtmfDetect(void);

extern char digit;
extern int sample, mag1, mag2, freq1, freq2, gtz_out[8];
extern short coef[8];
extern float gtz_out_float[8], coef_float[8], sample_float;



/*
 *  ======== main ========
 */
void main(void)
{


	System_printf("\n I am in main :\n");
	System_flush();
	/* Create a Clock Instance */
    Clock_Params clkParams;

    /* Initialise Clock Instance with time period and timeout (system ticks) */
    Clock_Params_init(&clkParams);
    clkParams.period = 1;
    clkParams.startFlag = TRUE;

    /* Instantiate ISR for tone generation  */
	Clock_create(clk_SWI_Generate_DTMF, TIMEOUT, &clkParams, NULL);

    /* Instantiate 8 parallel ISRs for each of the eight Goertzel coefficients */
	Clock_create(clk_SWI_GTZ_0697Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_FLOAT_0697Hz, TIMEOUT, &clkParams, NULL);


	/* Start SYS_BIOS */
    BIOS_start();
}

/*
 *  ====== clk0Fxn =====
 *  Dual-Tone Generation
 *  ====================
 */
void clk_SWI_Generate_DTMF(UArg arg0)
{
	static int tick;


	tick = Clock_getTicks();
	sample =  mag1*sin(2.0*PI*freq1*TICK_PERIOD*tick) + mag2*sin(2.0*PI*0*TICK_PERIOD*tick); // sampling for floating point
	sample_float = sample;

	sample = (int) mag1*sin(2.0*PI*freq1*TICK_PERIOD*tick) + mag2*sin(2.0*PI*0*TICK_PERIOD*tick); // sampling for fixed point
	sample = sample >>12;




}

/*
 *  ====== clk_SWI_GTZ =====
 *  gtz sweep
 *  ====================
 */
void clk_SWI_GTZ_0697Hz(UArg arg0)
{
   	static unsigned char N = 0;
   	static int Goertzel_Value = 0;

   	static short delay;
   	static short delay_1 = 0;
   	static short delay_2 = 0;

   	int prod1, prod2, prod3;
   	short coef_1 = coef[0]; // for detecting 697 Hz
   	short input;

   	input = (short) sample;



   	prod1 = ((delay_1*coef_1)+(delay_1*coef_1))>>15;
   	delay = input + ((short)prod1) - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;



   	if (N==206)
   	{
   	prod1 = (delay_1 * delay_1);
   	prod2 = (delay_2 * delay_2);
   	prod3 = ((delay_1 * coef_1)+(delay_1 * coef_1))>>15;
   	prod3 = ((short) prod3) * delay_2;
   	Goertzel_Value = (prod1 + prod2 - prod3)>>15;

   	gtz_out[0] = Goertzel_Value<<8;
   	Goertzel_Value = 0;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	}

}

void clk_SWI_GTZ_FLOAT_0697Hz(UArg arg0)
{
	static unsigned char N = 0;
	static float Goertzel_Value = 0;
	static float delay;
	static float delay_1 = 0;
	static float delay_2 = 0;
	float prod1, prod2, prod3;

   	prod1 = delay_1*coef_float[0]; // detectin 697Hz
   	delay = sample_float + prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = delay_1 * delay_1;
   	prod2 = delay_2 * delay_2;
   	prod3 = delay_1 * coef_float[0];
   	prod3 = prod3 * delay_2;
   	Goertzel_Value = prod1 + prod2 - prod3;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out_float[0] = Goertzel_Value/10000000;
   	Goertzel_Value = 0;
   	}
}
