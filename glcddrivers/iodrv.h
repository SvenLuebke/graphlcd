#ifndef IODRV_H
#define IODRV_H

#include <stdint.h>
#include <stdio.h>


typedef void*         IODRVhandle_t;
typedef int32_t       IODRVrc_t;

typedef enum
{
  IODRV_XFERTYPE_RESERVED,
  IODRV_XFERTYPE_COMMAND,
  IODRV_XFERTYPE_DATA
} IODRVxfertype_t;

#define IODRV_COMMAND             0x00u     /* low for commands */
#define IODRV_DATA                0x10u     /* high for data */


#define FTDI_GPIO_DIR_VAL         0x1Bu
#define FTDI_GPIO_MASK            0xF0u
#define FTDI_GPIO_RESET_MASK      0xFBu         /* all out except MISO */


#define FTDI_PIN_SCK        (0x01u << 0)
#define FTDI_PIN_MOSI       (0x01u << 1)
#define FTDI_PIN_MISO       (0x01u << 2)
#define FTDI_PIN_CS         (0x01u << 3)
#define FTDI_PIN_SPI        (FTDI_PIN_SCK | FTDI_PIN_MOSI | FTDI_PIN_MISO | FTDI_PIN_CS)
#define FTDI_PIN_CD         (0x01u << 4)
#define FTDI_PIN_UNUSED1    (0x01u << 5)
#define FTDI_PIN_UNUSED2    (0x01u << 6)
#define FTDI_PIN_UNUSED3    (0x01u << 7)
#define FTDI_PIN_UNUSED     (FTDI_PIN_UNUSED1 | FTDI_PIN_UNUSED2 | FTDI_PIN_UNUSED3)


#define IODRV_WriteByte(a, b, c)       {uint8_t *buffer = IODRV_GetDataBuffer(); buffer[0] = c; IODRV_Write(a, b, 1);}

IODRVrc_t   IODRV_Init(uint8_t channelId, uint32_t frequency, IODRVhandle_t *io_handle);
uint8_t*    IODRV_GetDataBuffer(void);
void        IODRV_WriteCommand(uint8_t command);
void        IODRV_WriteData(uint8_t data);
void        IODRV_WriteData(uint8_t data1, uint8_t data2);

void        IODRV_WriteDataBlock(int32_t sizeToTransfer);
IODRVrc_t   IODRV_TriggerXfer(IODRVhandle_t io_handle);
void        IODRV_DeInit(IODRVhandle_t* io_handle);

void        IODRV_ShowInfo(IODRVhandle_t* handle);
#endif
