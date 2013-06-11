/**
 * @file SCPI_respFmtr.h
 *
 * This file provides functions and definitions that
 * form the SCPI response formatter.
 *
 * The response formatter builds a @f$ \langle@f$ RESPONSE
 * MESSAGE @f$ \rangle@f$ out of response message elements
 * from valid query requests and response data. The
 * @f$ \langle@f$ RESPONSE MESSAGE @f$ \rangle@f$ is placed
 * in to the output queue. The response formatter's primary
 * responsibility is to convert the internal representation
 * of data elements into a sequence of data bytes according
 * to the IEEE 488.2 Device Talking Elements syntax rules.
 *
 * This file should not need to be edited by the user.
 */

#ifndef SCPI_RESPFMTR_H_
#define SCPI_RESPFMTR_H_

#define MAX_NUM_RDATA			4	/**< The maximum number of <RESPONSE DATA> elements per response message. */
#define MAX_RDATA_STRING_SIZE	10	/**< The maximum length of <RESPONSE DATA> element of string type including the string terminator. */

enum rspnsType {
	typeNotSet = 0,
	Integer,
	Double,
	String
};
typedef enum rspnsType rtype;		/**< A type used to indicate the type of <RESPONSE DATA>. */

extern void resetRespFmtr (void);

extern Uint16 respond (void *data, rtype dataType, bool isDataEnd);

extern Uint16 respFmtr (void);

#endif /* SCPI_RESPFMTR_H_ */
