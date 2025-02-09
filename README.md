# This is a SPWM Inverter (PIC16F1829)

## 📌 Project Overview
This project implements a **SPWM (Sinusoidal Pulse Width Modulation) based inverter** using **PIC16F1829**, generating a **50Hz AC output** through an **H-Bridge with MOSFET drivers** to achieve **DC-AC conversion**.

- **Input Voltage**: DC 24V  
- **Output Voltage**: AC 24V  
- **Output Frequency**: 50Hz  
- **Power Range**: Up to 100W  
- **PWM Resolution**: 10-bit  
- **H-Bridge MOSFET**: IRF840  
- **Driver IC**: 1EDC20I12AH  
- **MCU Development Environment**: MPLAB X IDE + XC8 Compiler

- ## 🔌 **Circuit Design**
### 1️⃣ **SPWM Generation**
- **64-point sine wave lookup table (LUT)**
- **MCU generates PWM using Timer2**
- **Uses ECCP Full-Bridge PWM mode**

### 2️⃣ **H-Bridge Configuration**
- **P1A + P1D handle Forward mode**
- **P1B + P1C handle Reverse mode**
- **MOSFETs are controlled by 1EDC20I12AH driver**

### 3️⃣ **Negative Voltage Generation**
- **Ensures MOSFETs fully turn OFF**

More details please reading my Blogger "https://bobhongdoghouse.blogspot.com/2025/01/diy-dc-ac-inverter.html"

