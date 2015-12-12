#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

///msg types
#define	cmd_ACK	125	///	acknowledge
#define	cmd_FDI	129	///	full device info
#define	cmd_LDD	130	///	load device data
#define	cmd_MDD	131	///	measured data
#define	cmd_RCD	135	///	read	calib date
#define	cmd_RED	137	///	read	check date
#define	cmd_WCD	140	///	write	calib date
#define	cmd_WED	142	///	write	check date
#define	cmd_RMD	145	///	read	manufacturing date
#define	cmd_RPC	147	///	read	pcb version
#define	cmd_WMD	150	///	write	manufacturing date
#define	cmd_WPC	152	///	write	pcb version
#define	cmd_SUD	154	///	store device info
#define	cmd_RSN	161	///	read	serial number
#define	cmd_WSN	166	///	write	serial number
#define	cmd_RST	167	///	reset
#define	cmd_RSV	169	///	read	software version
#define	cmd_WSV	174	///	write	software version






typedef struct{
    uint8_t length;
    unsigned char data[32];
}UART_MSG;

typedef struct{
    uint8_t length;
	uint8_t type[3];
    uint8_t message[256];

}message_buffer;


int send_data(int data);
#endif
