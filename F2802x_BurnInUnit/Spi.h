/**
 * @file Spi.h
 *
 * @brief Serial peripheral interface communications functions.
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

#define LOSPCLK_FREQ_SET 	0xE4EC10/*< The frequency of the low speed clock has been set to (in DevInit_F2802x.h). */

#define SPI_DFLT_BAUD		0x3D090	/**< SPI default baud rate. */
#define SPI_DFLT_CPOL 		0x00	/**< SPI default clock polarity. 0 = Rising edge, 1 = falling edge. */
#define SPI_DFLT_CPHA 		0x01	/**< SPI default clock phase. 0 = Without delay, 1 = with delay. */

#define SPI_FFRX_INTLVL 	0x01	/**< SPI Rx FIFO interrupt level. */
#define SPI_FFTX_INTLVL 	0x00	/**< SPI Tx FIFO interrupt level. */
#define SPI_FFTX_FILLVL 	0x04	/**< SPI Tx FIFO fill level. */

/** The possible SPI Modes. */
enum spiMode {
	spiSlave = 0,
	spiMaster = 1
};

/** A type to allow specification of the SPI mode. */
typedef enum spiMode spiMode;

/** The possible SPI loop-back settings. */
enum spiLpbk {
	disabled = 0,
	enabled = 1
};

/** A type to allows specification of the SPI loop-back setting. */
typedef enum spiLpbk spiLpbk;

/** The possible edge transition polarity settings. */
enum transPol {
	risingEdge = 0,
	fallingEdge = 1,
	risingAndFallingEdge = 3
};

/** A type to allow specification of the edge transition polarity setting. */
typedef enum transPol transPol;

/** The possible SPI clock phase settings. */
enum spiCPha {
	withDelay = 0,
	withoutDelay = 1
};

/** A type to allow specification of the SPI clock phase setting. */
typedef enum spiCPha spiCPha;

/** Initialises the SPI-A peripheral and relevant interrupts
 * @param[in]	 mode		Specifies if the device is in slave or master mode, values are master or slave.
 * @param[in]	 baud		Specifies the baud rate to be used minimum value is 117,187, maximum is 3,750,000
 * @param[in]	 loopback	Specifies if the SPI module should be run in loop-back mode, values are enable or disable.
 * @param[in]	 cPol		Specifies the clock polarity, values are risingEdge or fallingEdge.
 * @param[in]	 cPha		Specifies the clock phase, values are withDelay or withoutDelay.
 * @return					Error status.
 *
 * @warning This function will clear any values already in the SPI peripheral registers.
 * @warning This function MUST be called before any other public SPI function.
 */
extern Uint16 spiInit(spiMode mode, Uint32 baud, spiLpbk loopback, transPol cPol, spiCPha cPha);

/** Transmits whatever data is on the SCPI output queue if SPI has been selected as the external communications type. */
extern void spiTx (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /*SPI_H_*/

//===========================================================================
// End of file.
//===========================================================================
