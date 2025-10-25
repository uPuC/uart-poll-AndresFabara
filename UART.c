#include <avr/io.h>
#include "UART.h"

// Envío de un caracter al periferico (UART)
void UART_putchar(uint8_t com, char data) { 
    //Hay hasta 4 com, 0 a 3 si se necesita habilitar cualquier UART
    switch (com) { 
    case 0: 
	while (!(UCSR0A & (1 << UDRE0))); 
	UDR0 = data; 
	break; 
    case 1: 
	while (!(UCSR1A & (1 << UDRE1))); 
	UDR1 = data; 
	break; 
    case 2: 
	while (!(UCSR2A & (1 << UDRE2))); 
	UDR2 = data; 
	break; 
    case 3: 
	while (!(UCSR3A & (1 << UDRE3))); 
	UDR3 = data; 
	break; 
    default: 
	break; 
    } 
} 
// Si el dato esta disponible en el periferico 
uint8_t UART_available(uint8_t com) { 
    //Hay hasta 4 com, 0 a 3 si se necesita habilitar una compuerta
    switch (com) { 
    case 0: 
	return (UCSR0A & (1 << RXC0)) ? 1 : 0; 
    case 1: 
	return (UCSR1A & (1 << RXC1)) ? 1 : 0; 
    case 2: 
	return (UCSR2A & (1 << RXC2)) ? 1 : 0; 
    case 3: 
	return (UCSR3A & (1 << RXC3)) ? 1 : 0; 
    default: 
	return 0; 
    } 
}  
// Recepción de un carácter  
char UART_getchar(uint8_t com) { 
    //Hay hasta 4 com, 0 a 3 si se necesita habilitar una compuerta
    switch (com) { 
    case 0: 
	while (!(UCSR0A & (1 << RXC0))); 
	return UDR0; 
    case 1: 
	while (!(UCSR1A & (1 << RXC1))); 
	return UDR1; 
    case 2: 
	while (!(UCSR2A & (1 << RXC2))); 
	return UDR2; 
    case 3: 
	while (!(UCSR3A & (1 << RXC3))); 
	return UDR3; 
    default: 
	return 0; 
    } 
}  
// Recepción de cadena con edición básica 
void UART_gets(uint8_t com, char *str) { 
    if(com > 3){
	   if(str) str[0] = '\0';
	   return;
    }
	uint8_t i = 0; 

	UART_puts(com, "\x1B[s");
	UART_puts(com, "\x1B[K");

	while(1){
	   char c = UART_getchar(com);

	   if(c == '\r' || c == '\n'){
	      str[i] = '\0';
		  break;
       }else if (c == 8 || c == 127){
	      if(i > 0){
		    i--;
			str[i] = '\0';
			UART_puts(com, "\b \b");
          }else{
		    UART_puts(com, "\x1B[u");
          }
       }else if(i < 16) {
	      str[i++] = c;
		  UART_putchar(com, c);
       }
    } 
} 
 
// Envío de una cadena 
void UART_puts(uint8_t com, char *str) { 
    while (*str) 
	  UART_putchar(com, *str++); 
}
// Conversión: entero a cadena 
void itoa(uint16_t number, char *str, uint8_t base) { 
    char buffer[17]; 
    uint8_t i = 0; 
    if (base < 2 || base > 16) {
	   str[0] = '\0'; 
	   return; 
	} 
    if (number == 0) { 
	   str[0] = '0'; 
	   str[1] = '\0'; 
	   return; 
	} 
 
    while (number > 0) { 
        uint8_t digit = number % base; 
        buffer[i++] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10); 
        number /= base; 
    } 
    for (uint8_t j = 0; j < i; j++) 
	  str[j] = buffer[i - j - 1]; 
      str[i] = '\0'; 
} 
 
// Conversión: cadena a entero 
uint16_t atoi(char *str) { 
    uint16_t result = 0; 
    while (*str >= '0' && *str <= '9') { 
	    result = result * 10 +(*str - '0');
        str++; 
    } 
    return result; 
} 
 
// Inicialización del UART
//com es el UART a configurar, baudrate es la velocidad del baud
//size representa la cantidad de bits a manerjar, parity la paridad que maneja el UART
//stop es el stop bits, que es el numero de bits que para. 
void UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop) { 
    uint32_t baud16 = (16000000 / ((uint32_t)16 * baudrate)) - 1; 
    uint32_t baud8  = (16000000 / ((uint32_t)8  * baudrate)) - 1; 
 
    volatile uint8_t *UCSRB, *UCSRC, *UCSRA; 
    volatile uint16_t *UBRR; 
 
    switch (com) { 
        case 0: 
		UCSRA = &UCSR0A; 
		UCSRB = &UCSR0B; 
		UCSRC = &UCSR0C; 
		UBRR = &UBRR0; 
		break; 
        case 1: 
		UCSRA = &UCSR1A; 
		UCSRB = &UCSR1B; 
		UCSRC = &UCSR1C; 
		UBRR = &UBRR1; 
		break; 
        case 2: 
		UCSRA = &UCSR2A; 
		UCSRB = &UCSR2B; 
		UCSRC = &UCSR2C; 
		UBRR = &UBRR2; 
		break; 
        case 3: 
		UCSRA = &UCSR3A; 
		UCSRB = &UCSR3B; 
		UCSRC = &UCSR3C; 
		UBRR = &UBRR3; 
		break; 
        default: 
		return; 
    } 
 
    if (baud16 < baud8) { 
	  *UBRR = baud16; 
	  *UCSRA &= ~(1 << U2X0); 
	} 
    else{ 
	  *UBRR = baud8;  
	  *UCSRA |= (1 << U2X0); 
	} 
 
    *UCSRB = (1 << TXEN0) | (1 << RXEN0); 
 
    switch (size) { 
        case 5: 
		*UCSRC |= (0 << UCSZ00) | (0 << UCSZ01); 
		break; 
        case 6: 
		*UCSRC |= (1 << UCSZ00) | (0 << UCSZ01); 
		break; 
        case 7: 
		*UCSRC |= (0 << UCSZ00) | (1 << UCSZ01); 
		break; 
        case 8: 
		*UCSRC |= (1 << UCSZ00) | (1 << UCSZ01); 
		break; 
        default: 
		return; 
    } 
 
    switch (parity) { 
        case 0: 
		*UCSRC &= ~(3 << UPM00); 
		break; 
        case 1: 
		*UCSRC |= (3 << UPM00); 
		break; 
        case 2: 
		*UCSRC |= (2 << UPM00); 
		break;
		default: 
		return; 
    } 
 
    switch (stop) { 
        case 1: 
		*UCSRC &= ~(1 << USBS0); 
		break; 
        case 2: 
		*UCSRC |= (1 << USBS0); 
		break; 
        default: 
		return; 
    } 
} 
  
// Limpia la pantalla de la terminal
void UART_clrscr(uint8_t com) { 
    UART_puts(com, "\x1B[2J"); 
    UART_puts(com, "\x1B[H"); 
} 
 
// Cambia color de texto 
void UART_setColor(uint8_t com, uint8_t color) { 
    char buffer[8]; 
    sprintf(buffer, "\x1B[%dm", 30 + color); 
    UART_puts(com, buffer); 
} 
 
// Mueve el cursor en pantalla de la terminal 
void UART_gotoxy(uint8_t com, uint8_t x, uint8_t y) { 
    char buffer[16]; 
    sprintf(buffer, "\x1B[%d;%dH", y, x); 
    UART_puts(com, buffer); 
} 
