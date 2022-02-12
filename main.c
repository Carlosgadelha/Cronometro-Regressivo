/*
 * File:   main.c
 * Author: Carlos Gadelha -389110
 *
 * Created on 10 de Janeiro de 2022, 23:48
 */

#include <xc.h>
#define _XTAL_FREQ 4000000

#pragma config FOSC = XT_XT     // Oscillator Selection bits (XT oscillator (XT))
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)


unsigned char unidade,dezena;
int count = 0;
int buzz = 0;

void Apito(){
  INTCONbits.GIE = 1; //Habilita interrupcoes globalmente
  INTCONbits.PEIE = 1; //Habilita interrupcoes dos perifericos
  INTCONbits.INT0IE = 1; //Habilita interrupcao no INT0
  INTCON3bits.INT1IE = 1; //Habilita interrupcao no INT1
  
  buzz = 1;
    
  while(1){
    if(!buzz) break;
    PORTCbits.RC0 = 1;
      
    if(!buzz) break;
    __delay_ms(250);
        
    if(!buzz) break;
    PORTCbits.RC0 = 0;
        
    if(!buzz) break;
    __delay_ms(500);
       
    if(!buzz) break;
  }
}
void Contar(){
  if(dezena != 0 || unidade != 0 ){
      
      // aplica um delay total de 1 minuto
    __delay_ms(10000); //10s 
    __delay_ms(10000); //10s 
    __delay_ms(10000); //10s 
    __delay_ms(10000); //10s 
    __delay_ms(10000); //10s 
    __delay_ms(10000); //10s 
    
    unidade--;
    
    if (unidade == 0xff){
        
        unidade = 9;
        dezena--;
       
        if (dezena == 0xff){
          dezena = 0;   
        }   
    }
  }else{
    Apito();
  }  
}

void atualiza_PORTD() {
  PORTD = (dezena << 4) | unidade;
}

void main() {
    //Inicialização
    TRISBbits.RB0 = 1; //RB0 e RB1 seta como entrada
    TRISBbits.RB1 = 1;
    TRISCbits.RC0 = 0;//RC0 seta como saida
    TRISD = 0;  //PORTD como saida
    
    INTCONbits.INT0IE = 1; //Habilita interrupcao no INT0
    INTCON3bits.INT1IE = 1; //Habilita interrupcao no INT1
    INTCONbits.GIE = 1; //Habilita interrupcoes globalmente
    INTCONbits.PEIE = 1; //Habilita interrupca dos perifÃ©ricos    
    
    unidade = 0;
    dezena = 1;
    
    PORTD = 0;
    count = 0; 
    
    while(1) {   
     if (count){
        Contar();
     }
     atualiza_PORTD();
    __delay_ms(500);
    }
}

void __interrupt() isr(void) {
  if (INT0IF && buzz == 1){
    INT0IF = 0;  
    buzz = 0;
    PORTCbits.RC0 = 0;
    
    count = 0;
    unidade = 0;
    dezena = 1; 
  }
  
  else if (INT1IF && buzz == 1){
    INT1IF = 0;
    buzz = 0;  
    PORTCbits.RC0 = 0;
    
    count = 0;
    unidade = 0;
    dezena = 1; 
  }

  else if (INT0IF && buzz == 0){//Botao do INT0
    INT0IF = 0;
    dezena++;//incremento mais 10min  
    
    if(dezena > 6){
      dezena = 1;
    }    
  }
  
  else if (INT1IF && buzz == 0){//Botao do INT1  
    INT1IF = 0;
    count ^= 1;

    INTCONbits.INT0IE = 0; // Habilita interrupcao INT0
    INTCON3bits.INT1IE = 0; // Habilita interrupcao INT1
    INTCONbits.GIE = 0; // Desabilita interrupcoes globalmente
    INTCONbits.PEIE = 0;// Desabilita interrupcao dos perifericos
  }
}



