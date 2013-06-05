/**
 * @file SCPI_msgEPCtrl.h
 *
 * This file provides functions and definitions that
 * create the SCPI message exchange controller
 *
 * The message exchange control represents the
 * interconnection of control messages between the output
 * queue, response formatter input buffer, parser,
 * execution control, I/O control and device function
 * blocks.
 *
 * This file should not need to be edited by the user
 */

#ifndef SCPI_MSGXCHGCTRL_H_
#define SCPI_MSGXCHGCTRL_H_

#ifdef __cplusplus
	extern "C" {
#endif

struct msg {
	bool brq;		/* Byte was sent to IO Control */
	bool bav;		/* Byte arrived at IO control */
	bool get;		/* Group execute trigger (&GET) arrived at IO control */
	bool dcas;		/* Device clear (break / &DCL) arrived on the IO control */
	bool rmtSent;	/* A <RESPONSE MESSAGE TERMINATOR> was sent */
	bool ibEmpty;	/* Input buffer is empty */
	bool ibFull;	/* Input buffer is full - more bytes generate input overflow and cannot generate another bav */
	bool eom;		/* Parser identified an LF as a <PROGRAM MESSAGE TERMINATOR> */
	bool query;		/* Parser identified something which generates a response */
	bool pIdle;
	bool pBlocked;	/* Parser is waiting for the  execution control */
	bool ecIdle;
	bool ecBlocked;	/* Execution control is waiting for the device functions */
	bool pon;		/* Device was powered on */
	bool rfBlocked;	/* Response formatter is waiting for room on the output queue (set by the response formatter) */
	bool oqFull;	/* Output queue is full, blocking the response formatter (set by the output queue) */
	bool oqEmpty;	/* Output queue is empty */
//	bool ibBlocked;	/* Parser tried to get a byte from the input buffer, but it was empty */
	bool reqt;
	bool reqf;
	bool ibEom;
};
typedef struct msg msg;

extern msg msgs;

extern Uint16 scpiInit (Uint16 (*IOTx)(char data));
extern void scpi (void);


/*
 * reqt, reqf, p-idle, ec-idle, oq-empty, p-blocked, ec-idle, oq-empty messages?
 *
 * MESSAGE EXCHANGE STATES
 *  - READ			In the READ state the I/O Control is reading data bytes and GET messages from the interface and placing
 *					 them in the Input Buffer. The Parser and Execution Control are active and the Output Queue is empty. A brq
 *					 cannot occur while in the READ state.
 *					 The MEC remains in the READ state when either dcas or pon occurs. The device performs the INITIALIZE
 *					 action described in 6.3.5.2.
 *					 The MEC remains in the READ state when bav, get, eom, or ib-blocked occurs.
 *					 The MEC enters the QUERY state when query occurs.
 *
 *  - QUERY			In the QUERY state the Parser has recognized a valid query within a <PROGRAM MESSAGE>. The I/O
 *					 Control Emulator continues reading data bytes and GET messages from the interface and placing them in the
 *					 Input Buffer.
 *					 The MEC remains in the QUERY state when bav, get, query, or ib-blocked occurs.
 *					 The MEC enters the SEND state when brq occurs.
 *					 The MEC enters the RESPONSE state when eom occurs and ib-empty is TRUE.
 *					 The MEC enters the READ state when an eom occurs and ib-empty is FALSE. This condition indicates that
 *					 the controller started sending a new message before reading all of the response. The device performs the
 *					 INTERRUPTED action described in 6.3.5.3.
 *					 The MEC enters the DEADLOCK state when ib-full, oq-full, p-blocked, ec-blocked, and rf-blocked are all
 *					 TRUE. This condition indicates that the controller is sending more data to the device and expecting more
 *					 data from the device than the device can buffer. The device performs the DEADLOCK action described in
 *					 6.3.5.5.
 *					 The MEC enters the READ state when either dcas or pon occurs. The device performs the INITIALIZE
 *					 action described in 6.3.5.2.
 *
 *  - SEND			In the SEND state the device starts to send the <RESPONSE MESSAGE> to the controller. The device
 *					 continues to receive data bytes from the interface, and to parse and execute the Query Message stored in the
 *					 Input Buffer. The Response Formatter prepares the <RESPONSE MESSAGE>, and places it in the Output
 *					 Queue.
 *					 A deadlock situation is possible in the SEND state. The controller or communication channel may buffer
 *					 part of the response even through the application has not requested any data. This buffering is actually an
 *					 extension of the Output Queue, but the device has no control over it. If this extra buffer should fill, so the
 *					 device is prevented from sending more data while the controller is still sending the <PROGRAM
 *					 MESSAGE>, the Input Buffer may also fill, thus deadlocking the message exchange.
 *					 The device could possibly detect this situation, but it cannot clear the extra buffers, so it merely stays in the
 *					 SEND state until the controller clears the deadlock.
 *					 The MEC remains in the SEND state when bav, get, brq, query or ib-blocked occurs.
 *					 The MEC remains in the SEND state when ib-empty becomes TRUE. This transition might be considered
 *					 UNTERMINATED since it appears that the controller failed to send an eom before it started to read the
 *					 response. The nature of a full-duplex serial interface, however, allows bytes to be transferred and buffered in
 *					 both directions. The eom may still be on its way from the controller so the device waits for it in SEND.
 *					 The MEC enters the RESPONSE state when eom occurs and ib-empty is TRUE.
 *					 The MEC enters the READ state when eom occurs and ib-empty is FALSE. This condition indicates that the
 *					 controller started sending a new message before reading all of the response. The device performs the
 *					 INTERRUPTED action described in 6.3.5.3.
 *					 The MEC enters the READ state when either dcas or pon occurs. The device performs the INITIALIZE
 *					 action described in 6.3.5.2.
 *
 *  - RESPONSE		In the RESPONSE state the Parser has received a <PROGRAM MESSAGE TERMINATOR> and the device
 *					 is sending the complete <RESPONSE MESSAGE> to the controller. The Response Formatter formats the
 *				 	 <RESPONSE MESSAGE> and the I/O Control sends data bytes from the Output Queue to the controller.
 *					 An eom, query, or ib-blocked cannot occur because the Input Buffer is empty.
 *					 The MEC remains in the RESPONSE state when a brq occurs.
 *					 The MEC enters the READ state when RMT-sent occurs.
 *					 The MEC enters the READ state when bav or get occurs. This condition indicates that the controller started
 *					 sending a new message before reading all of the response. The device performs the INTERRUPTED action
 *					 described in 6.3.5.3 of this standard.
 *					 The MEC enters the READ state when either dcas or pon occurs. The device performs the INITIALIZE
 *					 action described in 6.3.5.2.
 *					 A device may have nothing to say while the MEC is in the RESPONSE state if all of the queries in the Query
 *					 Message failed to generate any <RESPONSE DATA> because of syntax (Command), semantic (Execution),
 *					 or Device-specific errors. The device does not send a <RESPONSE MESSAGE TERMINATOR> to the
 *					 controller. The MEC remains in the RESPONSE state until either bav, get, pon, or dcas occurs.
 *
 *  - DEADLOCK		In the DEADLOCK state the device has been asked to buffer more data than it has room to store. The Output
 *					 Queue was full, blocking the Response Formatter, Execution Control, and Parser. The Input Buffer was full,
 *					 and the controller was waiting to send more data bytes to the device.
 *					 The MEC breaks the deadlock by clearing the Output Queue and resetting the Response Formatter. The
 *					 MEC also reports a Query Error to the Status Reporting. While in the DEADLOCK state, the MEC continues
 *					 to parse and execute <PROGRAM MESSAGE UNIT> elements as usual, except that it discards query
 *					 responses rather than placing them into the Output Queue.
 *					 Because the Output queue is kept empty in DEADLOCK, brq can never occur.
 *					 The MEC remains in the DEADLOCK state when a bav, get, query or, ib-blocked occurs.
 *					 The MEC enters the READ state when eom occurs.
 *					 The MEC enters the READ state when either dcas or pon occurs. The device performs the INITIALIZE
 *					 action, described in 6.3.5.2.
 *
 * MESSAGE EXCHANGE PROTOCOL ACTIONS
 *  - Normal
 *  - INITIALIZE
 *  - INTERRUPTED
 *  - UNTERMINATED
 *  - DEADLOCK
 *  - Query after indefinite response
 *
 */

#ifdef __cplusplus
	}
#endif /* extern "C" */

#endif /* SCPI_MSGXCHGCTRL_H_ */
