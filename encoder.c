#include "ENCODER.h"
#include <TM4C129.h>

int last_data = 0;


void init_Encoder(){
	SYSCTL->RCGCGPIO |=  (1UL << 3);                /* enable clock for GPIOs    */
	GPIOD_AHB->DR2R |=  (1ul << 0); 
  GPIOD_AHB->PDR  |=  (1ul << 0); 
  GPIOD_AHB->DIR  &=  ~(1ul << 0); 
  GPIOD_AHB->DEN  |= (1ul << 0); 
}

int check_Encoder(){
	int data = 0, state = 0;
	data = GPIOD_AHB->DATA;															 //Obtengo dato de N0.
	if((data == 1) && (data&last_data) == 0){						   //Si el dato está en 1 (flanco de subida) y el dato anterior era 0, es un nuevo pulso (ppr).
		state = 1;
	}else{
		state = 0;
	}
	last_data = data;																		   //Salvo el dato actual.
	return state;																				   //Determina si es nuevo pulso o no (en cada lectura).
}

int measure_Speed(int pulses){
	float rpm = 0;
	rpm = ((float)pulses/(float)PPR)*120;			                             //Calcula velocidad en rps*60 (segundo) = rpm.
	return (int)rpm;
}

