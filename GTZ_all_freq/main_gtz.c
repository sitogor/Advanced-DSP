/*
 *  ======== gtz.c ========
 */    

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

/* implement using short*/

void clk_SWI_Generate_DTMF(UArg arg0);
void clk_SWI_GTZ_0697Hz(UArg arg0);
void clk_SWI_GTZ_0770Hz(UArg arg0);
void clk_SWI_GTZ_0852Hz(UArg arg0);
void clk_SWI_GTZ_0941Hz(UArg arg0);
void clk_SWI_GTZ_1209Hz(UArg arg0);
void clk_SWI_GTZ_1336Hz(UArg arg0);
void clk_SWI_GTZ_1477Hz(UArg arg0);
void clk_SWI_GTZ_1633Hz(UArg arg0);

/* Implement using floating point*/
void clk_SWI_GTZ_FLOAT_0697Hz(UArg arg0);
void clk_SWI_GTZ_FLOAT_0770Hz(UArg arg0);
void clk_SWI_GTZ_FLOAT_0852Hz(UArg arg0);
void clk_SWI_GTZ_FLOAT_0941Hz(UArg arg0);
void clk_SWI_GTZ_FLOAT_1209Hz(UArg arg0);
void clk_SWI_GTZ_FLOAT_1336Hz(UArg arg0);
void clk_SWI_GTZ_FLOAT_1477Hz(UArg arg0);
void clk_SWI_GTZ_FLOAT_1633Hz(UArg arg0);


extern void task0_dtmfGen(void);
extern void task1_dtmfDetect(void);

extern char digit;
extern int sample, mag1, mag2, freq1, freq2, gtz_out[2][4];
extern short coef[8];
extern float gtz_out_float[2][4], coef_float[8], sample_float;
extern int tdiff_fixed, start, tdiff_float;


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
	Clock_create(clk_SWI_GTZ_0770Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0852Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_0941Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1209Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1336Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1477Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_1633Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_FLOAT_0697Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_FLOAT_0770Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_FLOAT_0852Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_FLOAT_0941Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_FLOAT_1209Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_FLOAT_1336Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_FLOAT_1477Hz, TIMEOUT, &clkParams, NULL);
	Clock_create(clk_SWI_GTZ_FLOAT_1633Hz, TIMEOUT, &clkParams, NULL);




	/* Start SYS_BIOS */
    BIOS_start();
    start = Timestamp_get32();
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
	sample =  mag1*sin(2.0*PI*freq1*TICK_PERIOD*tick) + mag2*sin(2.0*PI*freq2*TICK_PERIOD*tick);
	sample_float = sample;
	sample = (int) mag1*sin(2.0*PI*freq1*TICK_PERIOD*tick) + mag2*sin(2.0*PI*freq2*TICK_PERIOD*tick);
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
   	short input;

   	input = (short) sample;
   	prod1 = (delay_1*coef[0])>>14;
   	delay = input + (short)prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = (delay_1 * delay_1);
   	prod2 = (delay_2 * delay_2);
   	prod3 = (delay_1 * coef[0]) >> 14;
   	prod3 = ((short) prod3) * delay_2;
   	Goertzel_Value = (prod1 + prod2 - prod3)>>7;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out[0][0] = Goertzel_Value;
   	Goertzel_Value = 0;
   	tdiff_fixed = Timestamp_get32() - start;
   	}
}

void clk_SWI_GTZ_0770Hz(UArg arg0)
{
   	static unsigned char N = 0;
   	static int Goertzel_Value = 0;
   	static short delay;
   	static short delay_1 = 0;
   	static short delay_2 = 0;
   	int prod1, prod2, prod3;
   	short input;

   	input = (short) sample;
   	prod1 = (delay_1*coef[1])>>14;
   	delay = input + (short)prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = (delay_1 * delay_1);
   	prod2 = (delay_2 * delay_2);
   	prod3 = (delay_1 * coef[1]) >> 14;
   	prod3 = ((short) prod3) * delay_2;
   	Goertzel_Value = (prod1 + prod2 - prod3)>>7;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out[0][1] = Goertzel_Value;
   	Goertzel_Value = 0;
   	}
}



void clk_SWI_GTZ_0852Hz(UArg arg0)
{
   	static unsigned char N = 0;
   	static int Goertzel_Value = 0;
   	static short delay;
   	static short delay_1 = 0;
   	static short delay_2 = 0;
   	int prod1, prod2, prod3;
   	short input;

   	input = (short) sample;
   	prod1 = (delay_1*coef[2])>>14;
   	delay = input + (short)prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = (delay_1 * delay_1);
   	prod2 = (delay_2 * delay_2);
   	prod3 = (delay_1 * coef[2]) >> 14;
   	prod3 = ((short) prod3) * delay_2;
   	Goertzel_Value = (prod1 + prod2 - prod3)>>7;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out[0][2] = Goertzel_Value;
   	Goertzel_Value = 0;
   	}
}


void clk_SWI_GTZ_0941Hz(UArg arg0)
{
   	static unsigned char N = 0;
   	static int Goertzel_Value = 0;
   	static short delay;
   	static short delay_1 = 0;
   	static short delay_2 = 0;
   	int prod1, prod2, prod3;
   	short input;

   	input = (short) sample;
   	input = input; // Scale down input to prevent overflow
   	prod1 = (delay_1*coef[3])>>14;
   	delay = input + (short)prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = (delay_1 * delay_1);
   	prod2 = (delay_2 * delay_2);
   	prod3 = (delay_1 * coef[3]) >> 14;
   	prod3 = ((short) prod3) * delay_2;
   	Goertzel_Value = (prod1 + prod2 - prod3)>>7;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out[0][3] = Goertzel_Value;
   	Goertzel_Value = 0;
   	}

}



void clk_SWI_GTZ_1209Hz(UArg arg0)
{
   	static unsigned char N = 0;
   	static int Goertzel_Value = 0;
   	static short delay;
   	static short delay_1 = 0;
   	static short delay_2 = 0;
   	int prod1, prod2, prod3;
   	short input;

   	input = (short) sample;
   	prod1 = (delay_1*coef[4])>>14;
   	delay = input + (short)prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = (delay_1 * delay_1);
   	prod2 = (delay_2 * delay_2);
   	prod3 = (delay_1 * coef[4]) >> 14;
   	prod3 = ((short) prod3) * delay_2;
   	Goertzel_Value = (prod1 + prod2 - prod3)>>7;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out[1][0] = Goertzel_Value;
   	Goertzel_Value = 0;
   	}

}


void clk_SWI_GTZ_1336Hz(UArg arg0)
{
   	static unsigned char N = 0;
   	static int Goertzel_Value = 0;
   	static short delay;
   	static short delay_1 = 0;
   	static short delay_2 = 0;
   	int prod1, prod2, prod3;
   	short input;

   	input = (short) sample;
   	prod1 = (delay_1*coef[5])>>14;
   	delay = input + (short)prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = (delay_1 * delay_1);
   	prod2 = (delay_2 * delay_2);
   	prod3 = (delay_1 * coef[5]) >> 14;
   	prod3 = ((short) prod3) * delay_2;
   	Goertzel_Value = (prod1 + prod2 - prod3)>>7;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out[1][1] = Goertzel_Value;
   	Goertzel_Value = 0;
   	}
}

void clk_SWI_GTZ_1477Hz(UArg arg0)
{
   	static unsigned char N = 0;
   	static int Goertzel_Value = 0;
   	static short delay;
   	static short delay_1 = 0;
   	static short delay_2 = 0;
   	int prod1, prod2, prod3;
   	short input;

   	input = (short) sample;
   	input = input;
   	prod1 = (delay_1*coef[6])>>14;
   	delay = input + (short)prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = (delay_1 * delay_1);
   	prod2 = (delay_2 * delay_2);
   	prod3 = (delay_1 * coef[6]) >> 14;
   	prod3 = ((short) prod3) * delay_2;
   	Goertzel_Value = (prod1 + prod2 - prod3)>>7;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out[1][2] = Goertzel_Value;
   	Goertzel_Value = 0;
   	}
}


void clk_SWI_GTZ_1633Hz(UArg arg0)
{
   	static unsigned char N = 0;
   	static int Goertzel_Value = 0;
   	static short delay;
   	static short delay_1 = 0;
   	static short delay_2 = 0;
   	int prod1, prod2, prod3;
   	short input;

   	input = (short) sample;
   	prod1 = (delay_1*coef[7])>>14;
   	delay = input + (short)prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = (delay_1 * delay_1);
   	prod2 = (delay_2 * delay_2);
   	prod3 = (delay_1 * coef[7]) >> 14;
   	prod3 = ((short) prod3) * delay_2;
   	Goertzel_Value = (prod1 + prod2 - prod3)>>7;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out[1][3] = Goertzel_Value;
   	Goertzel_Value = 0;
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

   	prod1 = delay_1*coef_float[0];
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
   	gtz_out_float[0][0] = Goertzel_Value/10000000000;
   	Goertzel_Value = 0;
   	tdiff_float = Timestamp_get32() - start;
   	}
}

void clk_SWI_GTZ_FLOAT_0770Hz(UArg arg0)
{
	static unsigned char N = 0;
	static float Goertzel_Value = 0;
	static float delay;
	static float delay_1 = 0;
	static float delay_2 = 0;
	float prod1, prod2, prod3;

   	prod1 = delay_1*coef_float[1];
   	delay = sample_float + prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = delay_1 * delay_1;
   	prod2 = delay_2 * delay_2;
   	prod3 = delay_1 * coef_float[1];
   	prod3 = prod3 * delay_2;
   	Goertzel_Value = prod1 + prod2 - prod3;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out_float[0][1] = Goertzel_Value/10000000000;
   	Goertzel_Value = 0;
   	}
}


void clk_SWI_GTZ_FLOAT_0852Hz(UArg arg0)
{
	static unsigned char N = 0;
	static float Goertzel_Value = 0;
	static float delay;
	static float delay_1 = 0;
	static float delay_2 = 0;
	float prod1, prod2, prod3;

   	prod1 = delay_1*coef_float[2];
   	delay = sample_float + prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = delay_1 * delay_1;
   	prod2 = delay_2 * delay_2;
   	prod3 = delay_1 * coef_float[2];
   	prod3 = prod3 * delay_2;
   	Goertzel_Value = prod1 + prod2 - prod3;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out_float[0][2] = Goertzel_Value/10000000000;
   	Goertzel_Value = 0;
   	}
}


void clk_SWI_GTZ_FLOAT_0941Hz(UArg arg0)
{
	static unsigned char N = 0;
	static float Goertzel_Value = 0;
	static float delay;
	static float delay_1 = 0;
	static float delay_2 = 0;
	float prod1, prod2, prod3;

   	prod1 = delay_1*coef_float[3];
   	delay = sample_float + prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = delay_1 * delay_1;
   	prod2 = delay_2 * delay_2;
   	prod3 = delay_1 * coef_float[3];
   	prod3 = prod3 * delay_2;
   	Goertzel_Value = prod1 + prod2 - prod3;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out_float[0][3] = Goertzel_Value/10000000000;
   	Goertzel_Value = 0;
   	}
}


void clk_SWI_GTZ_FLOAT_1209Hz(UArg arg0)
{
	static unsigned char N = 0;
	static float Goertzel_Value = 0;
	static float delay;
	static float delay_1 = 0;
	static float delay_2 = 0;
	float prod1, prod2, prod3;

   	prod1 = delay_1*coef_float[4];
   	delay = sample_float + prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = delay_1 * delay_1;
   	prod2 = delay_2 * delay_2;
   	prod3 = delay_1 * coef_float[4];
   	prod3 = prod3 * delay_2;
   	Goertzel_Value = prod1 + prod2 - prod3;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out_float[1][0] = Goertzel_Value/10000000000;
   	Goertzel_Value = 0;
   	}
}


void clk_SWI_GTZ_FLOAT_1336Hz(UArg arg0)
{
	static unsigned char N = 0;
	static float Goertzel_Value = 0;
	static float delay;
	static float delay_1 = 0;
	static float delay_2 = 0;
	float prod1, prod2, prod3;

   	prod1 = delay_1*coef_float[5];
   	delay = sample_float + prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = delay_1 * delay_1;
   	prod2 = delay_2 * delay_2;
   	prod3 = delay_1 * coef_float[5];
   	prod3 = prod3 * delay_2;
   	Goertzel_Value = prod1 + prod2 - prod3;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out_float[1][1] = Goertzel_Value/10000000000;
   	Goertzel_Value = 0;
   	}
}


void clk_SWI_GTZ_FLOAT_1477Hz(UArg arg0)
{
	static unsigned char N = 0;
	static float Goertzel_Value = 0;
	static float delay;
	static float delay_1 = 0;
	static float delay_2 = 0;
	float prod1, prod2, prod3;

   	prod1 = delay_1*coef_float[6];
   	delay = sample_float + prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = delay_1 * delay_1;
   	prod2 = delay_2 * delay_2;
   	prod3 = delay_1 * coef_float[6];
   	prod3 = prod3 * delay_2;
   	Goertzel_Value = prod1 + prod2 - prod3;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out_float[1][2] = Goertzel_Value/10000000000;
   	Goertzel_Value = 0;
   	}
}


void clk_SWI_GTZ_FLOAT_1633Hz(UArg arg0)
{
	static unsigned char N = 0;
	static float Goertzel_Value = 0;
	static float delay;
	static float delay_1 = 0;
	static float delay_2 = 0;
	float prod1, prod2, prod3;

   	prod1 = delay_1*coef_float[7];
   	delay = sample_float + prod1 - delay_2;
   	delay_2 = delay_1;
   	delay_1 = delay;
   	N++;

   	if (N==206)
   	{
   	prod1 = delay_1 * delay_1;
   	prod2 = delay_2 * delay_2;
   	prod3 = delay_1 * coef_float[7];
   	prod3 = prod3 * delay_2;
   	Goertzel_Value = prod1 + prod2 - prod3;
   	N = 0;
   	delay_1 = delay_2 = 0;
   	gtz_out_float[1][3] = Goertzel_Value/10000000000;
   	Goertzel_Value = 0;
   	}
}
