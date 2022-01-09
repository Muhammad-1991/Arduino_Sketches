/*
  Watchdog Timer(WDT)

  This sketch configures the watch dog timer control status register(WDTCSR) and resets the arduino-uno/nano
  after every 8 seconds. This reset is inevatible when MCU gets stuck in one operation/loop.
  In short, WDT provides gives you ensurance that your arduino is doing desired work all the
  time when powered. 

  The Details:
    - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

  The circuit:
    - No external components required.
    - Demonstrates working using only onboard LED(Pin#13).

  Created 31 Oct 2021
  by Muhammad Irfan


  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Ping
*/

#include <avr/wdt.h>

void setup()
  {
  //_________________________________________________________________ 
  Serial.begin(9600);                 //  =   These lines are put 
  Serial.println("Turning_On MCU");   //   |  to inform you that MCU is 
  delay(500);                         //  =   turning on after reset.
  //_________________________________________________________________
  
  cli();                             // Disable global(all) interrupts 
  wdt_reset();                       // Reset WDT timer
 
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  /*
  WDTCSR CONIGURATION:
  WDCE = 1;   :WD Change Enable
  WDE  = 1    :Reset Enable
  
  WDP3 = 1    ////
  WDP2 = 0      //   Set the four prescaler bits
  WDP1 = 0      //   for a 4sec timeout,
  WDP0 = 0    ////
  
  WDIE = 1;  Iterrupt Enable
  */
   
  //////////////////////*Note: The order of commands in this portion of sketch is very important. //////////////////////////////////
  MCUSR  &= ~(1 << WDRF);                                                         // Clear the Watch_Dog_Reset_Flag(WDRF) bit of MCUSR.      //// 
  WDTCSR |=  (1 << WDCE) | (1 << WDE);                                            // Set WD_change enable to make changes to WDTCSR          ////                                                                                 // Enable WD timer                                         ////
  WDTCSR  =  (1 << WDE ) | (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0); // Set watchdog timeout value (4 Seconds in this sketch)   ////
  WDTCSR |=  (1 << WDIE);                                                         // Enable WD interupts                                     ////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  sei();                                     // Enable global(all) interrupts 
} 

void loop()
{
  //_____________Put Main code here______________//
  
  //////////////Forever loop to simulate MCU stuck condition(s).////////////
  Serial.println("Timing_Down");                                      //////
  while(1)                                                            //////
    {                                                                 //////
      Serial.println(millis());                                       //////
      delay(500);                                                     //////
                                                                      //////
      if(0)      //Set if condition true i.e. (if(1)) to simulate     //////
        {        //WDT when wdt is resets before it overflows         //////                                             
          Serial.println("Loop_Break!");                              //////
          break;                                                      //////
        }                                                             //////
                                                                      //////
    }                                                                 //////
                                                                      ////// 
  wdt_reset();     // If Forever loop breaks before WDT times out     //////
  //////////////////////////////////////////////////////////////////////////
 
}

ISR(WDT_vect) // Watchdog timer interrupt
{
  // do somthing befor reset here
  digitalWrite(13, LOW); 
  Serial.println("Good_Bye!"); // MCU going to reset.
  //_____Note*: Just for Debug and demo purposes practically use of Serial.print_____//
  //____________________________is discouraged in ISR________________________________//
}
