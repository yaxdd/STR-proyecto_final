#include "stdio.h"
#include "encoder.h"

int pulses = 0, currentSpeed = 0, userSpeed = 150;
/*
	*-*-*-*-* Pseudocódigo del main *-*-*-*-*
	
	***Esto es a parte de lo de abajo y las declaraciones de arriba.***
	
	#Incluir librerias de FREERTOS
	
	#Declarar variables userSpeed,currentSpeed y pulses
	
	--
	Tarea 1: Verificar pulsos. PRIORIDAD 1
		
		Revisar encoder.
			Si encoder es 1, aumentar *pulses*.
			Si encoder es 0, no hace nada.
			
	--
	
	PD: Leí por ahí que para que la entrada del encoder no dé falsos positivos debe
	ponerse un tipo de filtro que evite los rebotes indeseados.	
	
	--
	Tarea 2: Actualizar velocidad y retroalimentar. PRIORIDAD 2
	
		Si la bandera del temporizador es 1:
			*Calcular velocidad y actualizar currentSpeed.
			*Mientras la velocidad actual sea distinta de la velocidad del usuario. (Quizás haya que poner una tolerancia
			 para que no se quede en un ciclo infinito)
				*Si la velocidad actual es menor, pwm++
				*Si la velocidad actual es mayor, pwm--
				
	--
	--
	Tarea 3: Interfaz del usuario. PRIORIDAD 3
		
		Si botón 1 es apretado, userSpeed++
		Si botón 2 es apretado, userSpeed--
		
	--
	--
	Tarea 4: Mostrar datos en pantalla. PRIORIDAD 4
	
		Mostrar velocidad actual (currentSpeed).
		Mostrar velocidad deseada del usuario (userSpeed).
		
	--
*/

int main(void){
	//Configuraciones iniciales.
	
	//Para pruebas del Encoder...
	init_Encoder();
	
	while(1){
		if(check_Encoder() == 1){
			pulses++;									    //Aumenta pulsos.
		}
		
		if((TIMER0_RIS_R&0x01) == 1){		//Ya pasó 1 segundo (quizás más hasta la verificación).
			timer_Off();
			TIMER0_ICR_R |= 0x01;	
			currentSpeed = measure_Speed(pulses);
			//while
			if(currentSpeed - 5 <= userSpeed){
				 //pwm++
			}
			if(currentSpeed + 5 >= userSpeed){
				//pwm--;
			}
			//end while
			pulses = 0;
			TIMER0_TAV_R = 0;
			timer_On();
		}
	  //Mostrar velocidades.
	}
}
