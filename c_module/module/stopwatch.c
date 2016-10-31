#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <linux/platform_device.h>
#include <asm/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <linux/version.h>

//////
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/version.h>

#include <linux/timer.h> 
//////

// 통합 device driver
#define DEV_MAJOR 245
#define DEV_MINOR 0
#define DEV_NAME "stopwatch"

//#include "./fpga_dot_font.h"
// fnd
#define FND_MAJOR 241           // fnd device minor number
#define FND_MINOR 0         // fnd device minor number
#define FND_NAME "fnd_driver"   // fnd device name
#define FND_GPL2CON 0x11000100  // Pin Configuration
#define FND_GPL2DAT 0x11000104  // Pin Data
#define FND_GPE3CON 0x11400140  // Pin Configuration
#define FND_GPE3DAT 0x11400144  // Pin DAta

#define LED_MAJOR 240          // 디바이스 드라이버의 주번호
#define LED_MINOR 0             // 디바이스 드라이버의 부번호
#define LED_NAME "led_driver"       // 디바이스 드라이버의 이름
#define LED_GPBCON 0x11400040   
#define LED_GPBDAT 0x11400044


// interrupt: INT
#define INT_MAJOR 246
#define INT_NAME "inter"

//fpga
#define IOM_BUZZER_ADDRESS 0x04000020 
#define IOM_FPGA_STEP_MOTOR_ADDRESS 0x0400000C 
#define IOM_FPGA_DOT_ADDRESS 0x04000210 
#define IOM_FPGA_DEMO_ADDRESS 0x04000300 
#define IOM_FPGA_TEXT_LCD_ADDRESS 0x04000100 
#define IOM_LED_ADDRESS 0x04000016 
#define IOM_DEMO_ADDRESS 0x04000300
#define IOM_FND_ADDRESS 0x04000004

//major
#define IOM_LED_MAJOR 260		// ioboard led device major number
#define IOM_LED_NAME "fpga_led"		// ioboard led device name
#define IOM_FND_MAJOR 261		// ioboard fpga device major number
#define IOM_FND_NAME "fpga_fnd"		// ioboard fpga device name

#define IOM_FPGA_TEXT_LCD_MAJOR 263		// ioboard led device major number
#define IOM_FPGA_TEXT_LCD_NAME "fpga_text_lcd"		// ioboard led device name

#define UON 0x00 
#define UOFF 0x01 

////////////////////////////////////////////////////////
/* function declaration */
//  fnd_driver.c

//ssize_t fnd_write(/*struct file *inode,*/ const short *gdata/*, size_t length, loff_t *off_what*/);
ssize_t fnd_write(unsigned short);	//gpio fnd 
int fnd_init(void);
void fnd_exit(void);
ssize_t led_write(void); 	//gpio led
int led_init(void);
void led_exit(void);



//fpga 
//  fpga_led_driver
ssize_t iom_led_write(int/*unsigned short*/);
int iom_led_init(void);
void iom_led_exit(void); 
//  fpga_fnd_driver
ssize_t iom_fpga_fnd_write(unsigned short*);
int iom_fpga_fnd_init(void);
void iom_fpga_fnd_exit(void); 
//  fpga_dot_driver
ssize_t iom_fpga_dot_write(int mode);
int iom_fpga_dot_init(void);
void iom_fpga_dot_exit(void);
//  fpga_text_lcd_driver
ssize_t iom_fpga_text_lcd_write(void);
int iom_fpga_text_lcd_init(void);
void iom_fpga_text_lcd_exit(void); 
void set_text(int mode);
//	fpga buzzer
void buzzer_driver_write(void);
void buzzer_driver_init(void);
void buzzer_driver_exit(void);

//	fpga step motor
void step_motor_driver_write(int mode);
void step_motor_driver_init(void);
void step_motor_driver_exit(void);


/* global variable */
//  fnd_driver.c
static unsigned char *fnd_data;
static unsigned int *fnd_ctrl;
static unsigned char *fnd_data2;
static unsigned int *fnd_ctrl2;
static unsigned char *led_data;
static unsigned int *led_ctrl;
static char *buffer = NULL;

// fpga
//  fpga_led_driver.c
static unsigned char *iom_fpga_led_addr;
static unsigned char *iom_led_demo_addr;
//  fpga_fnd_driver.c
static unsigned char *iom_fpga_fnd_addr;
static unsigned char *iom_fnd_demo_addr;
//  fpga_dot_driver.c
static unsigned char *iom_fpga_dot_addr;
static unsigned char *iom_dot_demo_addr;
//  fpga_text_lcd_driver.c
static unsigned char *iom_fpga_text_lcd_addr;
static unsigned char *iom_fpga_demo_addr;
// fpga buzzer & step motor
static unsigned char *iom_fpga_buzzer_addr;
static unsigned char *iom_demo_addr;
static unsigned char *iom_fpga_step_motor_addr;

/*
static unsigned char *iom_fpga_buzzer_addr;
static unsigned char *iom_demo_addr;
static unsigned char *iom_fpga_step_motor_addr;
static unsigned char *iom_fpga_fnd_addr;
static unsigned char *iom_demo_addr_fnd;
static unsigned char *iom_fpga_dot_addr;
static unsigned char *iom_demo_addr_dot;
static unsigned char *iom_fpga_text_lcd_addr;
static unsigned char *iom_demo_addr_lcd;
static unsigned char *iom_fpga_led_addr;
static unsigned char *iom_demo_addr_led;
*/

//  user define function and variable
//      kernel timer
struct timer_list *timer;
struct timer_list *timer2;
void kerneltimer_handler(unsigned long arg);
void kerneltimer_handler2(unsigned long arg);
void kerneltimer_init(void);
void kerneltimer_exit(void);

//     function to be invoked 
void call_device_driver(void);
void call_device_driver2(void);
void init_variables(void);


void fnd_call(void);
/* global variables and flags */
int pid;


int global_time = 0;
int operating_mode;
int time_feed;
int time_toilet;
int time_clean;
int show_mode_time;
int show_mode;

int feed_int, set_day;
int set_day_def = 330;
int onetime_qty;
// clean flag
//int start_clean;


//feed and toilet cnt/interval
int feed_qty;	//gpio 에 표시될 남은 먹이양
int fnd_mode;

int cnt_feed1;	//daily
int cnt_feed2;	//cumulative
int interval_feed3;	
int interval_flag1;
int cnt_toilet1;	//daily
int cnt_toilet2;	//cumulative
int interval_toilet3;
int interval_flag2;

// usage flag
int use_toilet_start;
int use_toilet_sec_cnt;
int toilet_occupied;
int feed_now;
int clean_now;
int before_toilet_time;
int	now_toilet_time;


//interrupt
int last_feed_time;
int last_toilet_time;

//  stopwatch flag
int sw_mode_select;
int min, sec;
unsigned short fnd_write_val;
unsigned short led_val;
unsigned char led_val_list[] = {0x70, 0xB0, 0xD0, 0xE0, 0x00, 0xF0};
unsigned char fnd_val_list[] = {0x96, 0x02, 0x04, 0x10, 0x80};
//unsigned char fnd_dat_list[] = {0x02, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0xC1, 0x1F, 0x01, 0x09, 0xFE, 0x00, 0xFF};
unsigned char fnd_dat_list[] = {0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0xC1, 0x1F, 0x01, 0x09, 0xFE, 0x00, 0xFF};
int time_val[4];
int quit;
unsigned long quit_time;
int quit_time_start;


//fpga led
unsigned short fpga_led_val;

//text lcd
char text_buf[33];


//buzzer
int buzzer_val = 0;		//buzzer on(1)/off(0)
int feed_sec;

//step motor
int motor_operate = 0;	//모터 동작
int rotate_dir;			//회전 방향
int motor_val[3];		
		//[0] - 0: stop rotation, 1: start rotation
		//[1] - 0: dir == left, 1: dir == right
		//[2] - speed, it's value has from 10(fastest speed) to 250(slowest speed)

//fpga dot matrix
char dot_matrix[4][10] = {
	{0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   },//blank
	{0x0E, 0x01, 0x22, 0x36, 0x3E, 0x7F, 0x7F, 0x3E, 0x1C, 0x63},//feed
	{0x00, 0x00, 0x00, 0x08, 0x1C, 0x1C, 0X3E, 0x3E, 0x7F, 0x7F},//toilet
	{0x00, 0x02, 0x06, 0x0A, 0x12, 0x12, 0x22, 0x27, 0x77, 0x07} //clean
};	//각각은 한줄에 7개의 합이겠죠?? 



/////////////////////////////////////////////////////////

//inter -> dev
int dev_open(struct inode *, struct file *);
int dev_release(struct inode *, struct file *);
ssize_t dev_write(struct file *, int *, size_t, loff_t *);
ssize_t dev_read(struct file *, const char *, size_t, loff_t *);
/*
int dev_open(struct inode *, struct file *);
int dev_release(struct inode *, struct file *);
ssize_t dev_write(struct file *, const unsigned long *, size_t, loff_t *);
*/
wait_queue_head_t wq_write;
DECLARE_WAIT_QUEUE_HEAD(wq_write);

//int inter_open(struct inode *, struct file *);
int inter_open(void);
//int inter_release(struct inode *, struct file *);
int inter_release(void);
ssize_t inter_write(struct file *, const unsigned long *, size_t, loff_t *);

irqreturn_t inter_handler(int irq, void* dev_id, struct pt_regs* reg);

static struct file_operations dev_fops =
{
	.open = dev_open,
	.write = dev_write,
	.read = dev_read,
	.release = dev_release,
};


//static int inter_usage=0;
static int dev_usage=0;
int interruptCount=0;

/* dev function */
// 드라이버가 열릴 때 수행되는 함수
int dev_open(struct inode *minode, struct file *mfile) {
    if (dev_usage != 0)
        return -EBUSY;
    dev_usage = 1;
    return 0;
}

// 드라이버가 클로즈 될 때 수행되는 함수
int dev_release(struct inode *minode, struct file *mfile) {
    dev_usage = 0;
    return 0;
}

// 드라이버에 값을 쓰기 위한 함수
ssize_t dev_write(struct file *inode, int *gdata, size_t length,
        loff_t *off_what) {
    int *tmp = gdata;
	int gdata_val = *gdata;
//    unsigned short dev_buff = 0;
	unsigned int dev_buff = 0;
//    unsigned short AND = 0xFF;

	int i, pos, val;
	int k;
	int g1, g2, g3, g4;	//JIN params - g1: flag, g2: temp, g3: mode, g4:data(distance)
	unsigned short fpga_fnd_val[4];

	
   // 사용자 공간에서 데이터를 커널공간으로 복사
    if (copy_from_user(&dev_buff, tmp, length))
		// 2byte receive from application : unsigned short(2byte) -> unsigned int(4byte)
        return -EFAULT;


	/* gdata 사용하여 각 write에 사용될 변수값 set 하고 mode 변경 */


    //stopwatch 인 부분

	printk("ggsato\n");
	//printk("@  [%d][%d][%d][%d]\n", g1, g2, g3, g4);
	del_timer(timer);
    add_timer(timer);
//	del_timer(timer2);
//	add_timer(timer2);
	//printk("dev_write: @01 \n");


//#write
//#dev_write
	//gpio (use  interrupt)
/*
	switch(fnd_mode){
		case 0:	//shows feed qtya
			//printk("    fnd 0\n");
			
	//		for(i=1, k=1000 ; i<=4 ; i++){
	///			pos = fnd_val_list[i];
	//			val = fnd_dat_list[(feed_qty % (k*10)) / k];
	//			fnd_write_val = (pos << 8) | val;
	//			fnd_write(fnd_write_val);
	//			mdelay(1);
	//			if(k>1)
	//				k/=10;
	//		}
			
			pos = fnd_val_list[1];
//			val = fnd_dat_list[(feed_qty % (k*10)) / k];
			val = fnd_dat_list[fnd_mode];
			fnd_write_val = (pos << 8) | val;
			fnd_write(fnd_write_val);
			mdelay(1);
			break;
		case 1:	//shows last feed time
			//printk("    fnd 1\n");
		
	//		for(i=1, k=1000 ; i<=4 ; i++){
//				pos = fnd_val_list[i];
//				val = fnd_dat_list[(last_feed_time % (k*10)) / k];
//				fnd_write_val = (pos << 8) | val;
//				fnd_write(fnd_write_val);
//				mdelay(1);
//				if(k>1)
//					k/=10;
//			}
			pos = fnd_val_list[2];
//			val = fnd_dat_list[(feed_qty % (k*10)) / k];
			val = fnd_dat_list[fnd_mode];
			fnd_write_val = (pos << 8) | val;
			fnd_write(fnd_write_val);
			mdelay(1);
			break;

		case 2:	//shows last toilet time
			//printk("    fnd 2\n");
	//	for(i=1, k=1000 ; i<=4 ; i++){
	//			pos = fnd_val_list[i];
	//			val = fnd_dat_list[(last_toilet_time % (k*10)) / k];
	//			fnd_write_val = (pos << 8) | val;
	//			fnd_write(fnd_write_val);
	//			mdelay(1);
	//			if(k>1)
	//				k/=10;
	//		}
			pos = fnd_val_list[3];
//			val = fnd_dat_list[(feed_qty % (k*10)) / k];
			val = fnd_dat_list[fnd_mode];
			fnd_write_val = (pos << 8) | val;
			fnd_write(fnd_write_val);
			mdelay(1);
			break;

		case 3:
			pos = fnd_val_list[4];
//			val = fnd_dat_list[(feed_qty % (k*10)) / k];
			val = fnd_dat_list[fnd_mode];
			fnd_write_val = (pos << 8) | val;
			fnd_write(fnd_write_val);
		break;
	}
*/

	
	//fnd_call();
	//printk("dev_write: @02\n");
	//gpio led
	led_val = led_val_list[fnd_mode];
	led_write();

	//printk("dev_write: @03\n");

	//#application 
		//int g1, g2, g3, g4;	//JIN params - g1: flag, g2: temp, g3: mode, g4:data(distance)
	g4 = gdata_val & 0xFF;			//g4: data(distance: 0 - not in toilet or 1 - in toilet )
	g3 = (gdata_val>>8) & 0xFF;		//g3: mode(app management - show selected(input mode) informaition now)
	g2 = (gdata_val>>16) & 0xFF;	//g2: temp - not used (alignment)
	g1 = (gdata_val>>24) & 0xFF;	//g1: flag - it determines where gdata from

	//printk("dev_write: @04\n");
	printk("gg [%d][%d][%d][%d]\n", g1, g2, g3, g4);
	if(g3 == 7){
		//set feed quantity(var: onetime_qty)
		onetime_qty = g2;
	}
	else if(g3 == 8){
		//set feed interval(var: feed_int)
		feed_int = g2;
	}

	//printk("dev_write: @05\n");
	//applicatoin edits these values...?
	//feed_int = 60;	//feed interval(sec)
	//onetime_qty = 75;
	set_day = 150;	//A day is 86400 seconds, but I set the day to seconds to test the application

	if(( /*timer->data*/ global_time % feed_int) == 0  && global_time != 0){
		//start feed... set op_mode to 1 and set feed_sec to 5
		operating_mode = 1;
		feed_sec = 5;	//feed_sec dereases in kerneltimer func
		feed_qty -= onetime_qty;

		cnt_feed1 += onetime_qty;	//daily
		cnt_feed2 += onetime_qty;	//cumulative
	}
	
	//printk("dev_write: @06\n");
	//set feed_sec 

	
	//g4 check a cat is in the toilet
	//g4 == 0 : not in, g4 == 1: in
	if(g3 == 0){
		if(g4 == 0 && toilet_occupied == 1){
			use_toilet_start = 0;
			use_toilet_sec_cnt = 0;
			toilet_occupied = 0;

			interval_toilet3 = now_toilet_time - before_toilet_time;
			before_toilet_time = now_toilet_time;
			now_toilet_time = 0;

			//set clean time
			time_clean = 5;
		}
		else if(g4 == 0){
			use_toilet_start = 0;
			use_toilet_sec_cnt = 0;
		}
		else if(g4 == 1 && use_toilet_start == 0){
			use_toilet_start = 1;
			//		use_toilet_sec_cnt++;	//in kernel timer
		}
	}
	//printk("dev_write: @07\n");
	if(use_toilet_sec_cnt >= 5){
		//cat is in the toilet...
		toilet_occupied = 1;
		if(use_toilet_sec_cnt == 5){
			now_toilet_time = global_time;//timer->data;
			cnt_toilet1++;
			cnt_toilet2++;
		}
	}

	//printk("dev_write: @08\n");


	//set operating_mode - 0: blank, 1: feed, 2: toilet, 3: clean
	//feed_sec, use_toilet_sec_cnt, time_clean;		
	//priority: clean > toilet > feed
	if(time_clean > 0){
		operating_mode = 3;
	}
	else if(use_toilet_sec_cnt > 0){
		operating_mode = 2;
	}
	else if(feed_sec > 0){
		operating_mode = 1;
	}
	else{
		operating_mode = 0;
	}

	//printk("dev_write: @09\n");
	//g3 determine fpga text lcd and fnd value;
	if(g3 != 0){
		show_mode = g3;
		if(show_mode_time == 0)
			show_mode_time = 5;	//kernel
	}
	else if(show_mode_time > 0){
		//Does not change the show_mode
	}
	else{
		show_mode = 0;	//blank
//		show_mode = operating_mode;
	}

	//printk("dev_write: @10\n");

	//Android:: mainActivity btn
	//Feed now(g3: 9, g4: ?) and Clean now(g3: 10, g4: ?)
	if(g3 == 9){
		//start feed... set op_mode to 1 and set feed_sec to 5
		operating_mode = 1;
		feed_sec = 5;	//feed_sec dereases in kerneltimer func
		feed_qty -= onetime_qty;

		cnt_feed1 += onetime_qty;	//daily
		cnt_feed2 += onetime_qty;	//cumulative

		show_mode = 0;
	}
	else if(g3 == 10){
		operating_mode = 3;
		time_clean = 5;
	}
	printk("show[%d], oper[%d]\n", show_mode, operating_mode);
	//printk("dev_write: @11\n");
	//fpga_fnd
	switch(show_mode){
		case 0:	// blank
			for(i=0 ; i<4 ; i++)
				fpga_fnd_val[i] = 0;
			iom_fpga_fnd_write(fpga_fnd_val);
			break;
		case 1:	//shows last feed time
			//cnt_feed1
			for(i=0, k=1000 ; i<4 ; i++){
				fpga_fnd_val[i] = (cnt_feed1 % (k * 10)) / k;
				if(k>1)
					k/=10;
			}
			iom_fpga_fnd_write(fpga_fnd_val);
			break;
		case 2:	//shows last toilet time
			//cne_feed2
			for(i=0, k=1000 ; i<4 ; i++){
				fpga_fnd_val[i] = (cnt_feed2 % (k * 10)) / k;
				if(k>1)
					k/=10;
			}
			iom_fpga_fnd_write(fpga_fnd_val);
			break;
		case 3:
			//feed_int
			for(i=0, k=1000 ; i<4 ; i++){
				fpga_fnd_val[i] = (feed_int % (k * 10)) / k;
				if(k>1)
					k/=10;
			}
			iom_fpga_fnd_write(fpga_fnd_val);
			break;
		case 4:
			//cnt_toilet1
			for(i=0, k=1000 ; i<4 ; i++){
				fpga_fnd_val[i] = (cnt_toilet1 % (k * 10)) / k;
				if(k>1)
					k/=10;
			}
			iom_fpga_fnd_write(fpga_fnd_val);
			break;
		case 5:
			//cnt_toilet2
			for(i=0, k=1000 ; i<4 ; i++){
				fpga_fnd_val[i] = (cnt_toilet2 % (k * 10)) / k;
				if(k>1)
					k/=10;
			}
			iom_fpga_fnd_write(fpga_fnd_val);
			break;
		case 6:
			//interval_toilet3
			for(i=0, k=1000 ; i<4 ; i++){
				fpga_fnd_val[i] = (interval_toilet3 % (k * 10)) / k;
				if(k>1)
					k/=10;
			}
			iom_fpga_fnd_write(fpga_fnd_val);
			break;

	}
	//printk("dev_write: @12\n");
	//fpga_text_lcd
	set_text(show_mode);
	//printk("dev_write: @13\n");
	iom_fpga_text_lcd_write();
	//fnd_call();
	//printk("dev_write: @14\n");
	//fpga_led
	iom_led_write(operating_mode);
	//fnd_call();
	//printk("dev_write: @15\n");
	//dot
	iom_fpga_dot_write(operating_mode);
	//fnd_call();
	//printk("dev_write: @16\n");
	//buzzer
	buzzer_driver_write();	//operating_mode == feed, and feed_sec > 0 (if It starts to feed, set the feed_sec to 5
	//fnd_call();
	//printk("dev_write: @17\n");
	//step_motor
	step_motor_driver_write(operating_mode);
	//fnd_call();
	//printk("dev_write: @18\n");


    return length;
}

ssize_t dev_read(struct file *inode, const char *gdata, size_t length, loff_t *off_what)
{
	unsigned char value;
	const char *tmp = gdata;
	//value = inb((unsigned int)iom_fpga_buzzer_addr);
	
	value = inb((unsigned int)iom_fpga_buzzer_addr);
	if(copy_to_user(tmp, &value, 1))
		return -EFAULT;
	
	return length;
}




/* GIVEN CODE: interrupt */

irqreturn_t inter_handler1(int irq, void* dev_id, struct pt_regs* reg){
    //SW2
	printk("interrupt1!!! = %x\n", gpio_get_value(S5PV310_GPX2(0)));
    //sw_mode_select = 2;
    //interruptCount = 0;
//	min = 59;
	//if(gpio_get_value(S5PV310_GPX2(0)) == 1)
	fnd_mode = 1;
	
	return IRQ_HANDLED;
}

irqreturn_t inter_handler2(int irq, void* dev_id, struct pt_regs* reg){
    //SW3
	printk("interrupt2!!! = %x\n", gpio_get_value(S5PV310_GPX2(1)));
    //sw_mode_select = 3;
    //interruptCount = 0;
	//if(gpio_get_value(S5PV310_GPX2(1)) == 1)
	fnd_mode = 2;
	return IRQ_HANDLED;
}

irqreturn_t inter_handler3(int irq, void* dev_id, struct pt_regs* reg){
    //SW4
	printk("interrupt3!!! = %x\n", gpio_get_value(S5PV310_GPX2(2)));
    //sw_mode_select = 4;
    //interruptCount = 0;
//	if(gpio_get_value(S5PV310_GPX2(2)) == 0){
		// FALLING(PRESS)
//		fnd_mode = 2;
//	}
//	else{
		// RISING(RELEASE)
//		fnd_mode = 0;
//	}
	if(gpio_get_value(S5PV310_GPX2(2)) == 1){
		feed_qty += 10;
	}
		fnd_mode = 0;
	
	return IRQ_HANDLED;
}

irqreturn_t inter_handler4(int irq, void* dev_id, struct pt_regs* reg){
    //VOL-
	printk("interrupt4!!! = %x\n", gpio_get_value(S5PV310_GPX2(3)));
    //interruptCount = 0;
	return IRQ_HANDLED;
}

irqreturn_t inter_handler5(int irq, void* dev_id, struct pt_regs* reg){
    //SW6
	printk("interrupt5!!! = %x\n", gpio_get_value(S5PV310_GPX2(4)));
	//fnd_mode = 3;mo
	if(gpio_get_value(S5PV310_GPX2(4)) == 1){
		feed_qty += 100;
	}
		fnd_mode = 0;
//	if(gpio_get_value(S5PV310_GPX2(4)) == 0){
		// FALLING(PRESS)
//		fnd_mode = 3;
//	}
//	else{
//		// RISING(RELEASE)
//		fnd_mode = 0;
//	}


/*
	if(gpio_get_value(S5PV310_GPX2(4)) == 0 && quit_time_start == 0){
		//PRESS - FALLING (press SW6)
		quit_time = timer->data;
		quit_time_start = 1;
		printk("SW6_press_1_[%d][%lu][%lu]\n",quit_time_start, quit_time, timer->data);
	}

	else{
		//RELEASE - RISING
		quit_time_start = 0;
		printk("SW6_release\n");
	}
*/

	return IRQ_HANDLED;
}

irqreturn_t inter_handler6(int irq, void* dev_id, struct pt_regs* reg){
    //VOL+
	printk("interrupt6!!! = %x\n", gpio_get_value(S5PV310_GPX2(5)));
    //interruptCount = 0;
	return IRQ_HANDLED;
}

irqreturn_t inter_handler7(int irq, void* dev_id, struct pt_regs* reg){
    //POWER
	printk("interrupt7!!! = %x\n", gpio_get_value(S5PV310_GPX0(1)));
    //interruptCount = 0;
	return IRQ_HANDLED;
}

//int inter_open(struct inode *minode, struct file *mfile){
int inter_open(){
	int ret;
/*    
	if(inter_usage!=0)
		return -EBUSY;
	inter_usage=1;
*/  

	/*
	*       SW2 : GPX2(0)
	*       SW3 : GPX2(1)
	*       SW4 : GPX2(2)
	*       SW6 : GPX2(4)
	*
	*       VOL+ : GPX2(5)
	*       POWER : GPX0(1) 
	*       VOL- : GPX0(1)
	*
	*		SW2,3,4,6 : PRESS-FALLING-0 / RELEASE-RISING-1
	*		VOL -, +  : PRESS-RISING-1 / RELEASE-FALLING-0		
	*		POWER	  : PRESS-FALLING-0 / RELEASE-RISING-1
	*/
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(0)), &inter_handler1, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "X2.0", NULL);//SW2
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(1)), &inter_handler2, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "X2.1", NULL);//SW3
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(2)), &inter_handler3, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "X2.2", NULL);//SW4
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(3)), &inter_handler4, IRQF_TRIGGER_RISING,"X2.3", NULL);//VOL-
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(4)), &inter_handler5, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "X2.4", NULL);//SW6
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(5)), &inter_handler6, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "X2.5", NULL);//VOL+
	ret=request_irq(gpio_to_irq(S5PV310_GPX0(1)), &inter_handler7, IRQF_TRIGGER_RISING, "X0.1", NULL);//POWER
/*
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(0)), &inter_handler1, IRQF_TRIGGER_RISING, "X2.0", NULL);//SW2
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(1)), &inter_handler2, IRQF_TRIGGER_RISING, "X2.1", NULL);//SW3
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(2)), &inter_handler3, IRQF_TRIGGER_RISING, "X2.2", NULL);//SW4
	//ret=request_irq(gpio_to_irq(S5PV310_GPX2(2)), &inter_handler3, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "X2.2", NULL);//SW4
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(3)), &inter_handler4, IRQF_TRIGGER_RISING,"X2.3", NULL);//VOL-
	//ret=request_irq(gpio_to_irq(S5PV310_GPX2(4)), &inter_handler5, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "X2.4", NULL);//SW6
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(4)), &inter_handler5, IRQF_TRIGGER_RISING, "X2.4", NULL);//SW6
	//ret=request_irq(gpio_to_irq(S5PV310_GPX2(4)), &inter_handler5, IRQF_TRIGGER_FALLING, "X2.4", NULL);//SW6
	ret=request_irq(gpio_to_irq(S5PV310_GPX2(5)), &inter_handler6, IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING, "X2.5", NULL);//VOL+
	ret=request_irq(gpio_to_irq(S5PV310_GPX0(1)), &inter_handler7, IRQF_TRIGGER_RISING, "X0.1", NULL);//POWER
*/
/* Ignore compile warning line 737 ~ 746 */
	printk("inter open\n");
	return 0;
}

//int inter_release(struct inode *minode, struct file *mfile){
int inter_release(){
/*
    inter_usage=0;
*/
	free_irq(gpio_to_irq(S5PV310_GPX2(0)), NULL);
	free_irq(gpio_to_irq(S5PV310_GPX2(1)), NULL);
	free_irq(gpio_to_irq(S5PV310_GPX2(2)), NULL);
	free_irq(gpio_to_irq(S5PV310_GPX2(3)), NULL);
	free_irq(gpio_to_irq(S5PV310_GPX2(4)), NULL);
	free_irq(gpio_to_irq(S5PV310_GPX2(5)), NULL);
	free_irq(gpio_to_irq(S5PV310_GPX0(1)), NULL);
	printk("inter release\n");
	return 0;
}

ssize_t inter_write(struct file *inode, const unsigned long *gdata, size_t length, loff_t *off_what){
    /*
	if(interruptCount==0){
		printk("sleep on\n");
		interruptible_sleep_on(&wq_write);
	}
    */
	printk("inter_write\n");

	return length;
}

/*
int __init inter_init(void){
	int result;
	result = register_chrdev(DEV_MAJOR,DEV_NAME, &inter_fops);
	interruptCount=0;

	if(result <0) {
		printk(KERN_WARNING"Can't get any major!\n");
		return result;
	}

	return 0;
}

void __exit inter_exit(void){
	unregister_chrdev(DEV_MAJOR,DEV_NAME);
}
*/
////////////////////////////////////////////////////////////////////


/* fnd */
//#fnd
void fnd_call(void){
	int i, k, pos, val;
	int j=10;
	while(j--){
	switch(fnd_mode){
		case 0:	//shows feed qtya
			//printk("    fnd 0\n");
			for(i=1, k=1000 ; i<=4 ; i++){
				pos = fnd_val_list[i];
				val = fnd_dat_list[(feed_qty % (k*10)) / k];
				fnd_write_val = (pos << 8) | val;
				fnd_write(fnd_write_val);
				mdelay(1);
				if(k>1)
					k/=10;
			}
			mdelay(1);
			break;
		case 1:	//shows last feed time
			//printk("    fnd 1\n");
			for(i=1, k=1000 ; i<=4 ; i++){
				pos = fnd_val_list[i];
				val = fnd_dat_list[(last_feed_time % (k*10)) / k];
				fnd_write_val = (pos << 8) | val;
				fnd_write(fnd_write_val);
				mdelay(1);
				if(k>1)
					k/=10;
			}
			mdelay(1);
			break;

		case 2:	//shows last toilet time
			//printk("    fnd 2\n");
			for(i=1, k=1000 ; i<=4 ; i++){
				pos = fnd_val_list[i];
				val = fnd_dat_list[(last_toilet_time % (k*10)) / k];
				fnd_write_val = (pos << 8) | val;
				fnd_write(fnd_write_val);
				mdelay(1);
				if(k>1)
					k/=10;
			}
			mdelay(1);
			break;
	}
	}
}
ssize_t fnd_write(unsigned short gdata)
{
    unsigned char fnd_sel = (unsigned char)(gdata >> 8);
    unsigned char fnd_dat = (unsigned char)(gdata & 0x00FF);

    outb (fnd_sel,(unsigned int)fnd_data2);
    outb (fnd_dat, (unsigned int)fnd_data);
	return 0;
//    return length;
}


int fnd_init(void)
{
//    int result;
    struct class *fnd_dev_class=NULL;
    struct device *fnd_dev=NULL;
/*
    result = register_chrdev(FND_MAJOR, FND_NAME, &fnd_fops);
    if(result <0) {
        printk(KERN_WARNING"Can't get any major!\n");
        return result;
    }
*/
    fnd_data = ioremap(FND_GPL2DAT, 0x01);
    fnd_data2 = ioremap(FND_GPE3DAT, 0x01);
    if(fnd_data==NULL)
    {
        printk("ioremap failed!\n");
        return -1;
    }
    fnd_ctrl = ioremap(FND_GPL2CON, 0x04);
    fnd_ctrl2 = ioremap(FND_GPE3CON, 0x04);
    if(fnd_ctrl==NULL)
    {
        printk("ioremap failed!\n");
        return -1;
    } else {
        fnd_dev = device_create(fnd_dev_class,NULL,MKDEV(FND_MAJOR,0),NULL,FND_NAME);
        if(fnd_dev!=NULL)
        {
            outl(0x11111111,(unsigned int)fnd_ctrl);
            outl(0x10010110,(unsigned int)fnd_ctrl2);
        }
        else {
            printk("device_reate : failed!\n");
        }
    }
//    printk("init module, /dev/fnd_driver major : %d\n", FND_MAJOR);
    printk("fnd init\n");
    outb(0xFF, (unsigned int)fnd_data);
    outb(0xFF, (unsigned int)fnd_data);

    return 0;
}


void fnd_exit(void)
{
    outb(0xFF, (unsigned int)fnd_data);
    iounmap(fnd_data);
    iounmap(fnd_data2);
    iounmap(fnd_ctrl);
    iounmap(fnd_ctrl2);
//    unregister_chrdev(FND_MAJOR, FND_NAME);
    printk("FND module removed.\n");
}


/* gpio led  */
//#led
ssize_t led_write(void)  
{
	outb (led_val, (unsigned int)led_data);  // 넘어온 데이터를 기록 -> LED 동작
	return 0;
}
int led_init(void)  // 드라이버가 초기화 될 때 동작되는 함수
{
	//    int result;
	unsigned int get_ctrl_io=0;
	struct class *led_dev_class=NULL;
	struct device *led_dev=NULL;

	/*
	// 디바이스 드라이버 등록
	result = register_chrdev(LED_MAJOR, LED_NAME, &led_fops);
	if(result <0) {  // 실패하면 오류처리
	printk(KERN_WARNING"Can't get any major!\n");
	return result;
	}
	 */
	led_data = ioremap(LED_GPBDAT, 0x01);  // LED 제어 데이터 메모리 매핑
	if(led_data==NULL) // 매핑에 실패하면 에러처리
	{   
		printk("ioremap failed!\n");
		return -1;
	}

	led_ctrl = ioremap(LED_GPBCON, 0x04);  // 물리주소를 매핑
	if(led_ctrl==NULL)  // 물리주소 매핑에서 오류가 발생하면 에러 처리
	{
		printk("ioremap failed!\n");
		return -1;
	} else {   // 물리주소 매핑을 성공하면 해당 레지스터 값을 변경
		get_ctrl_io=inl((unsigned int)led_ctrl);  // GPBCON 데이터를 받음
		led_dev = device_create(led_dev_class,NULL,MKDEV(LED_MAJOR,0),NULL,LED_NAME);
		buffer = (char*)kmalloc(1024, GFP_KERNEL);

		if(buffer != NULL)
			memset(buffer, 0, 1024);

		get_ctrl_io|=(0x11110000);  // GPB Pin 설정 레지스트 상위 4바이트를 출력으로 설정
		outl(get_ctrl_io,(unsigned int)led_ctrl);  // 설정된 값을 레지스터에 적용  
	}

	//    printk("init module, /dev/led_driver major : %d\n", LED_MAJOR);
	printk("led init\n");
	outb(0xF0, (unsigned int)led_data);

	return 0;
}
void led_exit(void)
{
	outb(0xF0, (unsigned int)led_data);
	iounmap(led_data);
	iounmap(led_ctrl);
	//    unregister_chrdev(LED_MAJOR, LED_NAME);
	printk("Removed LED module\n");

}

/* fpga led ( iom_led_~~~) */
//#fpga_led
ssize_t iom_led_write(int mode/*unsigned short gdata*/) 
{
	//unsigned char value = gdata;
	unsigned char value;
	unsigned char led_mat[4] = {128, 64, 32, 16};
	if(mode == 3){	//operating_mode: 3 - toilet
		value = 0;
		if(/*timer->data*/global_time %2)
			value += 0x0F;
//		value += led_mat[timer->data % 4];
		value += led_mat[global_time % 4];
		//value = timer->data	% 8;	
	}
	else{
		value = 0;
	}

	outb(value,(unsigned int)iom_fpga_led_addr);	    
	return 0;	
	//	return length;
}
int iom_led_init(void)
{

	iom_fpga_led_addr = ioremap(IOM_LED_ADDRESS, 0x1);
	iom_led_demo_addr = ioremap(IOM_DEMO_ADDRESS, 0x1);
	outb(UON,(unsigned int)iom_led_demo_addr);	    

	printk("fnd led init\n");
	return 0;
}
void iom_led_exit(void) 
{
	iounmap(iom_fpga_led_addr);
	iounmap(iom_led_demo_addr);
	//	unregister_chrdev(IOM_LED_MAJOR, IOM_LED_NAME);
	printk("fpga led module removed\n");	
}

/* fpga fnd */
//#fpga_fnd
ssize_t iom_fpga_fnd_write(unsigned short *gdata) 
{
	int i;
	for(i=0 ; i<4 ; i++)
		outb(gdata[i], (unsigned int)iom_fpga_fnd_addr+i);
	return 0;
}
int iom_fpga_fnd_init(void)
{

	iom_fpga_fnd_addr = ioremap(IOM_FND_ADDRESS, 0x4);
	iom_fnd_demo_addr = ioremap(IOM_DEMO_ADDRESS, 0x1);

	outb(UON,(unsigned int)iom_fnd_demo_addr);	    

	printk("fpga fnd init\n");
	return 0;
}
void iom_fpga_fnd_exit(void) 
{
	iounmap(iom_fpga_fnd_addr);
	iounmap(iom_fnd_demo_addr);
	//	unregister_chrdev(IOM_FND_MAJOR, IOM_FND_NAME);
}

/* fpga dot */
//#fpga_dot
ssize_t iom_fpga_dot_write(int mode) 
{
	int i;
	for(i=0 ; i< 10 ; i++)
		outb(dot_matrix[mode][i],(unsigned int)iom_fpga_dot_addr+i);
	return 0;
}
int iom_fpga_dot_init(void)
{

	iom_fpga_dot_addr = ioremap(IOM_FPGA_DOT_ADDRESS, 0x10);
	iom_dot_demo_addr = ioremap(IOM_FPGA_DEMO_ADDRESS, 0x1);

	outb(UON,(unsigned int)iom_dot_demo_addr);	    
	printk("fpga dot init\n");
	return 0;
}
void iom_fpga_dot_exit(void) 
{
	iounmap(iom_fpga_dot_addr);
	//	unregister_chrdev(IOM_FPGA_DOT_MAJOR, IOM_FPGA_DOT_NAME);
}


/* fpga text lcd */
//#fpga_text_lcd
//#fpga_lcd_text
//#fpga_text
//#fpga_lcd
//#lcd
ssize_t iom_fpga_text_lcd_write() 
{
	int i;
	for(i=0 ; i<32 ; i++)
		outb(text_buf[i],(unsigned int)iom_fpga_text_lcd_addr+i);
//	outb(gdata[i],(unsigned int)iom_fpga_text_lcd_addr+i);
	return 0;

}
void set_text_func(char *str){
	int i;

	for(i=0 ; i<32 ; i++){
		if(i < strlen(str)){
			text_buf[i] = *(str + i);
		}
		else{
			text_buf[i] = ' ';
		}
	}
}
void set_text(int mode){
	//text_buf[32]
	//int i;
	char *feed1 = "Daily fed count";
	char *feed2 = "Cumulative fed count";
	char *feed3 = "Interval of fed time";
	char *toilet1 = "Daily toilet usage count";
	char *toilet2 = "Cumulative toilet usage count";
	char *toilet3 = "Interval of toilet usage";	//

	
//	printk("set text call \n");
	switch(mode){
		case 1:
			set_text_func(feed1);
			break;
		case 2:
			set_text_func(feed2);
			break;
		case 3:
			set_text_func(feed3);
			break;
		case 4:
			set_text_func(toilet1);
			break;
		case 5:
			set_text_func(toilet2);
			break;
		case 6:
			set_text_func(toilet3);
			break;
		default:	//blank
			iom_fpga_text_lcd_init();
			break;
	}
/*
	for(i=0 ; i<32 ; i++){
		if(mode == 1){

			text_buf[i] = *(feed1 + i);
		}
	}
*/
}
int iom_fpga_text_lcd_init(void)
{
	int i;
	for(i=0 ; i<32 ; i++)
		text_buf[i] = ' ';
	text_buf[32] = '\0';

	iom_fpga_text_lcd_addr = ioremap(IOM_FPGA_TEXT_LCD_ADDRESS, 0x20);
	iom_fpga_demo_addr = ioremap(IOM_FPGA_DEMO_ADDRESS, 0x1);

	outb(UON,(unsigned int)iom_fpga_demo_addr);	    
//	printk("fpga text lcd init\n");
	return 0;
}
void iom_fpga_text_lcd_exit(void) 
{
	iounmap(iom_fpga_text_lcd_addr);
	//	unregister_chrdev(IOM_FPGA_TEXT_LCD_MAJOR, IOM_FPGA_TEXT_LCD_NAME);
}

/* fpga buzzer */
//#fpga_buzzer
void buzzer_driver_write(void)
{
	if(feed_sec > 0 && operating_mode == 1)
		buzzer_val = 1;
	else
		buzzer_val = 0;

	outb(buzzer_val, (unsigned int)iom_fpga_buzzer_addr);		
}
void buzzer_driver_init(void)
{
	printk("buzzer init\n");
	iom_fpga_buzzer_addr = ioremap(IOM_BUZZER_ADDRESS, 0x1);
	iom_demo_addr = ioremap(IOM_DEMO_ADDRESS, 0x1);
}
void buzzer_driver_exit(void)
{
	printk("buzzer exit\n");
	iounmap(iom_fpga_buzzer_addr);
	iounmap(iom_demo_addr);
}

/* fpga step motor */
//#fpga_motor
void step_motor_driver_write(int mode)
{
	// 먹이 주기 || 화장실 청소 일때  step motor 작동
	// 두 상황은 회전방향으로 구분됨
	/*
	   int motor_operate = 0;	//모터 동작
	   int rotate_dir;			//회전 방향
	   int motor_val[3];		
	*/
	//[0] - 0: stop rotation, 1: start rotation
	//[1] - 0: dir == left, 1: dir == right
	//[2] - speed, it's value has from 10(fastest speed) to 250(slowest speed)
	int i;

//	printk("step_motor: @01\n");
	switch(mode){
		//case 0:	//blank
		//break;
		case 1:	//feed
			motor_val[0] = 1;
			motor_val[1] = 0;
		break;
		//case 2:	//toilet
		//	break;
		case 3:	//clean
			motor_val[0] = 1;
			motor_val[1] = 1;
		break;
		default:	//blank & toilet
			motor_val[0] = 0;
			motor_val[1] = 0;
		break;
	}

	motor_val[2] = 250;	//fixed value

//	printk("step_motor: @02\n");
	for(i=0 ; i<3 ; i++)
		outb(motor_val[i], (unsigned int)iom_fpga_step_motor_addr + i);
//	printk("step_motor: @03\n");
}
void step_motor_driver_init(void)
{
	printk("step motor init\n");
	iom_fpga_step_motor_addr = ioremap(IOM_FPGA_STEP_MOTOR_ADDRESS, 0x4);
}
void step_motor_driver_exit(void)
{
	printk("step motor exit\n");
	iounmap(iom_fpga_step_motor_addr);
}



/* kernel timer */
void kerneltimer_init(){

    timer = kmalloc(sizeof(struct timer_list), GFP_KERNEL);
    if(timer == NULL){
        printk("timer allocation fail\n");
        return;
    }    
    init_timer(timer);
    timer->expires = get_jiffies_64() + (10*HZ)/10;
    timer->data = (unsigned long)&timer;
    timer->function = kerneltimer_handler;

/*
	timer2 = kmalloc(sizeof(struct timer_list), GFP_KERNEL);
	if(timer2 == NULL){
		printk("timer2 allocation fail\n");
		return;
	}
	init_timer(timer2);
	timer2->expires = get_jiffies_64() + HZ/100;
	timer2->data = (unsigned long)&timer2;
	timer2->function = kerneltimer_handler2;
*/	
}

void kerneltimer_handler2(unsigned long arg){
	timer2->expires = get_jiffies_64() + HZ/100;
	timer2->data++;

	call_device_driver2();
	add_timer(timer2);
}

void kerneltimer_handler(unsigned long arg){
   
    //timer->expires = get_jiffies_64() + (interval*HZ)/10;
    timer->expires = get_jiffies_64() + (10*HZ)/10;
    timer->data++;
	global_time++;
    //function invoke
    call_device_driver();
    
    add_timer(timer);
   
}

void kerneltimer_exit(){
    
    if(timer != NULL){
		printk("kernel timer free\n");
        del_timer(timer);
        kfree(timer);
        return;
    }
    
    if(timer2 != NULL){
		printk("kernel timer2 free\n");
        del_timer(timer2);
        kfree(timer2);
        return;
    }
	
    printk("timer free fail\n");
    return;
}
//#timer2
void call_device_driver2(){
	int i, k;
	int pos, val;
//	while(1){
	switch(fnd_mode){
		case 0:	//shows feed qtya
			//printk("    fnd 0\n");
			for(i=1, k=1000 ; i<=4 ; i++){
				pos = fnd_val_list[i];
				val = fnd_dat_list[(feed_qty % (k*10)) / k];
				fnd_write_val = (pos << 8) | val;
				fnd_write(fnd_write_val);
				mdelay(1);
				if(k>1)
					k/=10;
			}
			mdelay(1);
			break;
		case 1:	//shows last feed time
			//printk("    fnd 1\n");
			for(i=1, k=1000 ; i<=4 ; i++){
				pos = fnd_val_list[i];
				val = fnd_dat_list[(last_feed_time % (k*10)) / k];
				fnd_write_val = (pos << 8) | val;
				fnd_write(fnd_write_val);
				mdelay(1);
				if(k>1)
					k/=10;
			}
			mdelay(1);
			break;
		case 2:	//shows last toilet time
			//printk("    fnd 2\n");
			for(i=1, k=1000 ; i<=4 ; i++){
				pos = fnd_val_list[i];
				val = fnd_dat_list[(last_toilet_time % (k*10)) / k];
				fnd_write_val = (pos << 8) | val;
				fnd_write(fnd_write_val);
				mdelay(1);
				if(k>1)
					k/=10;
			}
			mdelay(1);
			break;

	}
//	}
}

//kernelhandler invokes this function
void call_device_driver(){





//#kernel timer
//#timer
	//global_time++;
	printk("[%d]_[%d]_[%d]_[%d]_[%d]\n", feed_sec, global_time, use_toilet_start, time_clean, show_mode_time);
	//printk("call_device:@02 \n");
	if(feed_sec > 0 )
		feed_sec--;


	//printk("call_device: @03\n");
	if((global_time % set_day) == 0){
		//end of a day
		//reset the daily feed and daily toilet usage
		printk("end of a day\n");
		cnt_feed1 = 0;
		cnt_toilet1 = 0;
	}

/*
	printk("call_device: @03\n");
	if( (timer->data) > set_day){
		cnt_feed1 = 0;
		cnt_toilet1 = 0;
		set_day += set_day_def;
	}
*/	
	//printk("call_device: @04\n");
	if(use_toilet_start == 1){
		use_toilet_sec_cnt++;	//cnt increase every sec
	}

	//printk("call_device: @05\n");
	if(time_clean > 0)
		time_clean--;

	//printk("call_device: @06\n");
	if(show_mode_time > 0)
		show_mode_time--;
	//printk("call_device: @07\n");
}

//#init_variables
void init_variables(void){
	//int i;

	led_val = led_val_list[5];

	//below 2 vars are used in #ap and it can be changed by application btn
	feed_int = 55;	//feed interval(sec)
	onetime_qty = 77;

	operating_mode = 0;
	
	time_feed = 0;
	time_toilet = 0;
	time_clean = 0;
	show_mode_time = 0;

// clean flag
	//start_clean;

	fnd_mode = 0;

	//feed and toilet cnt/interval
	feed_qty = 1000;	//gpio 에 표시될 남은 먹이양


	//They are displayed in fpga fnd 
	cnt_feed1 = 0;	//daily
	cnt_feed2 = 0;	//cumulative
	interval_feed3 = -1;	
	interval_flag1 = 0;
	cnt_toilet1 = 0;	//daily
	cnt_toilet2 = 0;	//cumulative
	interval_toilet3 = -1;
	interval_flag2 = 0;

	// usage flag
	use_toilet_start = 0;
	use_toilet_sec_cnt = 0;
	toilet_occupied = 0;
	feed_now = 0;
	clean_now = 0;
	before_toilet_time = 0;
	now_toilet_time = 0;

	//interrupt
	last_feed_time = 0;
	last_toilet_time = 0;

	//fpga led
	fpga_led_val = 0;
	//buzzer
	buzzer_val = 0;		//buzzer on(1)/off(0)
	feed_sec = 0;

	//step motor
	motor_operate = 0;	//모터 동작
	rotate_dir = 0;			//회전 방향
	motor_val[0] = 0;		
	motor_val[1] = 0;		
	motor_val[2] = 250;		
	//[0] - 0: stop rotation, 1: start rotation
	//[1] - 0: dir == left, 1: dir == right
	//[2] - speed, it's value has from 10(fastest speed) to 250(slowest speed)




}


//dev_init: 전체 init
int __init dev_init(void) // 드라이버가 초기화 될 때 동작되는 함수
{
    int result;
    // 디바이스 드라이버 등록
    result = register_chrdev(DEV_MAJOR, DEV_NAME, &dev_fops);
    if(result <0) {
        // 실패하면 오류처리
        printk(KERN_WARNING"Can't get any major!\n");
        return result;
    }


	printk("dev_init called\n");

   
    //init global vars and flags
    sw_mode_select = 0;
    quit = 0;
	min = 0;
	sec = 0;
	quit_time_start = 0;

	printk("mode sel: %d, quit: %d, [%d][%d]\n", sw_mode_select, quit, min, sec);

	global_time = 0;

    //call init func
	init_variables();
    inter_open();
    kerneltimer_init();
    fnd_init();
	led_init();
	iom_led_init();
	iom_fpga_fnd_init();
	iom_fpga_dot_init();
	iom_fpga_text_lcd_init();
	step_motor_driver_init();
	buzzer_driver_init();

/*
	pid = fork();
	if(pid == -1){
		perror("fork error ");
		exit(0);
	}
	else if(pid == 0){
		//This code excuted by child process
		
	}
*/
	printk("dev_init success: \n");

    return 0;
}

//dev_exit: 전체 exit
void __exit dev_exit(void)
{

	printk("dev_exit called\n");

    //call exit func
    inter_release();
    kerneltimer_exit();
	fnd_exit();
	led_exit();
	iom_led_exit();
	iom_fpga_fnd_exit();
	iom_fpga_dot_exit();
	iom_fpga_text_lcd_exit();
	step_motor_driver_exit();
	buzzer_driver_exit();
 
    unregister_chrdev(DEV_MAJOR, DEV_NAME);
    printk("Removed DEV module\n");
} 


//module_init(inter_init);
//module_exit(inter_exit);

module_init( dev_init);
module_exit( dev_exit);
MODULE_LICENSE("GPL");
