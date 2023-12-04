#include <18F4550.h>
#fuses NOPROTECT, NOWDT, INTRC // 8 MHz
#use delay(clock=8M) // 8 MHz
// REGISTROS PUERTOS
#BYTE TRISA=0xF92
#BYTE PORTA=0xF80
#BYTE TRISC=0xF94
#BYTE PORTC=0xF82

// REGISTROS CCP
#BYTE PR2=0xFCB
// REGISTROS CCP1CON
#BYTE CCPR1L=0xFBE
#BYTE CCP1CON=0xFBD
#BIT DC1B1=0xFBD.5
#BIT DC1B0=0xFBD.4
#BIT CCP1M3=0xFBD.3
#BIT CCP1M2=0xFBD.2
#BIT CCP1M1=0xFBD.1
#BIT CCP1M0=0xFBD.0
// REGISTROS CCP2CON
#BYTE CCPR2L=0xFBB
#BYTE CCP2CON=0xFBA
#BIT DC2B1=0xFBA.5
#BIT DC2B0=0xFBA.4
#BIT CCP2M3=0xFBA.3
#BIT CCP2M2=0xFBA.2
#BIT CCP2M1=0xFBA.1
#BIT CCP2M0=0xFBA.0

// REGISTROS TIMER2
#BYTE TMR2=0xFCC
#BYTE T2CON=0xFCA
#BIT TMR2ON=0xFCA.2

// REGISTROS ADC
#BYTE ADCON0=0xFC2
#BIT GO_DONE=0xFC2.1
#BYTE ADCON1=0xFC1
#BYTE ADCON2=0xFC0
#BYTE ADRESH=0xFC4
#BYTE ADRESL=0xFC3

int16 CCPX1L=0, CCPX2L=0;
int1 bit5=0, bit4=0;

int16 ADCAN0 (void)
{
   int16 v1=0;
   ADCON0=0b00000001;
   delay_us(100);
   GO_DONE=1;
   while(GO_DONE==1);   
   v1=ADRESH<<8;
   v1=v1+ADRESL;
   if(v1>=500)
   {
      v1=500;
   }
   return v1;
}

int16 ADCAN1 (void)
{
   int16 v2=0;
   ADCON0=0b00000101;
   delay_us(100);
   GO_DONE=1;
   while(GO_DONE==1);   
   v2=ADRESH<<8;
   v2=v2+ADRESL;
   if(v2>=500)
   {
      v2=500;
   }
   return v2;
}

void main()
{
   // Configuración ADC
   TRISA=0xFF;
   ADCON1=0b00001101;
   ADCON2=0b10001000;
   // CONFIGURACIÓN PR2, SALIDAS PUERTO C, T2CON Y LECTURAS ADC
   PR2=124;
   CCPX1L=ADCAN0();
   CCPR1L=CCPX1L>>2;
   CCPX2L=ADCAN1();
   CCPR2L=CCPX2L>>2;
   TRISC=0x00;
   T2CON=0b00000011;
   //CCP1CON BITS 5 Y 4
   bit5=(CCPX1L>>1)&(0b0000000001);
   bit4=(CCPX1L)&(0b0000000001);
   DC1B1=bit5;
   DC1B0=bit4;
   // CCP1CON CONFIGURACIÓN PWM
   CCP1M3=1;
   CCP1M2=1;
   CCP1M1=0;
   CCP1M0=0;
   // CCP2CON BITS 5 Y 4
   bit5=(CCPX2L>>1)&(0b0000000001);
   bit4=(CCPX2L)&(0b0000000001);
   DC2B1=bit5;
   DC2B0=bit4;
   // CCP2CON CONFIGURACIÓN PWM
   CCP2M3=1;
   CCP2M2=1;
   CCP2M1=0;
   CCP2M0=0;
   //TIMER2
   TMR2=0;   
   TMR2ON=1;  
   for(;;)
   {
      // CCPR1L Y CCPR2L - PARTE BAJA
      CCPX1L=ADCAN0();
      CCPR1L=CCPX1L>>2;
      CCPX2L=ADCAN1();
      CCPR2L=CCPX2L>>2;  
      //CCP1CON BITS 5 Y 4
      bit5=(CCPX1L>>1)&(0b0000000001);
      bit4=(CCPX1L)&(0b0000000001);
      DC1B1=bit5;
      DC1B0=bit4;
      // CCP2CON BITS 5 Y 4
      bit5=(CCPX2L>>1)&(0b0000000001);
      bit4=(CCPX2L)&(0b0000000001);
      DC2B1=bit5;
      DC2B0=bit4;
   }
}
