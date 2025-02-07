/* 
 * File:   main.c
 * Author: bh.hong
 * purpose inverter, AC24, 50Hz sinewave generator
 *MCU : PIC16F1829
 * Created on 2025_Jan_26, 15:39
 */
#include <xc.h> 
#include <stdio.h>

#define _XTAL_FREQ 32000000 // Setting ferquence, 32Mhz

// 64 points SPWM lut?1023 PWM 10-bit
const unsigned int LUT[64] = {
        25, 50, 74, 99, 124, 148, 172, 195,
    218, 240, 262, 283, 304, 324, 343, 361,
    378, 395, 410, 424, 438, 450, 461, 472,
    481, 488, 495, 501, 505, 508, 510, 511,
    510, 508, 505, 501, 495, 488, 481, 472,
    461, 450, 438, 424, 410, 395, 378, 361,
    343, 324, 304, 283, 262, 240, 218, 195,
    172, 148, 124, 99, 74, 50, 25, 0
}; // lu table
unsigned char lut_index = 0; // LUT index
unsigned char direction = 0; // H direction (0 = Forward, 1 = Reverse)

// TMR2 interrput 
void __interrupt() ISR() {
    if (PIR1bits.TMR2IF) {
        PIR1bits.TMR2IF = 0;  // clear TMR2 interrput flag

        // Set Full-Bridge Model (Change direction every 64 points)
        if (lut_index == 0) {
            direction = !direction;  //Reverse direction of full-bridge
            CCP1CONbits.P1M0 = 1; // P1M0(bit7) = 1 ; Set Full-Bridge Model
            CCP1CONbits.P1M1 = direction; // change H-bridge // P1M1(bit6) Forward  Reverse change
        }

        
        // Get PWM Duty Cycle
        unsigned int duty = LUT[lut_index];

        // Update PWM REGISTER
        CCPR1L = (unsigned char)(duty >> 2); //convert to 8bit
        CCP1CONbits.DC1B = duty & 0x03;

        // Update "LUT" index
        lut_index++;
        if (lut_index >= 64) {
            lut_index = 0;  // peroid cycle
        }
    }
}

// PWM initial
void setupPWM() {
    // Set P1A (RC5), P1B (RC4), P1C (RC3), P1D (RC2) output
    TRISCbits.TRISC5 = 0; // P1A
    TRISCbits.TRISC4 = 0; // P1B
    TRISCbits.TRISC3 = 0; // P1C
    TRISCbits.TRISC2 = 0; // P1D

    // set Full-Bridge PWM (initial Forward)
    CCP1CON = 0b00001100; // ECCP; Full-Bridge Forward Model
    PWM1CON = 0b10000000; // enable PWM?enable Dead-Time
    PSTR1CON = 0b00001100; // Enable Full-Bridge PWM output (P1A, P1B, P1C, P1D)
//    PSTR1CON = 0b00011111; // Enable Full-Bridge PWM output (P1A, P1B, P1C, P1D)

    // Setting Dead-Time Protect H_bridge
    PWM1CONbits.P1DC = 5; // cinfig Dead-Time = 5 clock's period

    // PWM On
    CCPR1L = 0;
}

// setting Timer2 trigger SPWM update
void setupTimer2() {
    TMR2 = 0; // clear TMR2
    PR2 = 155; // setting  PR2, TMR2 trigget interrput every 156.25 us
    T2CON = 0b00001100; // TMR2  Postscaler 1:2(0001), TMR2(0), Prescaler 1:1(00)

    PIR1bits.TMR2IF = 0;   // clear interrput flag
    PIE1bits.TMR2IE = 1;   // interrput by TMR2
    INTCONbits.PEIE = 1;   // Enable interrput (Peripherals)
    INTCONbits.GIE = 1;    // Enable interrput
}

// Main
void main() {
    OSCCON = 0b11110000; // OSC internal frequsnce 32MHz
    APFCON1 = 0b00000011;
    setupPWM();   // Set PWM
    setupTimer2(); // set Timer2

    while(1);
}
