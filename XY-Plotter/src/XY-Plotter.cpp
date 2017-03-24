/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */
#if 1
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "ITM_write.h"
#include "GParser.h"
#include "drawingObject.h"
#include "debugEvent.h"
#include <stdlib.h>
#include <string.h>

#include "LPCXProject.h"
#define penTool 	1
#define laserTool 	0
QueueHandle_t xQueue = xQueueCreate( 20, sizeof(debugEvent) );
SemaphoreHandle_t xSemaphore = xSemaphoreCreateBinary();
GParser parser;
DrawingObject pen,laser;

int Xmillimeter=0;
int	Ymillimeter=0;
int	command=0;
int charCount=0;
int iNbTotalStepXaxis,iNbTotalStepYaxis;

static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	//Chip_IOCON_PinMuxSet(LPC_IOCON,1,11,(IOCON_MODE_PULLUP |IOCON_DIGMODE_EN |IOCON_INV_EN ));
	// initialize RIT (= enable clocking etc.)
	Chip_RIT_Init(LPC_RITIMER);
	// set the priority level of the interrupt
	// The level must be equal or lower than the maximum priority specified in FreeRTOS config
	// Note that in a Cortex-M3 a higher number indicates lower interrupt priority
	NVIC_SetPriority( RITIMER_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1 );
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 24); // X Motor
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 27); // Y Motor
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 28); // Y Direction
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 0); // X Direction
	/* Initial LED0 state is off */
	Board_LED_Set(0, false);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 29, IOCON_MODE_PULLUP);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 9, IOCON_MODE_PULLUP);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3, IOCON_MODE_PULLUP);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 0, IOCON_MODE_PULLUP);
}


/*
 * If mDraw puts something to UART this task is the one who receives mDraw commands
 * it Assembles a debugEvent which is sent to Queue
 */
static void ReceiverTask(void *pvParameters){
	int receivedChar;
	debugEvent debbie;
	while(1){
		if(xSemaphoreTake(xSemaphore,portMAX_DELAY)== pdTRUE){
			receivedChar=Board_UARTGetChar();
			if(receivedChar!=EOF){
				if(parser.takeCommand(receivedChar,charCount)){ //when enter is pressed true is returned
					debbie=parser.getDebugEvent();
					//ITM_write(debbie.echo);
					if(debbie.Gcode==110){
						xQueueSendToFront( xQueue, &debbie, portMAX_DELAY );
					}
					else{
					xQueueSendToBack( xQueue, &debbie, portMAX_DELAY );
					}
				}
			}

			xSemaphoreGive(xSemaphore);
		}
	}

}

static void MotorTask(void *pvParameters){
	char buffer[50];
	int iCurrentStepXaxis = 0;
	int iCurrentStepYaxis = 0;
	bool bEcho;
	debugEvent moveInfo;
	while(1){
		if(xQueueReceive( xQueue, &moveInfo, portMAX_DELAY)){
			if(xSemaphoreTake(xSemaphore,portMAX_DELAY)== pdTRUE){
				switch(moveInfo.Gcode){
				case 110:
					vQueueDelete( xQueue );
					Board_UARTPutSTR("M10 XY 310 350 0.00 0.00 A0 B0 H0 S80 U160 D90 \n");
					pen.write(160);
					laser.write(0);
					iNbTotalStepXaxis = CalibrationX();
					iCurrentStepXaxis = iNbTotalStepXaxis/2;
					iNbTotalStepYaxis = CalibrationY();
					iCurrentStepYaxis = iNbTotalStepYaxis/2;
					xQueue = xQueueCreate( 20, sizeof(debugEvent) );
					break;
				case 101:
					pen.write(moveInfo.Xplane);
					break;
				case 104:
					laser.write(moveInfo.Xplane);
					break;
				case 928:
					//move home xy(0,0)
					sprintf(buffer,"distance= %3.2f",getDistance(moveInfo.Xplane,moveInfo.Yplane));
					Board_UARTPutSTR(buffer);
					bEcho = MoveXYPlotter(0, 0, iNbTotalStepXaxis, iNbTotalStepYaxis);

					break;
				case 901:
					//move to moveinfo.Xplane and .Yplane

					//sprintf(buffer,"%3.2f;%3.2f;%3.2f \n",moveInfo.Xplane,moveInfo.Yplane,getDistance(moveInfo.Xplane,moveInfo.Yplane));
					//ITM_write(buffer);
					sprintf(buffer,"distance= %3.2f ",getDistance(moveInfo.Xplane,moveInfo.Yplane));
					Board_UARTPutSTR(buffer);
					bEcho = MoveXYPlotter(moveInfo.Xplane, moveInfo.Yplane, iNbTotalStepXaxis, iNbTotalStepYaxis);

					break;
				}
				Board_UARTPutSTR("OK \n");
				xSemaphoreGive(xSemaphore);
				vTaskDelay(5);
			}
		}
	}

}

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {
	prvSetupHardware();
	xSemaphoreGive(xSemaphore);

	laser.init(LPC_SCT1,SWM_SCT1_OUT0_O,1000,laserTool,0,12);
	pen.init(LPC_SCT0,SWM_SCT0_OUT0_O,50,penTool,0,10);

	xTaskCreate(ReceiverTask, "take command", configMINIMAL_STACK_SIZE * 3, NULL,
			(tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	xTaskCreate(MotorTask, "just for example", configMINIMAL_STACK_SIZE * 6, NULL,
			(tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	//ITM_init();
	/* Start the scheduler */
	vTaskStartScheduler();
}
#endif
