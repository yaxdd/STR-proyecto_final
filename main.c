#include <stdint.h>											
#include "PWM.h"												//Encabezado para usar PWM
#include "TM4C129.h"                    // Device header
#include "FreeRTOSConfig.h"             // ARM.FreeRTOS::RTOS:Config
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "semphr.h"    



//Retardo de systick para testeo
void retardo(uint32_t tiempo){
	NVIC_ST_CURRENT_R = 0X000000; //estado actual
	NVIC_ST_RELOAD_R = tiempo; //carga el tiempo
	NVIC_ST_CTRL_R |= 0x005;  //DIVIDE EL PSCILADOR POR 4
	while((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT)==0){};
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; //Desactiva banderas
}

//Main de prueba
int main(){
	SYSCTL->RCGCGPIO=0X00007FFF;	//activa puertos
	
		//Puerto J pines 0 y 1 como entrada
	GPIOJ_AHB->DEN =0xFF;					//Pines en modo digital
	GPIOJ_AHB->DIR = 0x00;				//Declaracion de pines como entrada
	GPIOJ_AHB->PUR = 0x03;				//Resistencias pull up en pines 0 y 1
	GPIOJ_AHB->AFSEL =0x00;				//Sin función especial
	
	PWM_config(0xFFFF,0x0001);		//Configura PWM en el maximo periodo con el minimo de duty cycle

	
	while(1){
		if((GPIOJ_AHB->DATA&0x01)==0){				//Use boton en PJ0 para aumentar duty cycle
			aumentar_duty_cycle(1000);
			
		}else if((GPIOJ_AHB->DATA&0x02)==0){	//Use boton en PJ1 para disminuir duty cycle
			disminuir_duty_cycle(1000);
		}

		retardo(800000);											//Retardo para botón
	}

	
	
}
