/*********************************************************************
* File Name     : CanLib.h
* Description   : Header file for CanLib.c
* Date          : 20/07/2018
*********************************************************************/

#ifndef CANLIB_H
#define CANLIB_H

#include <p30f5011.h>
#include "types.h"

/*--- Can message structure. ---*/

typedef struct
  {
  uint32_t eid;
  uint8_t length;
  uint8_t data[8];
  }CANMSG;

/*--- CanLib function prototypes. ---*/

void CanInit(void);                                              // Call this function first(this puts the can in configuration mode)
void CanSetMask(uint8_t mask_no, uint16_t mask);                 // Then set up masks (right now mask settings are only for SID....EID will be added soon)
void CanSetFilter(uint8_t filter_no, uint16_t sid, uint32_t eid) // and filters(can be set to receive both SID and EID)
void CanConfig(void);                                            // Then call this function last, to enable can(putting up can in normal mode).
void CanSetPriority(uint8_t buffer, uint8_t priority);           // set the message transmission priority
void CanSendMessage(const CANMSG *message);                      // Send the message on the bus
Bool CanReceiveMessage(CANMSG *message);                         // Returns True if message received.

/*--- End of file. ---*/

#endif
