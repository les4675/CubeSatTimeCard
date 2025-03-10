#include <msp430.h>

//Setup: GNSS on P2.1 (CCI1A), CSAC on P2.4 (CCI2A)

// Global variables for timer overflow and capture timestamps
volatile unsigned long timerOvCount = 0;
volatile unsigned long pps1_timestamp = 0; // GNSS PPS timestamp from P2.1 interrupt
volatile unsigned long pps2_timestamp = 0; // CSAC PPS timestamp from P2.0 interrupt
volatile unsigned char pps1_captured = 0;
volatile unsigned char pps2_captured = 0;
volatile unsigned long diff = 0;

volatile unsigned int gnss_capVal = 0, gnss_ovVal = 0, gnss_capVal_report = 0, gnss_ovVal_report = 0;
volatile unsigned int csac_capVal = 0, csac_ovVal = 0, csac_capVal_report = 0, csac_ovVal_report = 0;
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


    __enable_interrupt();   // Enable global interrupts

    while (1) {
        if (reportValues)
        {
            /*
            // Compute the absolute phase difference in timer counts.
            if (pps1_timestamp > pps2_timestamp)
                diff = pps1_timestamp - pps2_timestamp;
            else
                diff = pps2_timestamp - pps1_timestamp;
            */
            gnss_capVal_report = gnss_capVal;
            gnss_ovVal_report = gnss_ovVal;
            csac_capVal_report = csac_capVal;
            csac_ovVal_report = csac_ovVal;


            uart_send_string("GNSS,");
            utoa(gnss_capVal, buf);
            uart_send_string(buf);
            uart_send_string(",");
            utoa(gnss_ovVal, buf);
            uart_send_string(buf);
            uart_send_string("CSAC,");
            utoa(csac_capVal, buf);
            uart_send_string(buf);
            uart_send_string(",");
            utoa(csac_ovVal, buf);
            uart_send_string(buf);
            uart_send_string("\r\n");

            reportValues = 0;

            // Reset flags for the next measurement.
            pps1_captured = 0;
            pps2_captured = 0;
        }
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
    UCA0CTL1 |= UCSSEL_2;      // Use SMCLK

    // Baud rate configuration for 9600 baud (10MHz/9600 = 1041.67, 1041.67/16 = 65 + 2/16)
    UCA0BR0 = 65;
    UCA0MCTL = UCBRF_2 | UCBRS0 | UCOS16;

    UCA0CTL1 &= ~UCSWRST;      // Release state machine
}

void timer_init(void)
{
    // Set up Timer_A as a free-running counter in continuous mode using SMCLK,
    // and enable its overflow interrupt to extend the 16-bit counter.
    TA1CTL = TASSEL_2 | MC_2 | TAIE;
    TA1CCTL1 = CM1 | SCS | CAP | CCIE;  //Capture mode on rising edge of CCI1A (P2.1), enable interrupt
    TA1CCTL2 = CM1 | SCS | CAP | CCIE;  //Capture mode on rising edge of CCI2A (P2.4), enable interrupt

    P2SEL |= BIT1 | BIT4; //Configure P2.1 and P2.4 as CCIxA pins for timer A1

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

//Interrupt handler for captures and overflow
#pragma vector = 0xFFF8
__interrupt void Timer_A1_ISR(void)
{
    switch (TA1IV)
    {
        case(2):
            gnss_capVal = TACCR1;
            gnss_ovVal = timerOvCount;
            break;
        case(4):
            csac_capVal = TACCR2;
            csac_ovVal = timerOvCount;
            break;
        case(0xA):
            timerOvCount++;
            //Resets overflow counter and reports values after 0.996s
            if (timerOvCount >= 152)
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
