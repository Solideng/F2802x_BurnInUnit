/**
 * @file Spi.h
 *
 * @brief SPI communications functions.
 *
 * @warning
 * The following bridge tracks should be cut on the TI
 * C2000 LaunchPad XL PCB before SPI (or I2C) may be used:
 * |Bridge to Cut	| GPIO 	|PCB Pin|
 * |:--------------:|:-----:|:-----:|
 * | JP5			| GPIO32| J2-6	|
 * | JP7			| GPIO33| J2-7	|
 * | JP8			| GPIO16| J6-7	|
 * | JP10			| GPIO17| J6-8	|
 *
 * This should result in the following functionality:
 * |Function	| GPIO 	|PCB Pin|
 * |:----------:|:-----:|:-----:|
 * | I2C_SDAA	| GPIO32| J6-7	|
 * | I2C_SCLA	| GPIO33| J6-8	|
 * | SPI_MOSI	| GPIO16| J2-6	|
 * | SPI_MISO	| GPIO17| J2-7	|
 * | SPI_CLK	| GPIO18| J1-18	|
 * | SPI_STE 	| GPIO19| J2-1 	|
 *
 */
#ifndef SPI_H_
#define SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

// Public SPI functions
extern void spiInit(void);			// Configures SPI peripheral for communications
									//  Sends 11 left aligned bits at a time
									// 	spiInit() MUST BE CALLED BEFORE ANY OTHER PUBLIC SPI FUNCTIONS
									// 	spiInit() CLEARS ANY VALUES CURRENTLY IN SPI REGISTERS

extern void spiTx(Uint16 txData);	// Transmits 11 left aligned bit txData on SPI

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /*SPI_H_*/

//===========================================================================
// End of file.
//===========================================================================
