#include "msp430xG46x.h"


volatile unsigned int A0results[8];

volatile unsigned int A1results[8];

volatile unsigned int A2results[8];

volatile unsigned int A3results[8];

unsigned int Index;

int button;




void main(void)

{

  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog

  P6SEL |= 0x0f;                            // Enable A/D inputs

  ADC12CTL0 = SHT0_8 + MSC + ADC12ON;       // Turn on ADC12, use int. osc.

                                            // extend sampling time so won't

// P6SEL |= 0x01; // P6.0 ADC option select

 P5DIR |= 0x02; // P5.1 output

 P6DIR  = 0x06;

 P6OUT  = 0x02;                                        // get overflow




 P3DIR|=0x03;

 P2DIR|=0x03;

 P1DIR =0x00;

 P1IE=0x01;

 P1IES=0x01;

 button=0;

                                            // Set MSC so conversions triggered

                                            // automatically

  ADC12CTL1 = SHP + CONSEQ_3;               // Use sampling timer, set mode

  ADC12IE = 0x08;                           // Enable ADC12IFG.3 for ADC12MEM3

  ADC12MCTL0 = INCH_0;                      // A0 goes to MEM0

  ADC12MCTL1 = INCH_1;                      // A1 goes to MEM1

  ADC12MCTL2 = INCH_2;                      // A2 goes to MEM2

  ADC12MCTL3 = EOS + INCH_3;                // A3 goes to MEM3, end of sequence




  while (1)
  {

    ADC12CTL0 |= ENC;                       // Enable conversions

    ADC12CTL0 |= ADC12SC;                   // Start conversions

    __bis_SR_register(LPM0_bits + GIE);     // LPM0
  }

}




#pragma vector = ADC12_VECTOR

__interrupt void ADC12_ISR(void)
{

	if(button)
	{

		if (ADC12MEM0 >= 0x750 && ADC12MEM1 <= 0x750 && ADC12MEM2 <= 0x430) // ADC12MEM = A0 > 0.5AVcc?
		{

			P5OUT |= 0x02; // P5.1 = 1

			P3OUT=0x01;
	
		 	P2OUT=0x01;

	 	}

		else if(ADC12MEM1 > 0x750)
		{

	 		P5OUT &= ~0x02; // P5.1 = 0

	 		P3OUT=0x00;

	 		P2OUT=0x01;

	 	}

		else if(ADC12MEM2 > 0x430)
		{

	 		P5OUT &= ~0x02; // P5.1 = 0

	 		P3OUT=0x01;

	 		P2OUT=0x00;

	 	}

	}

	else
	{

	 P3OUT=0x00;

	 P2OUT=0x00;

	}

  	A0results[Index] = ADC12MEM0;             // Move A0 results, IFG is cleared

	A1results[Index] = ADC12MEM1;             // Move A1 results, IFG is cleared

	A2results[Index] = ADC12MEM2;             // Move A2 results, IFG is cleared

	A3results[Index] = ADC12MEM3;             // Move A3 results, IFG is cleared

	Index = (Index + 1) & 0x7;                // Increment results index, modulo

	__no_operation();                         // SET BREAKPOINT HERE

	__bic_SR_register_on_exit(LPM0_bits);     // Exit LPM0

}




#pragma vector=PORT1_VECTOR

__interrupt void gpio_port1_isr(void)
{

    P1IFG=0x00;

    button = ~button;

}
