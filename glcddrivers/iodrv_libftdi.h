#ifndef IODRV_LIBFTDI_H
#define IODRV_LIBFTDI_H

#include <stdio.h>

#include <ftdi.h>
#include "iodrv.h"

typedef struct ftdi_context* ftdi_handle_t;
typedef int32_t ftdi_status_t;
typedef uint32_t ftdi_bytecounter_t;
typedef uint8_t* ftdi_mpssebuf_t;

#define MPSSE_DATA_OFFSET     6
/*MPSSE Control Commands*/
#define MPSSE_CMD_SET_DATA_BITS_LOWBYTE   0x80

/* Size of a MPSSE command packet */
#define MPSSE_CMD_SIZE 3
enum mpsse_commands {
  ENABLE_ADAPTIVE_CLOCK = 0x96,
  DISABLE_ADAPTIVE_CLOCK = 0x97,
  TCK_X5 = 0x8A,
  TCK_D5 = 0x8B,
  TRISTATE_IO = 0x9E,
};

enum mpsse_pins {
  SCLK = 1,
  MOSI = 2,
  MISO = 4,
  CS_L = 8,
};
/* SCLK/MOSI/CS_L are outputs, MISO is an input */
#define PINS_DIR (SCLK | MOSI | CS_L)


/* SPI mode 2:
 * propagates data on the rising edge
 * and reads data on the rising edge of the clock.
 */
#define MPSSE_SPI_CMD_TX            (MPSSE_DO_WRITE | MPSSE_WRITE_NEG)
#define SPI_CMD_RX                  (MPSSE_DO_READ)
#define SPI_CMD_TXRX                (MPSSE_DO_WRITE | MPSSE_DO_READ | MPSSE_WRITE_NEG)

IODRVrc_t mpsse_set_pins(IODRVhandle_t io_handle, uint8_t levels);
IODRVrc_t mpsse_set_clock(ftdi_mpssebuf_t buffer, uint32_t freq, ftdi_bytecounter_t* bytesWritten);
void mpsse_set_protocolParams(ftdi_mpssebuf_t buffer, uint8_t pinInitialState, uint8_t pinDirection, ftdi_bytecounter_t* bytesWritten);

#endif
