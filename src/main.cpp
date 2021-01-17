/*******************************************************************************
* Copyright (C) 2020 TUSUR, FB, #2376  
* File              : main.cpp
* IDE               : NetBeans
* Version           : 8.2
* Created File      : 27.10.2020
* Last modified     : 17.01.2021
*
* Support mail      : s.b_sharkova@mail.ru
*
* Target MCU        : MDR1986
*
* Description       : lab2 TIMER PWM
*                   :
*******************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <main.h>
#include <stdlib.h>
#include <inttypes.h>
#include "mstn_version.h" 
#include "mstn_led.h" 
#include "mstn_clk.h" 
#include "mstn_usb.h" 
#include "mstn_button.h" 
#include "mstn_external_interrupt.h" 
#include "mstn_systick_interrupt.h"
#include "mstn_led.h"

#define PERIOD              5                               //длительность одного уровня (5*200мс)
#define BRIGHTNESS_LEVELS   5                               //количество уровней ярковсти

volatile uint16_t   delta = UINT16_MAX/BRIGHTNESS_LEVELS;   //яркость изменяется от 0% до 100% с заданным шагом


void CountTime()//обработчик прерыввания от таймера с периодом 200мс
{
    static uint8_t  green_counter,              //счетчик для времени одного состояния диодов
                    level_counter,              //счетчик уровней яркости
                    red_state;                  //состояние красного диода
    green_counter = ++green_counter % PERIOD;                         
    if (!green_counter)                                                     //если прошло 5*200мс
    {
        level_counter = (++level_counter) % BRIGHTNESS_LEVELS;              //переключаем уровень
        red_state = !red_state;                                             //инвертируем состояние красного диода
        
        LED_SetPwmGreen(level_counter * delta);                             //устанавливаем яркость
        LED_SetRedState(red_state);                                         //установливаем состояние красного диода
    }
}



int main(void)
{
    STI_SetPeriod_ms(200);              //задаем период прерывания по таймеру 200мс (максимум, больше нельзя, поэтому и использовали счетчик)
    STI_AttachInterrupt(CountTime);
    while(1)
    {}
    return EXIT_SUCCESS;
} 