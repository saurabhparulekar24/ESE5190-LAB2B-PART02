#include "pico/stdlib.h"
#include <stdio.h>
#include "stdlib.h"
#include "hardware/gpio.h"
#include "sio_registers.h"
#include "pixel.h"
#include "hardware/pio.h"
#include <math.h>


void record(bool arr[],int seconds)
{
    pixel_init();
    stdio_init_all();                                           //Initialization functions
    VALUE a;
    int cnt=0;
while(true){   
        pixel_set(0x000000ff);
        a = register_read((ADDRESS)0xd0000004);                 //Reading the register with BOOT button status

        if(!(1<<21 & a)){                                       // If BOOT button is pressed
            arr[cnt]=true;
            pixel_set(0x00ff00ff);
            sleep_ms(5);
        }
        else{                                                   // If BOOT button is not pressed
        arr[cnt]=false;
        sleep_ms(5);
        }
        cnt=cnt+1;
        if(cnt>=((seconds*1000)-2))
        {
            cnt=0;
            pixel_set(0x00000000);
            break;
        }
}
}


void play(bool arr[], int seconds, int sdir, char color)
{
  pixel_init();
  stdio_init_all();                                             //Initialization functions
     /*
    Mode 0 - Normal Speed
    Mode 1 - 2x Slower Speed
    Mode 2 - 4x Slower Speed
    Mode 3 - 2x Faster Speed
    Mode 4 - 4x Faster Speed
    */
    uint32_t light_color;

    switch(color){
            case 'r':
                light_color = 0x00ff0000;
                break;
            case 'g':
                light_color = 0x0000ff00;
                break;
            case 'b':
                light_color = 0x000000ff;
                break;
                default : 
                light_color = 0x000000ff;
    
    }

    int div;
    int speed;                                                 //speedx slower, if speed=4, it will play 4x slower
    switch(sdir){
      case 0  : //Mode 0
      speed = 1;
      div=1;
      break; 
	
      case -2  : //Mode 1
      speed = 2;
      div=1;
      break; 

      case -4  : //Mode 2
      speed = 4;
      div=1;
      break; 

      case 2  : //Mode 3
      speed = 1;
      div=2;
      break; 

      case 4  : //Mode 4
      speed = 1;
      div=4;
      break; 
  
   default : 
    speed = 1;
    div=1;

    }

for(int j=0;j<=seconds*1000;j=j+1)                              //Replaying the Recording
            {
                if(arr[j]==true)
                {
                    pixel_set(light_color);
                    sleep_ms(floor((5*speed)/div));
                }

                else
                {
                    pixel_set(0x00000000);
                    sleep_ms(floor((5*speed)/div));
                }
            }

pixel_set(0x00000000);
}