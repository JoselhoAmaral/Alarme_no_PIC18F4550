#include <18F4550.h>        //201910450.70147
#include <stdio.h>

#fuses HS, NOWDT, PUT, BROWNOUT, NOLVP, CPUDIV1,PLL5,USBDIV //Configuração dos fusíveis
#use delay(clock=20000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)

//Colunas
#define COL1   pin_b0
#define COL2   pin_b1
#define COL3   pin_b2
//Linhas
#define LIN1   pin_d0
#define LIN2   pin_d1
#define LIN3   pin_d2
#define LIN4   pin_d3

const int32 time = 3036;   //ajuste tempo de varredura das teclas
const float time_repete = 36;   //ajuste tempo de repetição das teclas

#int_timer1

void funcao_colocar_senha(){
   set_timer1(time + get_timer1());
   
   static signed int32 n = 0;
   static int senha[5] = {7,0,1,4,7}; // Esta é a senha, ela pode ser mudada para qualquer outra.
   static boolean entrar = 0;
   static boolean corrigir = 0;
   static boolean mensagem = 0;
   static int contrasenha[5];
   int *ponteiro;
   static int cont, verificacao;

// Atribui o endereço do vetor ao ponteiro
   ponteiro = &contrasenha[cont];
   
   
   if (mensagem == 0){ // Mensagem inicial do programa
      printf("Por favor, digite sua senha:\n--> ");
      mensagem = 1;
   }
   
   n++;
   
   if (n > 4) {n=0;}
   
   switch(n){
      case 1:
         output_bit(LIN1,0); 
         output_bit(LIN2,1); 
         output_bit(LIN3,1); 
         output_bit(LIN4,1); 
         if (!input(COL1)) {printf("1"); n=-1*(time_repete); *ponteiro = 1; cont++;}
         if (!input(COL2)) {printf("2"); n=-1*(time_repete); *ponteiro = 2; cont++;}
         if (!input(COL3)) {printf("3"); n=-1*(time_repete); *ponteiro = 3; cont++;}
      break;
   
      case 2:
         output_bit(LIN1,1); 
         output_bit(LIN2,0); 
         output_bit(LIN3,1); 
         output_bit(LIN4,1);  
         if (!input(COL1)) {printf("4"); n=-1*(time_repete); *ponteiro = 4; cont++;}
         if (!input(COL2)) {printf("5"); n=-1*(time_repete); *ponteiro = 5; cont++;}
         if (!input(COL3)) {printf("6"); n=-1*(time_repete); *ponteiro = 6; cont++;}
      break;
      
      case 3:
         output_bit(LIN1,1); 
         output_bit(LIN2,1); 
         output_bit(LIN3,0); 
         output_bit(LIN4,1);  
         if (!input(COL1)) {printf("7"); n=-1*(time_repete); *ponteiro = 7; cont++;}
         if (!input(COL2)) {printf("8"); n=-1*(time_repete); *ponteiro = 8; cont++;}
         if (!input(COL3)) {printf("9"); n=-1*(time_repete); *ponteiro = 9; cont++;}
      break;
      
      case 4:
         output_bit(LIN1,1); 
         output_bit(LIN2,1); 
         output_bit(LIN3,1); 
         output_bit(LIN4,0);  
         if (!input(COL1)) {printf("\n\t'*'\n"); n=-1*(time_repete); corrigir = 1;}
         if (!input(COL2)) {printf("0"); n=-1*(time_repete); *ponteiro = 0; cont++;}
         if (!input(COL3)) {printf("\n\t'#'"); n=-1*(time_repete); entrar = 1;}
      break;
      }
      
      //Ao apertar '*', a senha é resetada para tentar novamente....
      if (corrigir == 1){         
         // resetando vetor
         for(cont = 0;cont < 5; cont++){
         contrasenha[cont] = 0;
         }
         
         mensagem = 0;
         corrigir = 0;
         cont = 0;
      }
      
      // comparativo de senha...
      if (entrar == 1){ // se a senha estiver certa
      
      for(cont = 0;cont < 5; cont++){
         if(senha[cont] == contrasenha[cont]){
         verificacao++;
         contrasenha[cont] = 0; // resetando vetor
         }
      }
         if(verificacao == 5){
         // Se a senha estiver certa...
            printf("\nLed verde ligado!\n");
            output_high(pin_d6); //liga led verde
            
            output_low(pin_d7); //desliga led vermelho
            printf("Led vermelho desligado!\n");
            
            delay_ms(5000); //delay 5 segundos
            
            printf("Led verde desligado!\n\n");
            output_low(pin_d6); //desliga led verde

            entrar = 0;
            mensagem = 0;
         }
         else{ // se a senha estiver errada...
            printf("\nSenha incorreta!\nLed vermelho ligado!\n\n");
            output_high(pin_d7); //liga led vermelho
            
            entrar = 0;
            mensagem = 0;
         }
      cont = 0;
      verificacao = 0;
      }
}//fim função

void main()
{
   port_b_pullups(TRUE);
   
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_timer1);
   
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
   
   while(true)
   { }
}
