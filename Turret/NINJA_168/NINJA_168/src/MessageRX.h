#ifndef _MESSAGE_RX__H_
#define _MESSAGE_RX__H_

#define	CONTROL_MESSAGE 'C'
#define	DATA_MESSAGE    'D'
#define	QUERY           'Q'

#define	MESSAGE_TYPE	0
#define	MESSAGE_LENGTH	1
#define HEADER_LENGTH	2
#define NO_MESSAGE_TO_PARSE FALSE
#define MESSAGE_TO_PARSE TRUE

//Biggest Payload Allowed = 128 Bytes
extern unsigned char receiveOneCharMessage();
extern void parseMessageTemp(void);

#endif
