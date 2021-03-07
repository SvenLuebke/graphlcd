
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "iodrv.h"
#include "iodrv_libftdi.h"


const uint32_t    kBufferSize = 8192 * 2;
const uint16_t    kUSBpid_FT4232H = 0x6010;
const uint16_t    kUSBpid_FT4232Serial = 0x6011;
const uint16_t    kUSBpid_UM232R = 0x6001;
const uint16_t    kUSBpid = kUSBpid_FT4232Serial;


uint8_t           buffer0[kBufferSize] = { 0x2A };
uint32_t          buffer0FillIndex = 0;

void IODRV_SPIStartSequence(uint8_t pinStart);
void IODRV_SPIEndSequence(void);


IODRVrc_t IODRV_Init(uint8_t channelId, uint32_t frequency, IODRVhandle_t *io_handle)
{
  ftdi_status_t ftdi_status;
  ftdi_handle_t ftdi_handle;


  buffer0FillIndex = 0;
  if ((*io_handle = (IODRVhandle_t) ftdi_new()) == 0)
  {
    fprintf(stderr, "ftdi_new failed\n");
    return EXIT_FAILURE;
  }
  ftdi_handle = (ftdi_handle_t) *io_handle;
  ftdi_status = ftdi_usb_open(ftdi_handle, 0x0403, kUSBpid);
  if (ftdi_status < 0 && ftdi_status != -5)
  {
    fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ftdi_status, ftdi_get_error_string(ftdi_handle));
    ftdi_free(ftdi_handle);
    return EXIT_FAILURE;
  }
  ftdi_usb_reset(ftdi_handle);
  IODRV_ShowInfo(io_handle);
  ftdi_set_interface(ftdi_handle, (ftdi_interface) channelId);
  ftdi_set_bitmode(ftdi_handle, 0, 0); // reset
  ftdi_set_bitmode(ftdi_handle, 0, BITMODE_MPSSE); // enable mpsse on all bits
  usleep(50000); // sleep 50 ms for setup to complete
  mpsse_set_clock(&buffer0[buffer0FillIndex], frequency, &buffer0FillIndex);
  mpsse_set_protocolParams(&buffer0[buffer0FillIndex],
                           (FTDI_PIN_SCK | FTDI_PIN_MOSI | FTDI_PIN_CS | FTDI_PIN_CD | FTDI_PIN_UNUSED),
                           (FTDI_PIN_SCK | FTDI_PIN_MOSI | FTDI_PIN_CS | FTDI_PIN_CD), &buffer0FillIndex);
  if (ftdi_write_data(ftdi_handle, &buffer0[0], buffer0FillIndex) != (int32_t) buffer0FillIndex)
    return EXIT_FAILURE;

  buffer0FillIndex = 0;
  return EXIT_SUCCESS;
}

void IODRV_WriteCommand(uint8_t command)
{
  IODRV_SPIStartSequence(FTDI_PIN_CD);

  /* length LSB */
  buffer0[buffer0FillIndex++] = (uint8_t)((1 - 1) & 0x000000FF);
  /* length MSB */
  buffer0[buffer0FillIndex++] = (uint8_t)(((1 - 1) & 0x0000FF00) >> 8);
  buffer0[buffer0FillIndex++] = command;

  IODRV_SPIEndSequence();
}

void IODRV_WriteData(uint8_t data)
{
  IODRV_SPIStartSequence(FTDI_PIN_MOSI);

  /* length LSB */
  buffer0[buffer0FillIndex++] = (uint8_t)((1 - 1) & 0x000000FF);
  /* length MSB */
  buffer0[buffer0FillIndex++] = (uint8_t)(((1 - 1) & 0x0000FF00) >> 8);
  buffer0[buffer0FillIndex++] = data;

  IODRV_SPIEndSequence();
}

inline void IODRV_SPIStartSequence(uint8_t pinStart)
{
  buffer0[buffer0FillIndex++] = MPSSE_CMD_SET_DATA_BITS_LOWBYTE;
  /* if SPI data is transferred via MPSSE_CMD_DATA_OUT_BYTES_NEG_EDGE,
     FTDI_PIN_SCK needs to be added here, otherwise deleted */
  buffer0[buffer0FillIndex++] = 0xFF & ~(FTDI_PIN_SCK | pinStart | FTDI_PIN_CS | FTDI_PIN_UNUSED);
  buffer0[buffer0FillIndex++] = FTDI_GPIO_DIR_VAL;
  buffer0[buffer0FillIndex++] = MPSSE_SPI_CMD_TX;
}

inline void IODRV_SPIEndSequence(void)
{
  buffer0[buffer0FillIndex++] = MPSSE_CMD_SET_DATA_BITS_LOWBYTE;
  /* if SPI data is transferred via MPSSE_CMD_DATA_OUT_BYTES_POS_EDGE,
     FTDI_PIN_SCK needs to be added here, otherwise deleted */
  buffer0[buffer0FillIndex++] = FTDI_PIN_CD | FTDI_PIN_CS;
  buffer0[buffer0FillIndex++] = FTDI_GPIO_DIR_VAL;
}

void IODRV_WriteData(uint8_t data1, uint8_t data2)
{
  IODRV_SPIStartSequence(FTDI_PIN_MOSI);

  /* length LSB */
  buffer0[buffer0FillIndex++] = (uint8_t)((2 - 1) & 0x000000FF);
  /* length MSB */
  buffer0[buffer0FillIndex++] = (uint8_t)(((2 - 1) & 0x0000FF00) >> 8);
  buffer0[buffer0FillIndex++] = data1;
  buffer0[buffer0FillIndex++] = data2;

  IODRV_SPIEndSequence();
}

void IODRV_WriteDataBlock(int32_t sizeToTransfer)
{
  IODRV_SPIStartSequence(FTDI_PIN_MOSI);

  /* length LSB */
  buffer0[buffer0FillIndex++] = (uint8_t)((sizeToTransfer - 1) & 0x000000FF);
  /* length MSB */
  buffer0[buffer0FillIndex++] = (uint8_t)(((sizeToTransfer - 1) & 0x0000FF00) >> 8);

  buffer0FillIndex += sizeToTransfer;
  IODRV_SPIEndSequence();
}

IODRVrc_t IODRV_TriggerXfer(IODRVhandle_t io_handle)
{
  uint32_t retVal;
  uint32_t bytesWritten = 0;

  bytesWritten = ftdi_write_data((ftdi_handle_t)io_handle, buffer0, buffer0FillIndex);
  if (bytesWritten == buffer0FillIndex)
  {
    retVal = EXIT_SUCCESS;
  }
  else
  {
    fprintf(stdout, "Bytes written were not the same: %d\n", bytesWritten);
    retVal = EXIT_FAILURE;
  }
  buffer0FillIndex = 0;
  return retVal;
}

uint8_t *IODRV_GetDataBuffer(void)
{
  return &buffer0[buffer0FillIndex+MPSSE_DATA_OFFSET];
}

void IODRV_ShowInfo(IODRVhandle_t* handle)
{
  ftdi_handle_t ftdi = (ftdi_handle_t)*handle;
  struct ftdi_version_info version;
  unsigned int chipid;

  version = ftdi_get_library_version();
  fprintf(stdout, "Initialized libftdi %s (major: %d, minor: %d, micro: %d, snapshot ver: %s)\n",
                  version.version_str, version.major, version.minor, version.micro,
                  version.snapshot_str);
  fprintf(stdout, "FTDI type: (%d)\n", ftdi->type);

  fprintf(stdout, "ftdi_read_chipid: %d\n", ftdi_read_chipid(ftdi, &chipid));
  fprintf(stdout, "FTDI chipid: %X\n", chipid);
}

void IODRV_DeInit(IODRVhandle_t* io_handle)
{
  ftdi_handle_t ftdi_handle;
  ftdi_handle = (ftdi_handle_t)*io_handle;
  ftdi_free(ftdi_handle);
}

IODRVrc_t mpsse_set_pins(IODRVhandle_t io_handle, uint8_t levels)
{
  uint8_t buf[MPSSE_CMD_SIZE] = { 0 };
  buf[0] = SET_BITS_LOW;
  buf[1] = levels;
  buf[2] = PINS_DIR;
  return ftdi_write_data((ftdi_handle_t) io_handle, buf, sizeof(buf)) != sizeof(buf);
}

IODRVrc_t mpsse_set_clock(ftdi_mpssebuf_t buffer, uint32_t freq, ftdi_bytecounter_t *bytesWritten)
{
  uint32_t bufIdx = 0;
  uint32_t system_clock = 0;
  uint16_t divisor = 0;


  if (freq > 6000000) {
    buffer[bufIdx++] = TCK_X5;
    system_clock = 60000000;
  }
  else {
    buffer[bufIdx++] = TCK_D5;
    system_clock = 12000000;
  }
  divisor = (((system_clock / freq) / 2) - 1);
  buffer[bufIdx++] = TCK_DIVISOR;
  buffer[bufIdx++] = (divisor & 0xFF);
  buffer[bufIdx++] = ((divisor >> 8) & 0xFF);
  *bytesWritten += bufIdx;
  return EXIT_SUCCESS;
}

void mpsse_set_protocolParams(ftdi_mpssebuf_t buffer, uint8_t pinInitialState, uint8_t pinDirection, ftdi_bytecounter_t* bytesWritten)
{
  uint32_t bufIdx = 0;

  buffer[bufIdx++] = DIS_ADAPTIVE;    // opcode: disable adaptive clocking
  buffer[bufIdx++] = DIS_3_PHASE;     // opcode: disable 3-phase clocking
  buffer[bufIdx++] = SET_BITS_LOW;    // opcode: set low bits (ADBUS[0-7])
  buffer[bufIdx++] = pinInitialState; // argument: inital pin states
  buffer[bufIdx++] = pinDirection;    // argument: pin direction
  *bytesWritten += bufIdx;
}

