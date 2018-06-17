
#include "hw_stm32f030_mini.h"
unsigned int state_led = 1;
unsigned int flag = 1;
void delay(unsigned int timeout)
{
   unsigned int t1, t2;
    for (t1 = 0; t1 < timeout; t1++)
    {
        for (t2 = 0; t2 < 0xFFF; t2++)
        {
          asm(" nop");
        }
    }
}

/*enable clock*/
void enable_clock(void)
{
	/*GPIOC*/
	unsigned long int tmpreg; \
    tmpreg = read_reg(RCC_AHBENR, ~RCC_AHBENR_IOPCEN);\
    tmpreg = tmpreg | RCC_AHBENR_IOPCEN; \
    write_reg(RCC_AHBENR, tmpreg);\
    delay(10); \
	/*GPIOA*/
	unsigned long int tmpreg1; \
    tmpreg1 = read_reg(RCC_AHBENR, ~RCC_AHBENR_IOPAEN);\
    tmpreg1 = tmpreg1 | RCC_AHBENR_IOPAEN; \
    write_reg(RCC_AHBENR, tmpreg1);\
    delay(10); \
	
}
/*set mode GC8 - GC9*/
void init_pin (void)
{
	/*init Pc8 Pc9*/
	unsigned long int tmpreg; 
    tmpreg = read_reg(GPIOC_MODER, ~(0x0fu << 16)); //reset port  pc9 pc8
    tmpreg = tmpreg | (0x05u << 16); 
    write_reg(GPIOC_MODER, tmpreg);
	/*init buttons PA0*/
    tmpreg = read_reg(GPIOA_MODER, ~(0x03u << 0)); //reset PA0
    tmpreg = tmpreg | (GPIO_MODE_INPUT << 0); 
    write_reg(GPIOA_MODER, tmpreg);
}

/*Init EXTI*/
void init_interrupt (void)
{
	/*enable interrupt for EXTI0*/
	unsigned long int tmpreg; 
    tmpreg = read_reg(EXTI_IMR, ~(0x01u << 0)); 
    tmpreg = tmpreg | (0x01u << 0); 
    write_reg(EXTI_IMR, tmpreg);
	
	tmpreg = read_reg(EXTI_RTSR, ~(0x01u << 0)); 
    tmpreg = tmpreg | (0x01u << 0); 
    write_reg(EXTI_RTSR, tmpreg);
	
	/*SYSCFG*/
    tmpreg = read_reg(SYSCFG_EXTICR1, ~(0x0fu << 0)); 
    tmpreg = tmpreg | (0x0u << 0); 
    write_reg(SYSCFG_EXTICR1, tmpreg);
	
	/*NVIC*/
	tmpreg = read_reg(NVIC_PRI1, ~(0xffu << 8)); 
    tmpreg = tmpreg | (0x01u << 14); 
    write_reg(NVIC_PRI1, tmpreg);
	
	tmpreg = read_reg(NVIC_ISER, ~(0x01u << 5)); 
    tmpreg = tmpreg | (0x01u << 5); 
    write_reg(NVIC_ISER, tmpreg);
	
	/*enable global interrupt*/
	asm ("cpsie i");
}

/*ON - OFF led*/
void turn_on (unsigned char pin)
{
	write_reg(GPIOC_OBSRR, 1 << pin);
}
void turn_off (unsigned char pin)
{
	write_reg(GPIOC_OBSRR, 1 << (pin + 16));
}

void main(void)
{
	/*enable clock*/
	enable_clock();
	/*init_pin*/
	init_pin();
	
	init_interrupt();
	int f1 = 0;
	int f2 = 1;
	int n = 0;*/
  while(1)
  {
	if (state_led)
	{
		turn_on(LD3_PIN);
		delay(0xff);
		n = f1 + f2;
		for (int i = 0; i < n; i++)
		{
			if (state_led)
			{
				turn_off(LD3_PIN);
				turn_on(LD4_PIN);
				delay(0x0f);
				turn_off(LD4_PIN);
				delay(0x0f);
			}
		}
		f1 = f2;
		f2 = n;
	}
  }
}
/*****************************************************************************************************************/
void EXTI0_1_IRQHandler(void)
{
	/*Interrupt PA0*/
	unsigned int temp = read_reg(EXTI_PR, 1 << 0);
	if (1 == temp)
	{
		if (flag) //stop
		{
			state_led = 0;
			turn_on (LD3_PIN); turn_on (LD4_PIN);
			flag = 0;
		}
		else //continue
		{
			state_led = 1;
			flag = 1;
		}
	}
	/*Xoa co ngat*/
	write_reg (EXTI_PR, (1 << 0)); //xoa co
	write_reg (NVIC_ICPR, (1 << 5)); //Xoa co NVIC
}

void Reserved_IRQHandler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void NMI_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void HardFault_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void SVC_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void PendSV_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}

void SysTick_Handler(void)
{
  while(1)
  {
    /* nothing to be run here */
  }
}


