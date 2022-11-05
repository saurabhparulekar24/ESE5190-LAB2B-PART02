/*
*    Title: REPL for ESE5190
*    Author: Saurabh Parulekar, Amogh Gajare
*    Date: 03-Nov-2022
*    Code version: V1.0
*    Availability: 
*/
#include "hardware/pio.h"                                                               //Standard RP2040 library
#include "pico/stdlib.h"                                                                //Standard C library
#include <stdio.h>                                                                      //Standard C library
#include "stdlib.h"                                                                     //Standard C library
#include "hardware/gpio.h"                                                              //Standard RP2040 library
#include "hardware/structs/sio.h"                                                       //Standard RP2040 library
#include "sio_registers.h"                                                              //Custom Register Read library
#include "button_capture.h"                                                             //Custom BOOT button Capture library
#include "data_transfer.h"                                                              //Custom Data Transfer library



int main(){
    stdio_init_all();                                                                   //Standard Library Initialization
    VALUE a;                                                                            //Read Register Variable Declaration
    long int command;                                                                   //Command Variable Declaration
    long int value;                                                                     //Value Variable Declaration
    char operation;                                                                     //REPL operatoin Variable Declaration
    uint gpio;                                                                          //GPIO operatoin Variable Declaration
    int gpio_val;                                                                       //GPIO Value Variable Declaration
    int mode;                                                                           //Speed Variable Declaration
    char color;                                                                         //Neopixel Color Variable Declaration
    bool recording[5001] ={0};                                                          //5sec recording of BOOT button storage Variable Declaration
    printf("Waiting for USB\n");                                                        //Waiting for USB Connection
    while (!stdio_usb_connected()) {
      printf(".");
      sleep_ms(500);
    }
    printf("\nUSB host detected!\n");
    printf("\nWelcome to REPL Console, press 'h' for help\n");                          //REPL Intro Message
    while(true){
        printf("\nSA>>>");                                                              //Command Line for REPL(SA Stands for Saurabh and Amogh)
        sleep_ms(10);
        scanf(" %c",&operation);                                                        //Reading Character from Terminal, to perform the particular Action
        switch(operation){                                                              //Switch Case for execution of requested operation
            case 'r':                                                                   //Read Mode Case
                printf("\nRead Mode Entered, Enter Address:");
                sleep_ms(10);
                scanf(" %x",&command);                                                  //Reading Address from terminal                                                
                a = register_read((ADDRESS)command);                                    //Reading the value from the requested Register
                printf("\n%x\n",a);                                                     //Printing the value of the register
                break;                                                                  //you should always put a break, statement after each case, otherwise, the code moves on to other case statements too
            case 'w':                                                                   //Write Mode Case
                printf("\nWrite Mode Entered, Enter Address:");
                sleep_ms(10);                                                           
                scanf(" %x",&command);                                                  //Reading address from the terminal
                printf("\nWrite Mode Entered, Enter Value to be Set:");                 
                sleep_ms(10);                                                   
                scanf(" %x",&value);                                                    //Reading value to be set in the register from terminal
                register_write((ADDRESS)command,(VALUE)value);                          //Writing the value to the register address
                break;
            case 'g':                                                                   //GPIO Mode Case
                printf("\nGPIO Mode Entered, Enter GPIO PIN Number:");
                sleep_ms(10);
                scanf(" %d",&gpio);
                uint32_t mask = 1ul << gpio;                                           //Creating a Mask by shifting the 1 to the left by the number of GPIO,eg GPIO=22, shifting 1, 22 times to left: 1<<22 = 0x200000, info: shifting left results in multiplication of a number, shifting right in division
                printf("gpio %d",gpio);
                gpio_init(gpio);                                                       //Initializing the GPIO
                sio_hw->gpio_oe_set = mask;                                            //Setting the direction of GPIO(output)
                for(int k=0;k<3;k++){                                                  //Allows setting the value of GPIO thrice
                    printf("\nGPIO Mode Entered, Enter Value:");
                    sleep_ms(10);
                    scanf("%d",&gpio_val);
                    if(gpio_val == 1){
                        sio_hw->gpio_set = mask;                                       //Setting value of the GPIO i.e 1
                        printf("\nValue Set to %d",gpio_val);}
                    else{
                        sio_hw->gpio_clr = mask;                                       //Clearing value of the GPIO i.e 0
                        printf("\nValue Set to %d",gpio_val);}
                }
                break;
            case 'a':                                                                  //Record or Acquire Mode Case
                printf("Record Mode Entered\n");
                sleep_ms(10);
                printf("Record Mode Entered, Recording in Progress\n");
                record(recording,5);                                                   //Calling Record Function, you can trace this to button_capture.c
                printf("\nRecord Mode Entered, Record Ended\n");
                break;
            case 'p':                                                                  //Play Mode Case
                printf("Play Mode Entered\n");
                sleep_ms(10);
                printf("Play Mode Entered, Enter Speed:\n");
                scanf("%d",&mode);                                                     //Setting the Speed of Replay(-4:4x Slower,-2:2x Slower,0:Origianl Speed,2:2x Faster,4:4x Faster)
                printf("Play Mode Entered, Enter Color:\n");
                scanf(" %c",&color);                                                   //Setting Color of Neopixel(r/g/b)
                printf("Play Mode Entered,Replay in Progress:\n");
                play(recording,5,mode,color);                                          //Calling Play Function, you can trace this to button_capture.c
                printf("Play Mode Entered, Replay Ended:\n");
                break;
            case 's':                                                                  //Save Mode Case
                printf("Save Mode Entered\n");
                sleep_ms(10);
                encode(recording);                                                     //Calling Encode Function, you can trace this to data_transfer.c
                sleep_ms(20);
                printf("Save Mode Entered, Recording Saved\n");
                break;
            case 'u':                                                                  //Upload Mode Case
                printf("Upload Mode Entered\n");
                char upload[1000];
                sleep_ms(10);
                printf("Upload Mode Entered, Paste the Recording\n");
                scanf(" %s",&upload);                                                  //Taking Input of the recording
                printf("%s",upload);
                decode(recording,upload);                                              //Calling Encode Function, you can trace this to data_transfer.c
                printf("Upload Mode Entered, Recording Uploaded\n");
                break;
            case 'h':                                                                  //Help Mode Case
                printf("Welcome to REPL Console\nThe REPL supports the following commands\n");
                printf("'r':Read a value from any address\n'w':Write a value to any address\n'g':Write values to any GPIO\n'a':Start Recording BOOT button presses(5 secs)\n'p':Play the recording\n's':Save the recording to your PC\n'u':Upload a recording from your PC\n");
                break;
            default:
                printf("Invalid Commmand\n");
        

        }
        sleep_ms(10);

    }
    return 0;
}