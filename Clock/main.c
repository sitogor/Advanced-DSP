/*
 * main.c
 *
 *  Created on: 7 Apr 2021
 *      Author: Scott
 */
#include <xdc/cfg/global.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
Semaphore_Handle sem;
Task_Handle tsk;

unsigned long long freq;
unsigned long long now;
Types_FreqHz s_freq;
Types_Timestamp64 s_now;
double uptime;

//Must follow the same type as Task_FuncPtr.
void task_print(UArg, UArg);

void main() {
	System_printf("Entered main()\n");
	//Get timestamp trigger frequency.
	Timestamp_getFreq(&s_freq);
	freq = (unsigned long long) s_freq.hi << 32
			| (unsigned long long) s_freq.lo;
	//Set up a task to perform the printing, since Hwi cannot print.
	Task_Params tp;
	Task_Params_init(&tp);
	tp.priority = 1;
	tsk = Task_create(&task_print, &tp, NULL);
	//Set up a semaphore for triggering the print task.
	Semaphore_Params sp;
	Semaphore_Params_init(&sp);
	sp.mode = Semaphore_Mode_BINARY;
	sem = Semaphore_create(0, &sp, NULL);
	//Start the clock function, since it isn't started by default.
	Clock_start(clock0);
	//Start the SYS/BIOS scheduler.
	BIOS_start();
}

/*
 * This clock function was set up statically in the .cfg file.  Lines 8-12.  It triggers every 10 ms.
 * var clock0Params = new Clock.Params();
 * clock0Params.instance.name = "clock0";
 * clock0Params.period = 10;
 * clock0Params.startFlag = false;
 * Program.global.clock0 = Clock.create("&clk0_func", 10, clock0Params);
 */
void clk0_func(UArg unused) {
	//What's the system time?
	Timestamp_get64(&s_now);
	//Convert
	now = (unsigned long long) s_now.hi << 32 | (unsigned long long) s_now.lo;
	uptime = (double) now / (double) freq;
	//Cannot call System_printf() in a Swi, so use Semaphore to tell a task to do it instead.
	Semaphore_post(sem);
}

void task_print(UArg a0, UArg a1) {
	while (1) {
		//Wait until timer has fired.
		static int tick;
		Semaphore_pend(sem, BIOS_WAIT_FOREVER);
		tick = Clock_getTicks();
		//Print out the current uptime as detected by the timer's Hwi.
		System_printf("uptime: %lf\n", uptime);
		System_printf("\ntick: %d\n", tick);
	}
}
