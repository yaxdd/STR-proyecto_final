#include "PWM.h"			//Encabezado para usar PWM


//Configuracion inicial de PWM, salida en Puerto F, pin 0,a 1 MHz
void PWM_config(uint16_t periodo, uint16_t duty_cycle){
	SYSCTL_RCGCPWM_R = 0x0001;																				//Habilita y proporciona un reloj al módulo PWM 0 en el modo de ejecución.
	SYSCTL_RCGCGPIO_R  |= SYSCTL_RCGCGPIO_R5; 												//Habilita el reloj en puerto F
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5) == 0){};									//Verifica que se activo el puerto
	GPIO_PORTF_AFSEL_R	|= 0x01;																			//Seleccion de funcion alternativa en F0
	GPIO_PORTF_DEN_R |= 0x01;             														//Habilita funcion digital en PF0
	GPIO_PORTF_PCTL_R		= (GPIO_PORTF_PCTL_R&0xFFFFFFF0)+0x00000006;;	//Funcion alternativa para salida de PWM0
	GPIO_PORTF_AMSEL_R &= ~0x01;																			//Desactiva funcion analógica

	PWM0_CC_R = 0x0003;																								//Divide el reloj de sistema entre 16
	PWM0_0_CTL_R = 0x00;																							//Desactiva timers para configuración										
	PWM0_0_GENA_R = 0x00C8;																						//Define señal en bajo cuando counter=LOAD y señal en alto cuando counter=CMPA
	PWM0_0_LOAD_R = periodo-1;																				//Carga periodo
	PWM0_0_CMPA_R = duty_cycle-1;																			//Carga Ciclo de trabajo
	PWM0_0_CTL_R = 0x0001;																						//Inicia timers en generador 0
	PWM0_ENABLE_R = 0x01;																							//Habilita pin de salida para PWM
}

//Aumenta Duty cycle en la cantidad deseada
void aumentar_duty_cycle(int valor){
	int actual;																//Variable para modificar valor
	actual = PWM0_0_CMPA_R;										//Toma valor actual de duty cycle
	actual = actual + valor;							    //Aumenta valor de duty cycle
	if(actual >= PWM0_0_LOAD_R){							//Verifica que no se supere el máximo valor
		actual = PWM0_0_LOAD_R -1;							//Si se pasa del maximo se corrige con el valor maximo posible
	}
	PWM0_0_CMPA_R = actual;										//Carga valor de duty cycle
	
}

//Disminuye Duty cycle en la cantidad deseada
void disminuir_duty_cycle(int valor){
	int actual;																//Variable para modificar valor
	actual = PWM0_0_CMPA_R;										//Toma valor actual de duty cycle
	actual = actual - valor;									//Disminuye valor de duty cycle
	if(actual < 0){														//Verifica que no se supere el valor mínimo
		actual = 1;															//Si se pasa del mínimo se corrige con el valor mínimo posible
	}			
	PWM0_0_CMPA_R = actual;										//Carga valor de duty cycle
	
}
void Max_duty_cycle(){
	PWM0_0_CMPA_R = PWM0_0_LOAD_R -1;										//Carga valor de duty cycle
}
void Min_duty_cycle(){
	PWM0_0_CMPA_R = 1;										//Carga valor de duty cycle
}
//Establece duty cycle en la cantidad deseada
void Establecer_duty_cycle(uint16_t valor){
	PWM0_0_CMPA_R = valor;										//Carga nuevo duty cycle
}

