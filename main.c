#include <stdint.h>											
#include "PWM.h"												//Encabezado para usar PWM
#include "TM4C129.h"										// Device header
#include "BTN.h"
#include "TIMERS.h"
#include "LCD.h"
#include "SYSTICK.h"
#include "Serial.h"
#include "ENCODER.h"
//--------------------------------------------------
//--------------Prototipos--------------------------
void delay_sync(uint32_t);
void handleButtons(uint32_t);
void debounceClock(void);
void measureTime(void);
//--------------------------------------------------
static uint32_t debounceTicks;
static const uint32_t debouncePeriod = 2;
static int pulses = 0, currentSpeed = 0, userSpeed = 60;
//Retardo de systick para testeo
void delay_sync(uint32_t tiempo){
	NVIC_ST_CURRENT_R = 0X000000; //estado actual
	NVIC_ST_RELOAD_R = tiempo; //carga el tiempo
	NVIC_ST_CTRL_R |= 0x005;	//DIVIDE EL PSCILADOR POR 4
	while((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT)==0){}
	NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE); //Desactiva banderas
}

void handleButtons(){
	Start_Timer0();
	BTN_DisableInt();	
}

void debounceClock(){
	uint32_t btns= BTN_Get();
	// Si esta presionado el boton en el tick actual, acumula 
	if (btns>0){
		debounceTicks++; 
		// si el timer ha estado presionado durante intervalor de tiempos regulares
	if (debounceTicks % debouncePeriod	== 0){
		//volvemos a valir que se haya mantenido presionado
			if (btns>0){
				if (btns == 1){
					if (userSpeed+3>102){
						userSpeed = 102;
					}else {
						userSpeed+=3;
					}
				}	else if (btns == 2 ){
					
					if (userSpeed-3<0){
						userSpeed = 0;
					}else {
						userSpeed-=3;	
					}
				} else if (btns == 3){
					Min_duty_cycle();
				} else if (btns == 4) {
					Max_duty_cycle();
				}
				
			}else{
				// si se cumplió el tiempo pero no habia nada presionado, desactivamos el timer y reiniciamos la operacion
				Stop_Timer0();
				debounceTicks = 0;
				BTN_EnableInt();
			}
			
	}
	}else{
		// si se llegó a soltar, deten el timer y reinicia todo
		Stop_Timer0();
		debounceTicks = 0;
		BTN_EnableInt();
	}
}



void measureTime(){
	currentSpeed = measure_Speed(pulses);
			//while
	if(currentSpeed - 5 <= userSpeed){
		aumentar_duty_cycle(800);
	}else if(currentSpeed + 5 >= userSpeed){
		disminuir_duty_cycle(800);
	}
			//end while
	
	SER_OutString("Vel esperada : ");
	SER_PutDec((uint32_t)userSpeed);
	SER_PutChar('\n');
	SER_OutString("Vel real : ");
	SER_PutDec((uint32_t)currentSpeed);
	SER_PutChar('\n');
	
	LCD_OutString("Vel esp :       ");
	LCD_setCursor(10,0);
	LCD_OutUDec((uint32_t)userSpeed);
	LCD_setCursor(0,1);
	LCD_OutString("Vel real:       ");
	LCD_setCursor(11,1);
	LCD_OutUDec((uint32_t)currentSpeed);
	LCD_ReturnHome();
	
	pulses = 0;
}
//Main de prueba
int main(){
	//********************
	BTN_Initialize(); // inicializamos los botones
	SER_Initialize();
	PWM_config(60000,0x0001);		//Configura PWM en el maximo periodo con el minimo de duty cycle
	initSysTick1ms();
	init_Encoder();
	LCD_Init();
	LCD_Clear();
	//********************
	// Interrupciones
	BTN_SetupInt(handleButtons); // activamos las interupciones de los botones
	Init_Timer0(debounceClock,50); // activamos las interupciones del timer 0, 1 y 2
	Init_Timer2(measureTime,500);
	Start_Timer2();
	while(1){
	
	if(check_Encoder() == 1){
			pulses++;									    //Aumenta pulsos.
		}
	}

	
	
}
