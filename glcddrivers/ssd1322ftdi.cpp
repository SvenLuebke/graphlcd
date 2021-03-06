/*
 * GraphLCD driver library
 *
 * ssd1322ftdi.c  -  SSD1322 OLED driver class (via FTDI adapter)
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * (c) 2020 Sven Lübke <sven.luebke AT mikrosol.de>
 */

#include <stdint.h>
//#include <syslog.h>
#include <unistd.h>
#include <cstring>

#include <ftdi.h>

//#include "common.h"
#include "config.h"

#include "ssd1322ftdi.h"
#include "iodrv.h"

namespace GLCD
{

const int kLcdWidth  = 256;
const int kLcdHeight = 64;

// Display commands
const uint8_t kSSD1322CmdSetColumnAddress =                 0x15;
const uint8_t kSSD1322CmdSetRowAddress =                    0x75;
const uint8_t kSSD1322CmdWriteData =                        0x5C;
const uint8_t kSSD1322CmdReadData =                         0x5D;

const uint8_t kSSD1322CmdEnableGreyScale =                  0x00;
const uint8_t kSSD1322CmdSetMode =                          0xA0;
const uint8_t kSSD1322CmdSetDisplayStartLine =              0xA1;
const uint8_t kSSD1322CmdSetDisplayOffset =                 0xA2;
const uint8_t kSSD1322CmdEntireDisplayOff =                 0xA4;
const uint8_t kSSD1322CmdEntireDisplayOn =                  0xA5;
const uint8_t kSSD1322CmdSetNormalDisplay =                 0xA6;
const uint8_t kSSD1322CmdSetInverseDisplay =                0xA7;
const uint8_t kSSD1322CmdSelectVDD =                        0xAB;
const uint8_t kSSD1322CmdSleepModeOn =                      0xAE;
const uint8_t kSSD1322CmdSleepModeOff =                     0xAF;
const uint8_t kSSD1322CmdSetPhaseLength =                   0xB1;
const uint8_t kSSD1322CmdSetClockFrequency =                0xB3;
const uint8_t kSSD1322CmdDisplayEnhancementA =              0xB4;
const uint8_t kSSD1322CmdSetGPIO =                          0xB5;
const uint8_t kSSD1322CmdSetSecondPrechargePeriod =         0xB6;
const uint8_t kSSD1322CmdSetGrayScaleTable =                0xB8;
const uint8_t kSSD1322CmdSelectDefaultLinearGrayScaleable = 0xB9;
const uint8_t kSSD1322CmdSetPrechargeVoltage =              0xBB;
const uint8_t kSSD1322CmdSetVcomH =                         0xBE;
const uint8_t kSSD1322CmdSetContrastCurrent =               0xC1;
const uint8_t kSSD1322CmdMasterContrastCurrentControl =     0xC7;
const uint8_t kSSD1322CmdSetMUXRatio =                      0xCA;
const uint8_t kSSD1322CmdDisplayEnhancementB =              0xD1;
const uint8_t kSSD1322CmdSetCommandLock =                   0xFD;


cDriverSSD1322::cDriverSSD1322(cDriverConfig * config)
:   cDriver(config)
{
  refreshCounter = 0;
  if (EXIT_SUCCESS == IODRV_Init(0, &io_handle))
  {}
}

cDriverSSD1322::~cDriverSSD1322()
{
  IODRV_DeInit(&io_handle);
}

int cDriverSSD1322::Init()
{
  width = config->width;
  if (width <= 0)
    width = kLcdWidth;
  height = config->height;
  if (height <= 0)
    height = kLcdHeight;
  fprintf(stdout, "Configured with %d x %d\n", width, height);

  this->gfxMemBytesOneRow = width / 2;
  this->bufferSize = this->gfxMemBytesOneRow * height;

  for (unsigned int i = 0; i < config->options.size(); i++)
  {
    if (config->options[i].name == "")
    {
    }
  }

  // setup lcd array (wanted state)
  this->newLCD = new unsigned char[bufferSize];
  if (this->newLCD)
  {
    memset(&(this->newLCD[0]), 0, bufferSize);
  }

  // setup lcd array (current state)
  this->oldLCD = new unsigned char[bufferSize];
  if (this->oldLCD)
  {
    memset(&this->oldLCD[0], 0, bufferSize);
  }

  if (config->device == "")
  {
      return -1;
  }

  WriteCommand(kSSD1322CmdSetCommandLock, 0x12);
  WriteCommand(kSSD1322CmdSleepModeOn);
  WriteCommand(kSSD1322CmdSetClockFrequency, 0x91);
  WriteCommand(kSSD1322CmdSetMUXRatio, 0x3F);                  /* multiplex ratio */ /* duty = 1/64 */
  WriteCommand(kSSD1322CmdSetDisplayOffset, 0x00);             /* set offset */
  WriteCommand(kSSD1322CmdSetDisplayStartLine, 0x00);          /* start line */
  WriteCommand(kSSD1322CmdSetMode, 0x14, 0x11);                /* set remap */
  IODRV_TriggerXfer(io_handle);

  WriteCommand(kSSD1322CmdSelectVDD, 0x01);                    /* Enable internal VDD  regulator */
  WriteCommand(kSSD1322CmdDisplayEnhancementA, 0xA0, 0xFD);    /* Display Enhancement A */
  WriteCommand(kSSD1322CmdSetContrastCurrent, 0x9F);           /* Set Contrast Current */
  WriteCommand(kSSD1322CmdMasterContrastCurrentControl, 0x0F); /* Master Contrast Current Control */
  WriteCommand(kSSD1322CmdSelectDefaultLinearGrayScaleable);   /* Select Default Linear Gray Scale table */
  //WriteCommand(kSSD1322CmdEnableGreyScale);                  /* Enable Gray Scale table */
  WriteCommand(kSSD1322CmdSetPhaseLength, 0xE2);               /* Set Phase Length */
  WriteCommand(kSSD1322CmdDisplayEnhancementB, 0x82, 0x20);    /* Display Enhancement  B */ /* User is recommended to set A[5:4] to 00b */ /* Default */
  WriteCommand(kSSD1322CmdSetPrechargeVoltage, 0x1F);          /* Set Pre-charge voltage */
  WriteCommand(kSSD1322CmdSetSecondPrechargePeriod, 0x08);     /* Set Second Precharge Period */
  WriteCommand(kSSD1322CmdSetVcomH, 0x07);                     /* Set VCOMH */
  WriteCommand(kSSD1322CmdSetNormalDisplay);                   /* Normal Display */
  // clear display
  Clear();
  WriteCommand(kSSD1322CmdSleepModeOff);                       /* Sleep mode OFF */
  IODRV_TriggerXfer(io_handle);


  *oldConfig = *config;
  //syslog(LOG_INFO, "%s: SSD1322 initialized.\n", config->name.c_str());
  return 0;
}

int cDriverSSD1322::DeInit()
{
  // free lcd array (wanted state)
  if (this->newLCD)
  {
    delete this->newLCD;
  }
  // free lcd array (current state)
  if (this->oldLCD)
  {
    delete this->oldLCD;
  }

  return 0;
}

int cDriverSSD1322::CheckSetup()
{
  if (config->device != oldConfig->device ||
      config->width != oldConfig->width ||
      config->height != oldConfig->height)
  {
    DeInit();
    Init();
    return 0;
  }

  if (config->upsideDown != oldConfig->upsideDown ||
      config->invert != oldConfig->invert)
  {
    oldConfig->upsideDown = config->upsideDown;
    oldConfig->invert = config->invert;
    return 1;
  }
  return 0;
}

void cDriverSSD1322::Clear()
{
  memset(&(this->newLCD[0]), 0, bufferSize);
}


void cDriverSSD1322::SetPixel(int x, int y, uint32_t data)
{
  if (x >= width || y >= height)
    return;

  if (config->upsideDown)
  {
    x = width - 1 - x;
    y = height - 1 - y;
  }

  int y_offset = (y * this->gfxMemBytesOneRow);
  uint32_t ssd_data = x % 2 ? 0x0F : 0xF0;
  if (data == GRAPHLCD_White)
    newLCD[y_offset + x/2] |= ssd_data;
  else
    newLCD[y_offset + x / 2] &= ssd_data;
}


#if 0
void cDriverSSD1322::Set8Pixels(int x, int y, unsigned char data)
{
    if (x >= width || y >= height)
        return;

    if (!config->upsideDown)
    {
        int offset = 7 - (y % 8);
        for (int i = 0; i < 8; i++)
        {
            newLCD[x + i][y / 8] |= ((data >> (7 - i)) << offset) & (1 << offset);
        }
    }
    else
    {
        x = width - 1 - x;
        y = height - 1 - y;
        int offset = 7 - (y % 8);
        for (int i = 0; i < 8; i++)
        {
            newLCD[x - i][y / 8] |= ((data >> (7 - i)) << offset) & (1 << offset);
        }
    }
}
#endif

void cDriverSSD1322::Refresh(bool refreshAll)
{
  uint8_t* buffer;
  uint32_t x;
  int32_t y;


  refreshAll = true;
  if (refreshAll)
  {
    WriteCommand(kSSD1322CmdSetColumnAddress, 0x1C, 0x5B);
    WriteCommand(kSSD1322CmdSetRowAddress, 0, this->height);
    WriteCommand(kSSD1322CmdWriteData);
    buffer = IODRV_GetDataBuffer();
    int tmpIdx;
    for (y = 0; y < this->height; y++)
    {
      for (x = 0; x < this->gfxMemBytesOneRow; x++)
      {
        tmpIdx = (y * this->gfxMemBytesOneRow) + x;
        buffer[tmpIdx] = newLCD[tmpIdx];
      }
    }
    IODRV_WriteDataBlock(this->bufferSize);
    IODRV_TriggerXfer(io_handle);
    //memcpy(&(this->oldLCD[0]), &(this->newLCD[0]), bufferSize);
    // and reset RefreshCounter
    //refreshCounter = 0;
  }
  else
  {
      // draw only the changed bytes
  }
}

void cDriverSSD1322::SetBrightness(unsigned int percent)
{
  //WriteCommand(kCmdSetContrast, percent * 255 / 100);
}

void cDriverSSD1322::Reset()
{

}

void cDriverSSD1322::DrawHLine(uint8_t color, uint16_t x1, uint16_t y1, uint16_t x2)
{
  uint16_t index;

  index = (x1 / 2) + (y1 * this->gfxMemBytesOneRow);
  for (int32_t i = 0; i < (x2 - x1) / 2; i++)
  {
    this->newLCD[index + i] = color;
  }
}

void cDriverSSD1322::DrawHLineDirect(uint8_t color, uint16_t x1, uint16_t y1, uint16_t x2)
{
  uint8_t *buffer;
  int32_t i;

  WriteCommand(kSSD1322CmdSetColumnAddress, 0x1C + x1/4, 0x5B);
  WriteCommand(kSSD1322CmdSetRowAddress, (uint8_t) y1, y1 + 1);
  WriteCommand(kSSD1322CmdWriteData);
  buffer = IODRV_GetDataBuffer();
  for (i = 0; i < (x2 - x1) / 2; i++)
  {
    buffer[i] = color;
  }
  IODRV_WriteDataBlock(i);
  IODRV_TriggerXfer(io_handle);
}


void cDriverSSD1322::TestDirect(void)
{
  uint8_t* buffer;


  WriteCommand(kSSD1322CmdSetColumnAddress, 0x1C, 0x5B );
  WriteCommand(kSSD1322CmdSetRowAddress, 0x00, this->height);
  WriteCommand(kSSD1322CmdWriteData);
  buffer = IODRV_GetDataBuffer();
  for (uint32_t i = 0; i < this->bufferSize; i += 2)
  {
    buffer[i] = (i % 0x0A) & 0x0F;
    buffer[i + 1] = ((i % 0x0A) << 4) & 0xF0;
  }
  IODRV_WriteDataBlock(this->bufferSize);
  IODRV_TriggerXfer(io_handle);
}

void cDriverSSD1322::ClearDirect(void)
{
  uint8_t* buffer;


  WriteCommand(kSSD1322CmdSetColumnAddress, 0x1C, 0x5B);
  WriteCommand(kSSD1322CmdSetRowAddress, 0x00, this->height-1);
  WriteCommand(kSSD1322CmdWriteData);
  buffer = IODRV_GetDataBuffer();
  for (uint32_t i = 0; i < this->bufferSize; i++)
  {
    buffer[i] = 0x00;
  }
  IODRV_WriteDataBlock(this->bufferSize);
  IODRV_TriggerXfer(io_handle);
}

void cDriverSSD1322::WriteCommand(uint8_t command)
{
  IODRV_WriteCommand(command);
}

void cDriverSSD1322::WriteCommand(uint8_t command, uint8_t argument)
{
  IODRV_WriteCommand(command);
  IODRV_WriteData(argument);
}

void cDriverSSD1322::WriteCommand(uint8_t command, uint8_t argument1, uint8_t argument2)
{
  IODRV_WriteCommand(command);
  IODRV_WriteData(argument1);
  IODRV_WriteData(argument2);
}

} // end of namespace

