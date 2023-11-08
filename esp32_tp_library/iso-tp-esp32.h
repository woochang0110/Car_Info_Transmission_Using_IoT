#ifndef _ISOTP_H
#define _ISOTP_H

#include <esp32_can.h>

//#define ISO_TP_DEBUG

typedef enum {
  ISOTP_IDLE = 0,
  ISOTP_SEND,
  ISOTP_SEND_FF,
  ISOTP_SEND_CF,
  ISOTP_WAIT_FIRST_FC,
  ISOTP_WAIT_FC,
  ISOTP_WAIT_DATA,
  ISOTP_FINISHED,
  ISOTP_ERROR
} isotp_states_t;

#define CAN_MAX_DLEN 8  //Not extended CAN

/* N_PCI type values in bits 7-4 of N_PCI bytes */
#define N_PCI_SF  0x00  /* single frame */
#define N_PCI_FF  0x10  /* first frame */
#define N_PCI_CF  0x20  /* consecutive frame */
#define N_PCI_FC  0x30  /* flow control */

#define FC_CONTENT_SZ 3 /* flow control content size in byte (FS/BS/STmin) */

/* Flow Status given in FC frame */
#define ISOTP_FC_CTS  0   /* clear to send */
#define ISOTP_FC_WT 1     /* wait */
#define ISOTP_FC_OVFLW  2 /* overflow */

/* Timeout values */
#define TIMEOUT_SESSION  500 /* Timeout between successfull send and receive */
#define TIMEOUT_FC       250 /* Timeout between FF and FC or Block CF and FC */
#define TIMEOUT_CF       250 /* Timeout between CFs                          */
#define MAX_FCWAIT_FRAME  10   

#define MAX_MSGBUF 128    /* Received Message Buffer. Depends on uC ressources!
                             Should be enough for our needs */
struct Message_t
{
  INT8U len=0;
  isotp_states_t tp_state=ISOTP_IDLE;
  INT8U seq_id=1;
  INT8U fc_status=ISOTP_FC_CTS;
  INT8U blocksize=0;
  INT8U min_sep_time=0;
  INT32U tx_id=0;
  INT32U rx_id=0;
  INT8U *Buffer;
};

class IsoTp
{
	public:
		IsoTp(ESP32_CAN* bus);
		INT8U send(Message_t* msg);
		INT8U receive(Message_t* msg);
		void    print_buffer(INT32U id, INT8U *buffer, uint16_t len);
	private:
		ESP32_CAN* _bus;
		INT32U rxId;
		INT8U  rxLen;
		INT8U  rxBuffer[8];
    uint16_t rest;
		INT8U  fc_wait_frames=0;
		INT32U wait_fc=0;
		INT32U wait_cf=0;
    INT32U wait_session=0;
		INT8U  can_send(INT32U id, INT8U len, INT8U *data);
		INT8U  can_receive(void);
		INT8U  send_fc(struct Message_t* msg);
		INT8U  send_sf(struct Message_t* msg);
		INT8U  send_ff(struct Message_t* msg);
		INT8U  send_cf(struct Message_t* msg);
		INT8U  rcv_sf(struct Message_t* msg);
		INT8U  rcv_ff(struct Message_t* msg);
		INT8U  rcv_cf(struct Message_t* msg);
		INT8U  rcv_fc(struct Message_t* msg);
    void     fc_delay(INT8U sep_time);
};
                             
#endif
