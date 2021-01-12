#include "encoder.h"
#include "tm4c1294ncpdt.h"

int last_data = 0;

void config_PortN(){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12; 					 //Enciendo el bit 12 para activar el puerto N.
	while((SYSCTL_PRGPIO_R&SYSCTL_RCGCGPIO_R12) == 0){}; //Verifico que el bit 12 esté en 1 (Puerto N activado). De lo contrario, se mantendrá en el while.
  GPIO_PORTN_AFSEL_R &= 0; 													   //Habilito todos los pines como GPIO controlados por registros GPIO (General Purpose Input/Output).
	GPIO_PORTN_DIR_R &= ~0x01; 												   //Habilito el N0 como entrada asignándole 0.
	GPIO_PORTN_DEN_R &= 0x01; 													 //Habilito el N0 como digital.
}

void config_TimerA(){
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;					 //Habilito timer0.
	while((SYSCTL_PRTIMER_R&SYSCTL_RCGCTIMER_R0) == 0){};//Verifico que el timer está activado.
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;										 //Verifico que el timer no esté encendido.
	TIMER0_CFG_R = 0x00;																 //Modo de 32 bits.
	TIMER0_TAMR_R = 0x18;															   //Timer A en modo periódico y cuenta hacia arriba.
	TIMER0_TAILR_R = 0x00F42400;												 //Valor esperado 16,000,000 (16 MHz = 1 segundo para PIOSC). 
	TIMER0_CTL_R |= TIMER_CTL_TAEN;											 //Inicio el timer.
	TIMER0_ICR_R |= 0x01;																 //Bajo bandera por seguridad.
}

void init_Encoder(){
	config_TimerA();																		 //Inicializa Timer.
	config_PortN();																		   //Inicializa puerto N.
}

int check_Encoder(){
	int data = 0, state = 0;
	data = GPIO_PORTN_DATA_R;															 //Obtengo dato de N0.
	if((data == 1) && (data&last_data) == 0){						   //Si el dato está en 1 (flanco de subida) y el dato anterior era 0, es un nuevo pulso (ppr).
		state = 1;
	}else{
		state = 0;
	}
	last_data = data;																		   //Salvo el dato actual.
	return state;																				   //Determina si es nuevo pulso o no (en cada lectura).
}

int measure_Speed(int pulses){
	int rpm = 0;
	rpm = (pulses/PPR)*60;			                             //Calcula velocidad en rps*60 (segundo) = rpm.
	return rpm;
}

//int get_Ms(){
//	int timerticks = 0, milliseconds = 0;
//	timerticks = 0x00F42400 + TIMER0_TAV_R;							 //Calcula el segundo transcurrido hasta la verificación de la bandera del timer.
//	milliseconds = timerticks*(float)1/16000000*1000;    //Calcula el tiempo del timer (ticks) a milisegundos (con PIOSC de 16 MHz).
//	return milliseconds;
//}

void timer_On(){
	TIMER0_CTL_R |= TIMER_CTL_TAEN;											 //Inicio el timer.
}

void timer_Off(){
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;										//Apago el timer.
}

