#INCLUDE <18F4550.h>
#DEVICE ADC = 10
#FUSES NOPROTECT, NOWDT, NOLVP, HS, PLL2
#USE delay(clock = 8M)

#BYTE TRISA = 0xF92
#BIT  POT1 = 0xF80.0
#BIT  POT2 = 0xF80.1

#BYTE ADCON0 = 0xFC2
#BIT  DONE = 0xFC2.1
#BYTE ADCON1 = 0xFC1
#BYTE ADCON2 = 0xFC0
#BYTE ADRESH = 0xFC4
#BYTE ADRESL = 0xFC3

#BYTE TRISC = 0xF94
#BIT  CCP1 = 0xF82.2
#BIT  CCP2 = 0xF82.1

#BYTE CCP1CON = 0xFBD
#BIT  Bit0_CCP1 = 0xFBD.4
#BIT  Bit1_CCP1 = 0xFBD.5
#BYTE CCPR1L = 0xFBE

#BYTE CCP2CON = 0xFBA
#BIT  Bit0_CCP2 = 0xFBA.4
#BIT  Bit1_CCP2 = 0xFBA.5
#BYTE CCPR2L = 0xFBB

#BYTE TMR2 = 0xFCC
#BYTE PR2 = 0xFCB
#BYTE T2CON = 0xFCA

void main(){
   
   TRISA = 0x03;
   TRISC = 0x00;
   ADCON1 = 0b00001101;
   ADCON2 = 0b10001000;
   CCP1CON = 0b00001100;
   CCP2CON = 0b00001100;
   
   T2CON = 0b00000111;
   TMR2 = 0;
   PR2 = 30;
   
   int16 data1 = 0, data2 = 0, dpot1 = 0, dpot2 = 0;
   
   while(TRUE){
      ADCON0 = 0b00000001;
      delay_us(10);
      DONE = 1;
      while(DONE);
      data1 = ADRESH << 8;
      data1 = data1 + ADRESL;
      dpot1 = (int16)(data1*0.1213);
      CCPR1L = dpot1 >> 2;
      Bit0_CCP1 = dpot1;
      Bit1_CCP1 = dpot1 >> 1;
      
      ADCON0 = 0b00000101;
      delay_us(10);
      DONE = 1;
      while(DONE);
      data2 = ADRESH << 8;
      data2 = data2 + ADRESL;
      dpot2 = (int16)(data2*0.1213);
      CCPR2L = dpot2 >> 2;
      Bit0_CCP2 = dpot2;
      Bit1_CCP2 = dpot2 >> 1;
   }
   
}
