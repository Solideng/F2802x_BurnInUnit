/**
 * @file Sci.h
 *
 * @brief Serial communications interface functions.
 *
 * @par
 * On the TI C2000 LaunchPad XL:
 * |Function| GPIO 	|PCB Pin|
 * |:------:|:-----:|:-----:|
 * | SCI_RX	| GPIO28| J1-3	|
 * | SCI_TX	| GPIO29| J1-4	|
 *
 * @warning
 * On the TI C2000 LaunchPad XL the switch 4 (S4) should be
 * in the OFF position to communicate with devices other
 * than the on-board USB controller.
 *
 */

#ifndef SCI_H_
#define SCI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*================== MACROS ===================*/
#define SCIBAUD_MIN			29	/**< Minimum allowable value of SCI Baud. */
#define SCIFFRX_INT_LVL		1	/**< Interrupt level for receiving FIFO. */
#define SCIFFTX_INT_LVL		0	/**< Interrupt level for transmission FIFO. */
#define SCIFFTX_FILL_LVL 	4	/**< Fill level for transmission FIFO. */

/*============= GLOBAL FUNCTIONS ==============*/
/** Initialises the SCI(A) peripheral and relevant interrupts.
 * @param[in]	baud	The baud rate that the SCI should use minimum value is set by SCIBAUD_MIN.
 * @return				Error status.
 *
 * @warning This function will clear any values already in the SCI peripheral registers.
 * @warning This function MUST be called before any other SCI function.
 */
extern Uint16 sciInit(Uint32 baud);

/** Transmits whatever data is on the SCPI output queue if SCI has been selected as the external communications type. */
extern void sciTx (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SCI_H_*/
