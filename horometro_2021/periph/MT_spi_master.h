/*
 * spi_master.h
 *
 *  Created on: Apr 13, 2018
 *      Author: mtorres
 */

#ifndef MT_SPI_MASTER_H_
#define MT_SPI_MASTER_H_

/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include "project_defines.h"


/* Constantes y macros */

// Definición de pines

#define DDR_MOSI0		DDRB
#define PORT_MOSI0		PORTB
#define MOSI0			3
#define DDR_MISO0		DDRB
#define MISO0			4
#define DDR_SCK0		DDRB
#define PORT_SCK0		PORTB
#define SCK0			5
#define DDR_SS0			DDRB
#define PORT_SS0		PORTB
#define SS0				2

#define DDR_MOSI1		DDRE
#define PORT_MOSI1		PORTE
#define MOSI1			3
#define DDR_MISO1		DDRC
#define PORT_MISO1		PORTC
#define MISO1			0
#define DDR_SCK1		DDRC
#define PORT_SCK1		PORTC
#define SCK1			1
#define DDR_SS1			DDRE
#define PORT_SS1		PORTE
#define SS1				2


// Tamaño de los buffers de transmmisión y recepción
/*
#define SPI_TX_BUFF_SIZE	2
#define SPI_RX_BUFF_SIZE	2
*/

/* Definición de tipos y estructuras */

typedef enum{
	SPI_Slave = 0,
	SPI_Master = 16
}spi_mode_t;

typedef enum{
	CPOL_0 = 0,
	CPOL_1 = 8
}spi_clock_polarity_t;

typedef enum{
	CPHA_0 = 0,
	CPHA_1 = 4
}spi_clock_phase_t;

typedef enum{
	MSB_First = 0,
	LSB_First = 32
}spi_data_order_t;

typedef enum{
	SPI_Int_Enabled = 128,
	SPI_Int_Disabled = 0
}spi_interrupt_t;


typedef enum{
	SCK_Fosc_2 = 4,
	SCK_Fosc_4 = 0,
	SCK_Fosc_8 = 5,
	SCK_Fosc_16 = 1,
	SCK_Fosc_32 = 6,
	SCK_Fosc_64 = 2,
	SCK_Fosc_128 = 3
}spi_clock_source_t;


typedef struct{
	spi_mode_t mode;
	spi_clock_source_t sck_prescaler;
	spi_clock_polarity_t cpol;
	spi_clock_phase_t cpha;
	spi_data_order_t data_order;
	spi_interrupt_t interrupt;
}spi_config_t;


/* Declaración de variables globales */
//extern volatile uint8_t spi_tx_buffer[];
//extern volatile uint8_t spi_rx_buffer[];

/* Declaración de funciones (prototipos) */
void SPI0_Configurar(spi_config_t *spi_param);
void SPI1_Configurar(spi_config_t *spi_param);
uint8_t SPI0_Master_Tx_Byte(uint8_t byte);
uint8_t SPI1_Master_Tx_Byte(uint8_t byte);
void SPI0_Master_Tx_Bitstream(uint8_t n_bytes, uint8_t *output_buffer, uint8_t *input_buffer);
void SPI1_Master_Tx_Bitstream(uint8_t n_bytes, uint8_t *output_buffer, uint8_t *input_buffer);

#endif /* SPI_MASTER_H_ */
