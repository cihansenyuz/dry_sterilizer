
#define gpioClockEnable (*((volatile unsigned long *) 0x40021018)) /* 18 added to adress to offset */
#define gpioA (*((volatile unsigned long *) 0x40010800))
#define gpioB (*((volatile unsigned long *) 0x40010C00))
#define gpioC (*((volatile unsigned long *) 0x40011000))


/* list of ports */
#define PA 1
#define PB 2
#define PC 3

/* list of directions */
#define IN 0
#define OUT10 1
#define OUT2 2
#define OUT50 3

/* list of options for input */
#define IN_AN 0 /* analog mode */
#define IN_F 1 /* floating input */
#define IN_PP 2 /*pull-up push-down */

/* list of options for output */
#define OUT_GP_PP 0 /* general purpose push-pull */
#define OUT_GP_OD 1 /* general purpose open-drain */
#define OUT_AF_PP 2 /* alternate func. push-pull */
#define OUT_AF_OD 3 /* alternate func. open-drain */

/* State definition */
#define LOW 0
#define HIGH 1

void gpio_init (unsigned short port, unsigned short pin, unsigned dir, unsigned short opt);

int read_GP (unsigned short port, unsigned short pin);

void write_GP (unsigned short port, unsigned short pin, unsigned short state);

void toggle_GP (unsigned short port, unsigned short pin);
