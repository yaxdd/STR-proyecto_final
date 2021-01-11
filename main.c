#include <stdint.h>											
#include "PWM.h"												//Encabezado para usar PWM
#include "TM4C129.h"										// Device header
#include "BTN.h"
#include "TIMERS.h"
#include "LCD.h"
#include "SYSTICK.h"
//--------------------------------------------------
//--------------Prototipos--------------------------
void delay_sync(uint32_t);
void handleButtons(uint32_t);
void debounceClock(void);
//--------------------------------------------------
static uint32_t debounceTicks;
static const uint32_t debouncePeriod = 2;

//Retardo de systick para testeo
void delay_sync(uint32_t tiempo){
	NVIC_ST_CURRENT_R = 0X000000; //estado actual
	NVIC_ST_RELOAD_R = tiempo; //carga el tiempo
	NVIC_ST_CTRL_R |= 0x005;	//DIVIDE EL PSCILADOR POR 4
	while((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT)==0){}
	NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE); //Desactiva banderas
}
void test(int ms)
{
    volatile int i;
    for (i = 0; i < ms; i++)
		__NOP();
//        for (j = 0; j <= 1; j++) {}   // // Do nothing for 1ms
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
					aumentar_duty_cycle(1000);
				}	else if (btns == 2 ){
					disminuir_duty_cycle(1000);
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
//Main de prueba
int main(){
	//********************
	BTN_Initialize(); // inicializamos los botones
	PWM_config(0xFFFF,0x0001);		//Configura PWM en el maximo periodo con el minimo de duty cycle
	initSysTick1ms();
	LCD_Init();
	LCD_Clear();
	//********************
	// Interrupciones
	BTN_SetupInt(handleButtons); // activamos las interupciones de los botones
	Init_Timer0(debounceClock,50); // activamos las interupciones del timer 0, 1 y 2
	
	while(1){
	LCD_OutString("Vel ini : ");
	LCD_OutUDec(PWM0_0_CMPA_R);
	//LCD_setCursor(0,1);
	//LCD_OutString("Vel real: ");
	LCD_ReturnHome();
	
	}

	
	
}
