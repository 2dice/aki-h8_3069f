#include "defines.h"
#include "serial.h"

#define SERIAL_SCI_NUM 3

#define H8_3069F_SCI0 ((volatile struct h8_3069f_sci *)0xffffb0)
#define H8_3069F_SCI1 ((volatile struct h8_3069f_sci *)0xffffb8)
#define H8_3069F_SCI2 ((volatile struct h8_3069f_sci *)0xffffc0)

struct h8_3069f_sci {
  volatile uint8 smr;
  volatile uint8 brr;
  volatile uint8 scr;
  volatile uint8 tdr;
  volatile uint8 ssr;
  volatile uint8 rdr;
  volatile uint8 scmr;
};

#define H8_3069F_SCI_SMR_CKS_PER1  (0<<0)
#define H8_3069F_SCI_SMR_CKS_PER4  (1<<0)
#define H8_3069F_SCI_SMR_CKS_PER16 (2<<0)
#define H8_3069F_SCI_SMR_CKS_PER64 (3<<0)
#define H8_3069F_SCI_SMR_MP     (1<<2)
#define H8_3069F_SCI_SMR_STOP   (1<<3)
#define H8_3069F_SCI_SMR_OE     (1<<4)
#define H8_3069F_SCI_SMR_PE     (1<<5)
#define H8_3069F_SCI_SMR_CHR    (1<<6)
#define H8_3069F_SCI_SMR_CA     (1<<7)

#define H8_3069F_SCI_SCR_CKE0   (1<<0)
#define H8_3069F_SCI_SCR_CKE1   (1<<1)
#define H8_3069F_SCI_SCR_TEIE   (1<<2)
#define H8_3069F_SCI_SCR_MPIE   (1<<3)
#define H8_3069F_SCI_SCR_RE     (1<<4) /* 受信有効 */
#define H8_3069F_SCI_SCR_TE     (1<<5) /* 送信有効 */
#define H8_3069F_SCI_SCR_RIE    (1<<6) /* 受信割込み有効 */
#define H8_3069F_SCI_SCR_TIE    (1<<7) /* 送信割込み有効 */

#define H8_3069F_SCI_SSR_MPBT   (1<<0)
#define H8_3069F_SCI_SSR_MPB    (1<<1)
#define H8_3069F_SCI_SSR_TEND   (1<<2)
#define H8_3069F_SCI_SSR_PER    (1<<3)
#define H8_3069F_SCI_SSR_FERERS (1<<4)
#define H8_3069F_SCI_SSR_ORER   (1<<5)
#define H8_3069F_SCI_SSR_RDRF   (1<<6) /* 受信完了 */
#define H8_3069F_SCI_SSR_TDRE   (1<<7) /* 送信完了 */

static struct {
  volatile struct h8_3069f_sci *sci;
} regs[SERIAL_SCI_NUM] = {
  { H8_3069F_SCI0 }, 
  { H8_3069F_SCI1 }, 
  { H8_3069F_SCI2 }, 
};

#define SCI0_SSR (*(volatile uint8*)0xffffbc)
#define SCI0_SCR (*(volatile uint8*)0xffffba)
#define SCI0_BRR (*(volatile uint8*)0xffffb9)
#define SCI0_SMR (*(volatile uint8*)0xffffb8)

int disable_SCI0_TxRx(void)
    {
      SCI0_SCR = SCI0_SCR & ~0b00110000;
      return 0;
    }

int disable_SCI0_serial_interrupt(void)
    {
      SCI0_SCR = SCI0_SCR & ~0b11001100;
      return 0;
    }

int set_SCI0_clock_source_and_SCK_port_status(void)
    {
      SCI0_SCR = SCI0_SCR & ~0b00000011;
      return 0;
    }

int set_SCI0_serial_modes(void)
    {
      SCI0_SMR = SCI0_SMR & ~0b11111111;
      return 0;
    }

int set_SCI0_bitrate(void)
    {
      SCI0_BRR = 64;
      return 0;
    }

int enable_SCI0_serial_interrept(void)
    {
      SCI0_SCR = SCI0_SCR | 0b11000100;
      return 0;
    }

int enable_SCI0_TxRx(void)
    {
      SCI0_SCR = SCI0_SCR | 0b00110000;
      return 0;
    }

int serial_init(void)
{
  disable_SCI0_TxRx();
  disable_SCI0_serial_interrupt();

  set_SCI0_clock_source_and_SCK_port_status();
  set_SCI0_serial_modes();
  set_SCI0_bitrate();

  enable_SCI0_serial_interrept();
  enable_SCI0_TxRx();
  
  return 0;
}

/* 送信可能か？ */
int serial_is_send_enable(int index)
{
  volatile struct h8_3069f_sci *sci = regs[index].sci;
  return (sci->ssr & H8_3069F_SCI_SSR_TDRE);
}

/* １文字送信 */
int serial_send_byte(int index, unsigned char c)
{
  volatile struct h8_3069f_sci *sci = regs[index].sci;

  /* 送信可能になるまで待つ */
  while (!serial_is_send_enable(index))
    ;
  sci->tdr = c;
  sci->ssr &= ~H8_3069F_SCI_SSR_TDRE; /* 送信開始 */

  return 0;
}
