#include "CanLib.h"

/*--- Function prototypes ---*/

#define FCY            10000000*4/4 //FCY=ext_clk_freq*XT_PLLx/4


_FOSC(CSW_FSCM_OFF & XT_PLL4);
_FWDT(WDT_OFF);                 /* Turn off the Watch-Dog Timer.  */
_FBORPOR(PBOR_OFF & MCLR_EN);
_FGS(CODE_PROT_OFF);            /* Disable Code Protection */


/*********************************************************************
* Function Name : delay_ms
* Description   : generate desired delay in ms

*********************************************************************/
void delay_ms(unsigned int delay)
{
  unsigned int i,j;
  for(i=0;i<delay;i++)
  {
  for(j=0;j<1275;j++);
  }
}


/*********************************************************************
* Function Name : CanInit
* Description   : Put CAN into configuration mode.

*********************************************************************/
void CanInit(void)
  {
  C1CTRLbits.REQOP = 4;           //Request configuration mode
  while(C1CTRLbits.OPMODE != 4){  //Wait until configuration mode entered
    ;
    }
  }

/*********************************************************************
* Function Name : CanConfig
* Description   : Sets baud rate to 500Kbps with 20Tq for a clock of 10MHz
* Note : use any online baudratre calculator (http://www.bittiming.can-wiki.info/)
         directly set the C1CFG1/2 registers or set the individual bit.
*********************************************************************/

void CanConfig(void)
  {
	C1CFG1=0x0000;
	C1CFG2=0x0086;
  /*C1CFG1bits.BRP = 1;      //10 Meg clock
  C1CFG1bits.SJW = 3;        //Synchronized jump width time is 4Tq.
  C1CFG2bits.PRSEG = 7;      //Propagation time segment = 8Tq.
  C1CFG2bits.SEG1PH = 5;     //Phase buffer Segment 1 = 6Tq.
  C1CFG2bits.SAM = 0;        //Bus is sampled once.
  C1CFG2bits.SEG2PHTS = 1;   //PhaseSeg2 is freely programmable.
  C1CFG2bits.SEG2PH = 4;     //Phase buffer Segment 2 = 5Tq. */
  C1CTRL = 0x8800;           //Can clock is Fcy = 10MHz. Request normal operation mode.
  //Note:F_CAN for must not exceed 30 MHz. If CANCKS = 0, then F_CY must not exceed 7.5 MHz.
  //so accordingly set the CANCKS bit
  while(C1CTRLbits.OPMODE != 0){
    ;
    }
  }

/*********************************************************************
* Function Name : CanSetFilter
* Description   : This function sets the acceptance filter, SID for
                  standard length identifiers.

* Parameters    : uint16_t: filter_no:  (Filter number 0-5)
*                 uint16_t: sid:        (SID value)
*                 uint32_t: eid:        (EID value)
*********************************************************************/

void CanSetFilter(uint8_t filter_no, uint16_t sid, uint32_t eid)
  {
  uint32_t uppereid;
	sid = sid <<2;
  uppereid = eid >> 6;
  switch(filter_no)
    {
    case 0: C1RXF0SID = sid;
			      C1RXF0SIDbits.EXIDE = 1;
            C1RXF0EIDH = uppereid;
            eid = eid <<10;
            C1RXF0EIDL = eid;
            break;
    case 1: C1RXF1SID = sid;
			      C1RXF1SIDbits.EXIDE = 1;
            C1RXF1EIDH = uppereid;
            eid = eid <<10;
            C1RXF1EIDL = eid;
            break;
    case 2: C1RXF2SID = sid;
			      C1RXF2SIDbits.EXIDE = 1;
            C1RXF2EIDH = uppereid;
            eid = eid <<10;
            C1RXF2EIDL = eid;
            break;
    case 3: C1RXF3SID = sid;
			      C1RXF3SIDbits.EXIDE = 1;
            C1RXF3EIDH = uppereid;
            eid = eid <<10;
            C1RXF3EIDL = eid;
            break;
    case 4: C1RXF4SID = sid;
			      C1RXF4SIDbits.EXIDE = 1;
            C1RXF4EIDH = uppereid;
            eid = eid <<10;
            C1RXF4EIDL = eid;
            break;
    case 5: C1RXF5SID = sid;
			      C1RXF5SIDbits.EXIDE = 1;
            C1RXF5EIDH = uppereid;
            eid = eid <<10;
            C1RXF5EIDL = eid;
            break;

    default:C1RXF0SID = sid;
			      C1RXF0SIDbits.EXIDE = 1;
            C1RXF0EIDH = uppereid;
            eid = eid <<10;
            C1RXF0EIDL = eid;
            break;
    }
  }

/*************************************************************************
* Function Name : CanSetMask
* Description   : This function sets the values for the acceptance
*                 filter masks for standard length identifiers.

* Parameters    : uint16_t: mask_no:  (Mask number 0-1)
*                 uint16_t: mask:     (Mask value)
**************************************************************************/

void CanSetMask(uint8_t mask_no, uint32_t mask)
  {
  switch(mask_no)
    {
    case 0: C1RXM0SID = 0x0001;
            C1RXM0SIDbits.SID = mask;
            break;
    case 1: C1RXM1SID = 0x0001;
            C1RXM1SIDbits.SID = mask;
            break;

    default:C1RXM0SID = 0x0001;
            C1RXM0SIDbits.SID = mask;
            break;
  }
}

/*************************************************************************
* Function Name : CanSetPriority
* Description   : This function sets the message transmission
*                 priority bits.

* Parameters    : uint16_t: buffer:   (Transmit buffer number 0-3)
                  uint16_t: priority: (Transmit priority 0-3)
**************************************************************************/

void CanSetPriority(uint8_t buffer, uint8_t priority)
  {
  priority = (priority > 3U) ? 3U : priority;

  switch(buffer)
    {
    case 0: C1TX0CONbits.TXPRI = priority;
            break;
    case 1: C1TX1CONbits.TXPRI = priority;
            break;
    case 2: C1TX2CONbits.TXPRI = priority;
            break;
    default:
            break;
    }
  }

/*********************************************************************
* Function Name : CanSendMessage
* Description   : This function writes the message identifier(SID),
                  writes the data to be transmitted into the Transmit buffer
*                 and sets the corresponding Transmit request bit.

* Parameters    : Pointer to structure CANMSG defined in can.h
*********************************************************************/

void CanSendMessage(const CANMSG *message)
  {
  uint8_t buffer;
  uint16_t ix = ((message->eid & 0x07C0) << 5) | ((message->eid & 0x003F) << 2);

  buffer = getTxbuffer();

  switch(buffer)
    {
    case 0: C1TX0SID = ix;
            break;
    case 1: C1TX1SID = ix;
            break;
    case 2: C1TX2SID = ix;
            break;

    default:C1TX0SID = ix;
            break;
    }

  for(ix = 0; ix < message->length; ix++)
    {
    switch(buffer)
      {
      case 0: *((uint8_t *)&C1TX0B1+ix)= message->data[ix];
              break;
      case 1: *((uint8_t *)&C1TX1B1+ix)= message->data[ix];
              break;
      case 2: *((uint8_t *)&C1TX2B1+ix)= message->data[ix];
              break;
      default:*((uint8_t *)&C1TX0B1+ix)= message->data[ix];
              break;
      }
    }

  switch(buffer)
    {
    case 0: C1TX0DLC = 0x0180;
            C1TX0DLCbits.DLC = message->length;
            C1TX0CONbits.TXREQ = 1;
            break;
    case 1: C1TX1DLC = 0x0180;
            C1TX1DLCbits.DLC = message->length;
            C1TX1CONbits.TXREQ = 1;
            break;
    case 2: C1TX2DLC = 0x0180;
            C1TX2DLCbits.DLC = message->length;
            C1TX2CONbits.TXREQ = 1;
            break;
    default:break;
    }
  }

/*************************************************************************
* Function Name : CanReceiveMessage
* Description   : If a message has been received, read the data from
                  the receive buffer into the structure CANMSG
                  and clear the RXFUL bit.

* Parameters    : Pointer to structure CANMSG defined in can.h
* Returns       : True if message received else False.
**************************************************************************/

Bool CanReceiveMessage(CANMSG *message)
  {
  uint8_t ix, buffer = 0xff;
  uint32_t SID,EID,DLC;
uint32_t var = 0;

if(C1RX0CONbits.RXFUL){
  buffer = 0;
  SID = C1RX0SID;
  EID = C1RX0EID;
  DLC = C1RX0DLC;
}
else if(C1RX1CONbits.RXFUL){
  buffer = 1;
  SID = C1RX1SID;
  EID = C1RX1EID;
  DLC = C1RX1DLC;
  }

      //SID = ((SID >> 2)<<18);
      SID = (SID >> 2);
      SID = (SID <<18);
      var = SID;
      EID = (EID << 6);
      var = var | EID;
      DLC = DLC >> 11;
      var = var | DLC;

  if(buffer != 0xff)
    {
    switch(buffer)
      {
        case 0: message->eid = var;
                message->length = C1RX0DLCbits.DLC;
                break;
        case 1: message->eid = var;
                message->length = C1RX1DLCbits.DLC;
                break;
        default:
                break;
      }

    for(ix = 0; ix < message->length; ix++)
      {
      switch(buffer)
        {
        case 0: message->data[ix] = *((uint8_t *)&C1RX0B1 + ix);
                break;
        case 1: message->data[ix] = *((uint8_t *)&C1RX1B1 + ix);
                break;
        default:
              break;
        }
      }

    switch(buffer)
      {
      case 0: C1RX0CONbits.RXFUL = 0;
              break;
      case 1: C1RX1CONbits.RXFUL = 0;
              break;
      default:
              break;
      }
    }

  return (buffer != 0xff) ? True : False;
  }

/*************************************************************************
* Function Name : getTxbuffer
* Description   : Returns an available empty transmit buffer.

* Returns       : Transmit buffer number 0-2.
**************************************************************************/

static uint8_t getTxbuffer(void)
  {
  uint8_t buffer = 0;

  if(C1TX0CONbits.TXREQ == 0){
    buffer = 0;
    }
  else if(C1TX1CONbits.TXREQ == 0){
    buffer = 1;
    }
  else if(C1TX2CONbits.TXREQ == 0){
    buffer = 2;
    }

  return buffer;
  }

/*--- End of file.---*/
int main()
{
float dtc;
int i=0;
CANMSG tx;
CanInit();
CanSetMask(0, 0);
CanSetFilter(0, 0b11000111111,0b101110111000000000);
CanConfig();

tx.eid=0x121;
tx.length = 8;
tx.data[0]=1;
tx.data[1]=1;
tx.data[2]=1;
tx.data[3]=1;
tx.data[4]=1;
tx.data[5]=1;
tx.data[6]=1;
tx.data[7]=1;
PWM_init(600);
while(1){
CanReceiveMessage(&msg);
delay_ms(100);
if(msg.eid == 0x18feee00)
    {
      CoolTemp = msg;
    }
else if(msg.eid == 0x0cf00400)
    {
      EngSpeed = msg;
    }
dtc=(float)CoolTemp.data[1];
   	PWM_generate(dtc,600.0,7);

//delay_ms(10);
//CanSendMessage(&tx);
delay_ms(100);

}
return 0;
}
