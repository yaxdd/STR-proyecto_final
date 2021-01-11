#ifndef PWM_H_
#define PWM_H_

/* Se incluyen las librerias a utilizar */
#include <stdint.h>
#include "tm4c1294ncpdt.h"

void PWM_config(uint16_t periodo, uint16_t duty_cycle);
void aumentar_duty_cycle(int valor);
void disminuir_duty_cycle(int valor);
void Establecer_duty_cycle(uint16_t valor);
void Min_duty_cycle(void);
void Max_duty_cycle(void);



#endif // PWM_H_
