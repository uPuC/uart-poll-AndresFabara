#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

// Colores ANSI (0?7)
#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7

// Prototipos

void UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop);

// Transmisi?n
void UART_putchar(uint8_t com, char data);
void UART_puts(uint8_t com, char *str);

// Recepci?n
uint8_t UART_available(uint8_t com);
char UART_getchar(uint8_t com);
void UART_gets(uint8_t com, char *str);

// Secuencias de escape ANSI
void UART_clrscr(uint8_t com);
void UART_setColor(uint8_t com, uint8_t color);
void UART_gotoxy(uint8_t com, uint8_t x, uint8_t y);

// Conversi?n
void itoa(uint16_t number, char* str, uint8_t base);
uint16_t atoi(char *str);

	#endif
