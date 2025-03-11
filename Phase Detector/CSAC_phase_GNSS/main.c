#include <msp430.h>

//Setup: GNSS on P2.1 (CCI1A), CSAC on P2.4 (CCI2A)

// Global variables for timer overflow and capture timestamps
volatile unsigned long timerOvCount = 0;
volatile unsigned long pps1_timestamp = 0; // GNSS PPS timestamp from P2.1 interrupt
volatile unsigned long pps2_timestamp = 0; // CSAC PPS timestamp from P2.0 interrupt
volatile unsigned char gnss_captured = 0;
volatile unsigned char csac_captured = 0;
volatile unsigned long diff = 0;

volatile unsigned long gnss_capVal = 0, gnss_ovVal = 0, gnss_capVal_report = 0, gnss_ovVal_report = 0;
volatile unsigned long csac_capVal = 0, csac_ovVal = 0, csac_capVal_report = 0, csac_ovVal_report = 0;
volatile char reportValues = 0;

// UART function prototypes
void CLK_init(void);
void uart_init(void);
void timer_init(void);
void uart_send_char(char c);
void uart_send_string(const char *str);
void utoa(unsigned long value, char *str);

int main(void) {
    char buf[12];

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    //MCLK and SMCLK is set to 10MHz
    CLK_init();

    uart_init();

    timer_init();

    __enable_interrupt();   // Enable global interrupts

    while (1) {
        if (reportValues)
        {
            reportValues = 0;
            /*
            // Compute the absolute phase difference in timer counts.
            if (pps1_timestamp > pps2_timestamp)
                diff = pps1_timestamp - pps2_timestamp;
            else
                diff = pps2_timestamp - pps1_timestamp;
            */
            if (gnss_captured && csac_captured)
            {
                gnss_captured = 0;
                csac_captured = 0;
                gnss_capVal_report = gnss_capVal;
                gnss_ovVal_report = gnss_ovVal;
                csac_capVal_report = csac_capVal;
                csac_ovVal_report = csac_ovVal;


                uart_send_string("GNSS,");
                utoa(gnss_capVal_report, buf);
                uart_send_string(buf);
                uart_send_string(",");
                utoa(gnss_ovVal_report, buf);
                uart_send_string(buf);
                uart_send_string(",CSAC,");
                utoa(csac_capVal_report, buf);
                uart_send_string(buf);
                uart_send_string(",");
                utoa(csac_ovVal_report, buf);
                uart_send_string(buf);
                uart_send_string("\r\n");

                // Reset flags for the next measurement.
            }
            else
            {
                if (!csac_captured)
                {
                    uart_send_string("%CSAC NOT CAPTURED\r\n");
                }
                if (!gnss_captured)
                {
                    uart_send_string("%GNSS NOT CAPTURED\r\n");
                }
            }

        }
        _nop();
    }
}


//Set SMCLK and MCLK to source from
//10MHz on LFXT1 from XIN Pin
void CLK_init(void)
{
    //Select MCLK and SMCLK to source from LFXT1CLK
    BCSCTL2 |= SELM_3 | SELS;

    //Set LFXT1CLK to source from external
    BCSCTL3 |= LFXT1S_3;
    BCSCTL3 &= ~XCAP_3;

    //Set ACLK to be 10MHz/8 = 1.25MHz
    BCSCTL1 |= DIVA_3;

    //Set P2.6 (Pin 19) to be an input for XIN
    P2SEL |= BIT6 | BIT7;
    P2SEL2 |= BIT6 | BIT7;
}

// UART initialization for 9600 baud using SMCLK (~1MHz)
void uart_init(void) {
    // Configure UART pins: P1.1 (RX) and P1.2 (TX)
    P1SEL  |= BIT1 | BIT2;
    P1SEL2 |= BIT1 | BIT2;

    UCA0CTL1 |= UCSWRST;       // Put state machine in reset
    UCA0CTL1 |= UCSSEL_1;      // Use ACLK

    // Baud rate configuration for 9600 baud (10MHz/9600 = 1041.67, 1041.67/16 = 65 + 2/16)
    UCA0BR0 = 208;
    UCA0MCTL = UCBRS_3;

    UCA0STAT |= UCLISTEN;

    UCA0CTL1 &= ~UCSWRST;      // Release state machine
}

void timer_init(void)
{
    // Set up Timer_A as a free-running counter in continuous mode using SMCLK,
    // and enable its overflow interrupt to extend the 16-bit counter.
    TA1CTL = TASSEL_2 | TAIE;
    TA1CCTL0 = CM0 | SCS | CAP | CCIE;  //Capture mode on rising edge of CCI1A (P2.1), enable interrupt
    TA1CCTL2 = CM0 | SCS | CAP | CCIE;  //Capture mode on rising edge of CCI2A (P2.4), enable interrupt

    P2SEL |= BIT0 | BIT4; //Configure P2.1 and P2.4 as CCIxA pins for timer A1

    TA1CTL |= MC_2;
    /*
    // Configure CSAC PPS on P2.0 (external interrupt)
    P2DIR &= ~BIT0;      // Input
    P2IE  |= BIT0;       // Enable interrupt on P2.0
    P2IES &= ~BIT0;      // Trigger on rising edge
    P2IFG &= ~BIT0;      // Clear flag

    // Configure GNSS PPS on P2.1 (external interrupt)
    P2DIR &= ~BIT1;      // Input
    P2IE  |= BIT1;       // Enable interrupt on P2.1
    P2IES &= ~BIT1;      // Trigger on rising edge
    P2IFG &= ~BIT1;      // Clear flag
    */
}

// Transmit a single character over UART
void uart_send_char(char c) {
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = c;
}

// Transmit a null-terminated string over UART
void uart_send_string(const char *str) {
    while (*str)
        uart_send_char(*str++);
}

// Convert an unsigned long integer to a decimal string
void utoa(unsigned long value, char *str) {
    char buf[12];
    int i = 0;
    int j = 0;
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    while (value > 0 && i < 11) {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }
    // Reverse the string.
    for (j = 0; j < i; j++)
        str[j] = buf[i - 1 - j];
    str[i] = '\0';
}
//Interrupt handler for capture0
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A0_ISR(void)
{
    _nop();
    gnss_captured = 1;
    gnss_capVal = TA1CCR0;
    gnss_ovVal = timerOvCount;
    TA1CCR0 = 0;
    TA1CCTL0 &= ~COV;
}

//Interrupt handler for capture2 and overflow
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer_A1_ISR(void)
{
    switch (TA1IV)
    {
        /*case(2):
            _nop();
            gnss_capVal = TA1CCR1;
            gnss_ovVal = timerOvCount;
            TA1CCTL1 &= ~COV;
            TA1CCTL1 &= ~CCIE;
            break;*/
        case(4):
            _nop();
            csac_captured = 1;
            csac_capVal = TA1CCR2;
            csac_ovVal = timerOvCount;
            TA1CCTL2 &= ~COV;

            break;
        case(0xA):
            _nop();
            timerOvCount++;
            //Resets overflow counter and reports values after 0.99942s
            if (timerOvCount >= 244)
            {
                timerOvCount = 0;
                reportValues = 1;
            }
            break;
        default:
            break;
    }
}


/*
// Port 2 ISR handles both P2.0 and P2.1 interrupts.
#pragma vector = PORT2_VECTOR
__interrupt void Port_2_ISR(void) {
    // CSAC PPS on P2.0
    if (P2IFG & BIT0) {
        pps2_timestamp = (((unsigned long)timer_overflow_count) << 16) | TA0R;
        pps2_captured = 1;
        P2IFG &= ~BIT0; // Clear flag for P2.0
    }
    // GNSS PPS on P2.1
    if (P2IFG & BIT1) {
        pps1_timestamp = (((unsigned long)timer_overflow_count) << 16) | TA0R;
        pps1_captured = 1;
        P2IFG &= ~BIT1; // Clear flag for P2.1
    }
} */
