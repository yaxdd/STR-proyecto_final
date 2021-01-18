/*
 * Archivo:   encoder.h
 * Autor: 		Mario Iván Pech Pérez
 *
 */

#ifndef ENCODER_H_
#define ENCODER_H_

/* Se incluyen las librerias a utilizar */
#include <stdint.h>
#include "tm4c1294ncpdt.h"

#define PPR 20				//Pulso Por Revolución.
extern int last_data;

/* Definicion de las funciones a utilizar */

int check_Encoder(void);

int measure_Speed(int pulses);

void config_TimerA(void);

void config_PortN(void);

void init_Encoder(void);

//int get_Ms(void);

void timer_On(void);

void timer_Off(void);

#endif // ENCODER_H_
