/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"
#include "init.h"
#include "lightutils.h"

void StartApplication (void);
void thread1 (void const *argument);
void thread2 (void const *argument);

osThreadId tid_thread1;                         
osThreadId tid_thread2;                         

osThreadDef(thread1, osPriorityHigh,   1, 0);
osThreadDef(thread2, osPriorityNormal, 1, 0);

osSemaphoreId semaphore;                        
osSemaphoreDef(semaphore); 



int main (void) {
	osKernelInitialize();                    // initialize CMSIS-RTOS
	
	labInit();
	StartApplication();

  osKernelStart();                         // start thread execution 
}


void thread1 (void const *argument) {
  int32_t value;
  while (1) {                         
    osSemaphoreWait (semaphore, osWaitForever);        
		lightsOn(BLUE);
		osDelay( 400); 
		lightsOff(BLUE);
		osDelay( 100);                                                                          
		osSemaphoreRelease (semaphore);            
  }
}
  
void thread2 (void const *argument) {
  while (1) {
    osSemaphoreWait (semaphore, osWaitForever);                   
    lightsOn(RED);
		osDelay( 400); 
		lightsOff(RED);
		osDelay( 100);             
		osSemaphoreRelease (semaphore);
  }
}

void StartApplication (void) {
  semaphore = osSemaphoreCreate(osSemaphore(semaphore), 1);
  tid_thread1 = osThreadCreate(osThread(thread1), NULL);
  tid_thread2 = osThreadCreate(osThread(thread2), NULL);
}

