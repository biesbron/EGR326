/****************************************************************
  * Lab One:     Interfacing a keypad and rotary encoder with the
  *              MSP432
  * Author:      Kasonde Musonda
  * Date:        10/02/2020
  * Professor:   Dr. Brian Krug
  * Description: To develop a C-Program that detects and decodes
  *              a key pressed on the keypad
  ****************************************************************/
 #include "msp.h"
 #include <stdio.h>

 #define KEYPAD P4   /*Define Macros*/
 #define BANK 0x7F

 volatile uint8_t flag=0; //Flag for when a button is pressed

 /*Function prototype*/
 void setPins(void);
 int  Read_Keypad(void);
 void userPrompt(void);
 void delay_Micro(int micro);

 /*
  * Pin Declaration and Instantiation
  * @Param:void
  * @Return:void
  */
 void setPins(){

     KEYPAD->SEL0 &= ~BANK;     //sets all buttons from keypad to input.
     KEYPAD->SEL1 &= ~BANK;
     KEYPAD->DIR &= ~BANK;
     KEYPAD->REN |=  BANK;
     KEYPAD->OUT |=  BANK;

     SysTick -> CTRL = 0;      //Disable systick during step
     SysTick -> LOAD = 0x00FFFFFF;   //max reload value
     SysTick -> VAL = 0; //clears it
     SysTick -> CTRL = 0x00000005;   //enables systick 3MHz no interrupts
 }

 /*
  This function checks for a button press on the keypad
  and returns an integer to the main function
  to which button was pressed.
  @Return: uint8_t
  @Param: void
 */
 int Read_Keypad(void){

 uint8_t i = 0; //variable initialization
 int keyPress = 0;

 for(i=0; i<=2; i++){ //cycles through columns on keypad (code from lecture slides)

    KEYPAD->DIR |= (0x01 << i);
    KEYPAD->OUT &= ~(0x01 << i);

   delay_Micro(10);


  if(!((KEYPAD->IN & BIT3)==BIT3)){  //checks if button in row 0 is pressed

      flag=1; //Sets flag to indicate a pushbutton press.

      if(i==0){keyPress = 1;}
      if(i==1){keyPress = 2;}
      if(i==2){keyPress = 3;}

      while(!((KEYPAD->IN & BIT3)==BIT3)){} //Program halt-prevents multiple pushes.
  }

  if(!((KEYPAD->IN & BIT4)==BIT4)){     //Checks row 2 for button push.

       flag=1;

       if(i==0){keyPress = 4;}
       if(i==1){keyPress = 5;}
       if(i==2){keyPress = 6;}

       while(!((KEYPAD->IN & BIT4)==BIT4)){} //Empty loop to for button press and hold.
  }
  if(!((KEYPAD->IN & BIT5)==BIT5)){ //Checks Row 3 for button push.

       flag=1;    //Flag set.

       if(i==0){keyPress = 7;}
       if(i==1){keyPress = 8;}
       if(i==2){keyPress = 9;}

       while(!((KEYPAD->IN & BIT5)==BIT5)){}
      }
  if(!((KEYPAD->IN & BIT6)==BIT6)){ //Check row 4 for button push.

       flag=1;  //Flag set

       if(i==0){keyPress = 11;}
       if(i==1){keyPress = 0;}
       if(i==2){keyPress = 12;}

       while(!((KEYPAD->IN & BIT6)==BIT6)){}
       }
      KEYPAD->DIR &= ~(0x01 << i); //turns columns back to high input state
      KEYPAD->OUT |= (0x01 << i);
  }

   if(flag==1 && keyPress ==11){
       keyPress='*';
       printf("%c\n",keyPress);
   }else if(flag==1 && keyPress==12){
       keyPress='#';
       printf("%c\n",keyPress);
   }else if(flag==1){
       printf("%d\n",keyPress);
   }

   flag=0; //Resets flag.
   return keyPress;
}



 /*
  * Prompts user to press a key- prints to screen via
  * printf
  * @Param:void
  * @return:void
  */
 void userPrompt(){printf("Press key on keypad\n");}

 /*
  * Delay in milliseconds using SysTick Timer
  */
 void delay_Micro(int micro){
     SysTick->LOAD = (3*micro)-1;
     SysTick->VAL = 0;
     while((SysTick->CTRL & 0x10000)==0){}
 }

 /*
  * Main Method
  */
 void main(void){

     WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // stop watchdog timer

      setPins();
      userPrompt();

      while(1){
          Read_Keypad();
      }
 }
