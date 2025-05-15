#ifndef UART
#define UART


extern void UART0_Init (void);
extern void UART2_Init (void);
extern void UART5_Init (void);
extern void UART1_Init (void);
extern void UART0_SendChar (char data);
extern void UART0_SendString (char *pt);
extern char UART0_Recievechar (void);
extern void UART0_RecieveString (char *command, int length);
extern char UART1_Recievechar (void);
extern void UART2_SendChar (char data);
extern void UART2_SendString (char *pt);
extern char UART2_Recievechar (void);
extern void UART2_RecieveString (char *command, int length);

extern void UART5_SendChar (char data);
extern void UART5_SendString (char *pt);
extern char UART5_Recievechar (void);
extern void UART5_RecieveString (char *command, int length);



#define UART0                   0
#define UART1                   1
#define UART2                   2
#define UART3                   3
#define UART4                   4
#define UART5                   5
#define UART6                   6
#define UART7                   7

#define UART_PARITY_NONE        0
#define UART_PARITY_ODD         1
#define UART_PARITY_EVEN        2

#define UART_DATA_5				5
#define UART_DATA_6				6
#define UART_DATA_7				7
#define UART_DATA_8				8

#define UART_STOP_BIT_1			1
#define UART_STOP_BIT_2			2

#define UART_BDR_2400			2400
#define UART_BDR_4800			4800	
#define UART_BDR_9600			9600
#define UART_BDR_14400			14400
#define UART_BDR_19200			19200
#define UART_BDR_28800			28800
#define UART_BDR_38400			38400
#define UART_BDR_57600			57600
#define UART_BDR_76800			76800
#define UART_BDR_115200			115200
#define UART_BDR_230400			230400
#define UART_BDR_250000			250000
/************************************< FUNCTIONS ***********************************************/
#define GPIO_PA0_U0RX			0x00000001
#define GPIO_PA1_U0TX			0x00000002
/**< UART1 */
#define GPIO_PB0_U1RX			0x00000001
#define GPIO_PB1_U1TX			0x00000002
/**< UART2 */
#define GPIO_PD6_U2RX			0x00000040			
#define GPIO_PD7_U2TX			0x00000080
/**< UART3 */
#define GPIO_PC6_U3RX			0x00000040			
#define GPIO_PC7_U3TX			0x00000080
/**< UART4 */
#define GPIO_PC4_U4RX			0x00000010
#define GPIO_PC5_U4TX			0x00000020
/**< UART5 */
#define GPIO_PE4_U5RX			0x00000010
#define GPIO_PE5_U5TX			0x00000020
/**< UART6 */
#define GPIO_PD4_U6RX			0x00000010
#define GPIO_PD5_U6TX			0x00000020
/**< UART7 */
#define GPIO_PE0_U7RX			0x00000001
#define GPIO_PE1_U7TX			0x00000002

#define UART_POARTA				0
#define UART_POARTB				1
#define UART_POARTC				2
#define UART_POARTD				3
#define UART_POARTE				4

#define UART_LCRH_WLEN_S		5

#endif
