# STM32 Bare-Metal: ADC Read from PA1 (Channel 1)

---

## 📌 Summary

This project demonstrates how to perform a single ADC (Analog-to-Digital Converter) reading on the STM32F4 Discovery board using **ADC1** and **PA1 (Channel 1)**.  
The entire configuration is done at the **register level**, without using HAL or CMSIS.  
It reads analog voltage values through polling by monitoring the **EOC (End Of Conversion)** flag.

---

## 🔁 Previous Lesson

If you haven’t completed the previous lesson where we implemented USART-based communication, check it out here:

👉 [Previous Lesson: Bidirectional UART (USART2 <-> USART3)](https://github.com/iek2443/stm32-baremetal-uart-bidirectional)

---

## 🧠 What You Will Learn

- How to configure PA1 as an analog input pin
- How to enable and configure **ADC1** for single conversion
- How to manually trigger conversions with `SWSTART`
- How to poll the `EOC` flag in the status register
- How to read ADC result from the **DR (Data Register)**

---

## ⚙️ Key Registers Used

- `RCC->AHB1ENR` → Enables clock for GPIOA
- `GPIOA->MODER` → Configures PA1 as analog mode
- `RCC->APB2ENR` → Enables clock for ADC1
- `ADC1->SQR3`   → Selects the ADC channel (channel 1 = PA1)
- `ADC1->SQR1`   → Sets the sequence length
- `ADC1->CR2`    → Enables ADC and starts software conversion
- `ADC1->SR`     → Checks for EOC (End Of Conversion)
- `ADC1->DR`     → Holds the final 12-bit ADC result

---

## 🔧 Requirements

- STM32F4 Discovery Board
- Analog sensor or potentiometer connected to PA1
- ARM GCC Toolchain
- ST-Link programmer
- (Optional) Serial output for displaying result

---

📁 Project Structure
--------------------

stm32-baremetal-adc-read/\
├── src/\
│   └── main.c         --> Bare-metal ADC read logic (PA1)\
├── inc/               --> (Optional: header files)\
└── README.md

---

🧭 Pin Mapping

| Function        | Port | Pin | Description             |
|-----------------|------|-----|-------------------------|
| ADC Input       | A    |  1  | Channel 1 analog input  |

---

## 🔁 Conversion Flow

	1.	Set PA1 to analog mode
	2.	Configure ADC1 to use channel 1
	3.	Start conversion with software trigger
	4.	Wait until EOC flag is set
	5.	Read value from ADC1->DR

--- 
