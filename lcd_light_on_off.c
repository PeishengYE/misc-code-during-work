#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <getopt.h>            

#include <fcntl.h>             
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>         
#include <linux/videodev2.h>
#include <time.h>
#include <linux/fb.h>
enum tag_DISP_CMD{

    DISP_CMD_LCD_ON = 0x140,
    DISP_CMD_LCD_OFF = 0x141,
    DISP_CMD_LCD_SET_BRIGHTNESS = 0x142,
    DISP_CMD_LCD_ON_CONFIRM = 0x14f,

    DISP_CMD_PWM_SET_PARA = 0x300,
    DISP_CMD_PWM_GET_PARA = 0x301

}__disp_cmd_t;


typedef struct
{
    /*
    __bool enable;
    __u32 active_state;
    __u32 duty_ns;
    __u32 period_ns;
    */


    signed char enable;
    unsigned int active_state;
    unsigned int duty_ns;
    unsigned int period_ns;

}__pwm_info_t;

int disp_on()
{
	int disphd;
	int arg[4];

	if((disphd = open("/dev/disp",O_RDWR)) == -1)
	{
		printf("open file /dev/disp fail. \n");
		return -1;
	}

    arg[0] = 0;
	printf("enable the backlight \n");
    ioctl(disphd, DISP_CMD_LCD_ON_CONFIRM, (void*)arg);
    ioctl(disphd, DISP_CMD_LCD_ON, (void*)arg);
	close(disphd);
	return 0;
}

int disp_off()
{
	int disphd;
	int arg[4];

	if((disphd = open("/dev/disp",O_RDWR)) == -1)
	{
		printf("open file /dev/disp fail. \n");
		return -1;
	}

    arg[0] = 0;
	printf("disable the backlight \n");
    ioctl(disphd, DISP_CMD_LCD_OFF, (void*)arg);
	close(disphd);
	return 0;
}

static int rgb_to_brightness(int color_input)
{
	int color = color_input & 0x00ffffff;
	return ((77 * ((color >> 16) & 0x00ff))
		+ (150 * ((color >> 8) & 0x00ff)) +
		(29 * (color & 0x00ff))) >> 8;
}


/*
 * this call only accept 0~255 
 * */

static int set_light_backlight(int brightness )
{

    int err = 0;
	int disphd;
    unsigned long  args[3];

	if((disphd = open("/dev/disp",O_RDWR)) == -1)
	{
		printf("open file /dev/disp fail. \n");
		return -1;
	}

	args[0]  = 0;
	args[1]  = brightness;
	args[2]  = 0;
	err = ioctl(disphd,DISP_CMD_LCD_SET_BRIGHTNESS,args);
    if(err < 0 ){
		printf("set Brightness error \n");
    }else{

		printf("set Brightness successfully \n");
    }

	close(disphd);
    return err;
}

static int get_pwm_parameter( )
{

    int err = 0;
	int disphd;
    unsigned long  args[3];
    __pwm_info_t pwm_info;

	args[0]  = 0;
	args[1]  = (unsigned int)&pwm_info;
	args[2]  = 0;


	if((disphd = open("/dev/disp",O_RDWR)) == -1)
	{
		printf("open file /dev/disp fail. \n");
		return -1;
	}

    memset(&pwm_info, 0, sizeof(__pwm_info_t));

	err = ioctl(disphd, DISP_CMD_PWM_GET_PARA, args);
    if(err < 0 ){
		printf("get_pwm_parameter()>> failed \n");
    }else{

		printf("get_pwm_parameter()>> successfully \n");

        /*
    __bool enable;
    __u32 active_state;
    __u32 duty_ns;
    */
        

	    printf(" pwm_info.enable = %d\n", pwm_info.enable);
	    printf(" pwm_info.active_state = %d\n", pwm_info.active_state);
	    printf(" pwm_info.duty_ns = %d ns\n", pwm_info.duty_ns);
	    printf(" pwm_info.period_ns = %d ns\n", pwm_info.period_ns);
    }


	close(disphd);
    return err;
}






int
main (int argc, char **argv)
{
  int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int index;
  int brightness = 0;
  int c;
  int color_input;

  opterr = 0;
  fprintf (stderr, "Option a for enable backlight, b for disable backlight, c for brightness (1-255)\n");
  fprintf (stderr, "./backlight_controller -a -c255\n");
  fprintf (stderr, "./backlight_controller -a -c5\n");
  fprintf (stderr, "./backlight_controller -b \n\n\n");
  while ((c = getopt (argc, argv, "abc:")) != -1)
    switch (c)
      {
      case 'a':
        aflag = 1;
        break;
      case 'b':
        bflag = 1;
        break;
      case 'c':
        cvalue = optarg;
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }
  printf ("aflag = %d, bflag = %d, cvalue = %s\n",
          aflag, bflag, cvalue);

  if(aflag == 1){
     disp_on();
  }

  if(bflag == 1){
     disp_off();
  }

  if(cvalue != NULL){
    brightness = atoi(cvalue);
    printf ("brightness:  %d\n", brightness);
    
    set_light_backlight( brightness );
    get_pwm_parameter();

    /*
    for(color_input = 0; color_input < 0xffffff ; color_input += 0x20)
     printf("brightness: %d -> %d\n" , color_input,  rgb_to_brightness( color_input));
     */
  }

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  return 0;
}

