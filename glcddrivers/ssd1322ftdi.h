#ifndef _GLCDDRIVERS_SSD1322FTDI_H
#define _GLCDDRIVERS_SSD1322FTDI_H

#include <stdint.h>
#include <stdio.h>

#include "driver.h"


typedef void *  SED1322_HANDLE;
typedef int32_t SSD1322status_t;

typedef struct
{
  uint8_t     cmd;
  const int   *data;
} SSD1322CmdDataEntry_t;


namespace GLCD
{

class cDriverConfig;

class cDriverSSD1322 : public cDriver
{
private:
  SED1322_HANDLE io_handle;

  unsigned char *newLCD; // wanted state
  unsigned char *oldLCD; // current state
  uint32_t bufferSize;
  uint32_t gfxMemBytesOneRow;

  int refreshCounter;

  int CheckSetup();

  void Reset();
  void WriteCommand(uint8_t command);
  void WriteCommand(uint8_t command, uint8_t argument);
  void WriteCommand(uint8_t command, uint8_t argument1, uint8_t argument2);

public:
  cDriverSSD1322(cDriverConfig * config);
  virtual ~cDriverSSD1322();

  virtual int Init();
  virtual int DeInit();

  virtual void Clear();
  virtual void SetPixel(int x, int y, uint32_t data);
  //virtual void Set8Pixels(int x, int y, unsigned char data);
  virtual void Refresh(bool refreshAll = false);
  virtual void SetBrightness(unsigned int percent);

  void DrawHLine(uint8_t color, uint16_t x1, uint16_t y1, uint16_t x2);
  void DrawHLineDirect(uint8_t color, uint16_t x1, uint16_t y1, uint16_t x2);
  void TestDirect(void);
  void ClearDirect(void);
};

} // end of namespace

#endif
