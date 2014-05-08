#ifndef _SERIAL_H_INCLUDED_
#define _SERIAL_H_INCLUDED_
int disable_SCI0_TxRx(void);
int disable_SCI0_serial_interrupt(void);
int set_SCI0_clock_source_and_SCK_port_status(void);
int set_SCI0_serial_modes(void);
int set_SCI0_bitrate(void);
int enable_SCI0_serial_interrept(void);
int enable_SCI0_TxRx(void);

int set_SCI0_sending(void);

int serial_init(void); 
int serial_is_send_enable(int index); 
int serial_send_byte(unsigned char b); 

#endif
