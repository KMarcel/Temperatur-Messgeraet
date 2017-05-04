/*************************************************************************
 Title	:   Temperatur Messgerät
 Author:    Marrcel Kühl <Marcel.Kuehl@web.de>  https://github.com/KMarcel
 File:	    Temperatur
 Software:  Arduino IDE
 Hardware:  Arduino Serie 
***************************************************************************/
// Die Standart Librarys Eingebunden. Diese sind für verschieden Typen wie bool unerlässlig
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>		//Delay Funktionen werden über diese Library eingebunden

// Diese Library ist für das LC Display HD44780 zuständig und gibt
// uns Funktionen für das Ansteuern dieses LC Displays
#include "lcd_pollin.h"

// Map ist eine Umwandlung von einer Größe in eine Andere Größe 

// Parameter
long map(
int x, 			//	= 	einen Wert den wir umgewandelt haben wollen (Diese Sollte bitte im Eingangsgröße bereich liegen) 
int in_min, 	// 	=	Kleinste Eingangsgröße
int in_max, 	//	=	Größte Eingangsgröße
int out_min, 	//	=	Kleinste Ausgangsgröße
int out_max) 	//	=	Größte Ausgangsgröße
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// In dieser Funktionen wird am A0 eingang gemessen. Desweiteren wird der Wert in ADU_Wert gespeichert und über die Funktion map
// Auf 0 bis 100 geändert dieses wird in der Variable Spannung gespeichert und dann zurückgegeben
int Messung()
{
	int Spannung;
	uint16_t ADU_Wert;
	ADCSRA = ADCSRA | 0b01000000;
	while ((ADCSRA & 0b01000000) != 0){}
	ADU_Wert = ADC;
	Spannung = map(ADU_Wert, 500, 750, 0, 100);
	return Spannung;
}
/** Inistitialisierung der einzelnen Register und des LC Displays **/
void Init_LCD(void)					// Initialisiert das LCD
{
	lcd_init(LCD_DISP_ON);
}

void Init_Pollin(void)
{
	DDRD = 0b11100000;
}

void Init_ADU(void)
{
	ADMUX = 0b01000000;
	ADCSRA = 0b10000100;
}

void Init(void)
{
	Init_Pollin();
	Init_ADU();
	Init_LCD();
}

/*****************************************************************/
// Hauptfunktion
int main(void)
{
// in der Messung1 wird unser gemessenes Signal das in Grad Celsius umgewandelt worden ist gespeichert.
	int Messung1 = 0; 
// puffer dient als Ausgabe Variable auf dem Display
	char puffer[10];
	Init();	
    while(1)
    {	
// Speicherung des Grad Celsius in der Variable Messung1
		Messung1 = Messung();
// Umwandlung von Int zu Char Array
		itoa(Messung1,puffer,8);
		lcd_gotoxy(0,1);
		lcd_puts("Temperatur: ");
// Ausgabe der Temperatur
		lcd_puts(puffer);
		lcd_command(0b11011111);
		_delay_ms(10);
		lcd_clrscr();
			
    }
}
