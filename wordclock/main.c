#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "ldr.h"
#include "i2c-rtc.h"

#define BIT(a) (1 << a)

void
InitPWM()
{
  DDRD = 0xfc; //(1 << PD4) || (1 << PD5) || (1 << PD7);


  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
  TCCR1B = (1 << CS10);
  TCCR2 = (1 << CS20) | (1 << WGM20) | (1 << COM21);
  TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (01 << COM00) | (1 << CS00) | (1 << CS02);

  TIMSK &= ~0x3c;
}

void
SetPWMOutput(uint8_t duty)
{
  OCR1AL = duty;
  OCR1BL = 255 - duty;
  OCR0 = duty;
  OCR2 = duty;
}

void
SetColor(uint8_t bright, uint8_t uiR, uint8_t uiG, uint8_t uiB)
{
  char r = ((((uint16_t) bright) * uiR) / 255);
  char g = ((((uint16_t) bright) * uiG) / 255);
  char b = ((((uint16_t) bright) * uiB) / 255);
  OCR1BL = r;
  OCR1AL = g;
  OCR2 = b;
}

#ifndef cbi
#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void
uartPutc(char c)
{
  while (!(UCSRA & (1 << UDRE)))
    ;
  UDR = c;
}

void
uartPuts(char *s)
{
  int x = 0;
  while (s[x])
  {
    uartPutc(s[x]);
    x++;
  }
}

#define SHIFT_SR_SPI_DDR  DDRB
#define SHIFT_SR_SPI_PORT PORTB
#define SHIFT_SR_SPI_MOSI PIN5
#define SHIFT_SR_SPI_MISO PIN6
#define SHIFT_SR_SPI_RCLK PIN4
#define SHIFT_SR_SPI_SCK  PIN7

void
shift32_output(uint32_t value)
{
  uint8_t u0 = (uint8_t) (value);
  uint8_t u1 = (uint8_t) (value >> 8);
  uint8_t u2 = (uint8_t) (value >> 16);
  uint8_t u3 = (uint8_t) (value >> 24);

  SPDR = u3;
  while (!(SPSR & (1 << SPIF)))
    ;

  SPDR = u2;
  while (!(SPSR & (1 << SPIF)))
    ;

  SPDR = u1;
  while (!(SPSR & (1 << SPIF)))
    ;

  SPDR = u0;
  while (!(SPSR & (1 << SPIF)))
    ;

  /* latch data */
  SHIFT_SR_SPI_PORT &= ~(1 << SHIFT_SR_SPI_RCLK);
  SHIFT_SR_SPI_PORT |= (1 << SHIFT_SR_SPI_RCLK);
}

void
shift_init(void)
{
  SHIFT_SR_SPI_DDR |= (1 << SHIFT_SR_SPI_MOSI) | (1 << SHIFT_SR_SPI_RCLK) | (1 << SHIFT_SR_SPI_SCK);
  SHIFT_SR_SPI_DDR &= ~(1 << SHIFT_SR_SPI_MISO); /* MISO muss eingang sein */
  SHIFT_SR_SPI_PORT |= (1 << SHIFT_SR_SPI_RCLK) | (1 << SHIFT_SR_SPI_MISO);

  SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << SPR0) | (1 << SPR1);

  SPSR |= (1 << SPI2X);

  shift32_output(0); /* send dummybytes to intialize */
}

#define UART_MAXSTRLEN 100

volatile uint8_t uart_str_complete = 0;
volatile uint8_t uart_str_count = 0;
volatile char uart_string[UART_MAXSTRLEN + 1] = "";

ISR(SIG_UART_RECV)
{
  unsigned char nextChar;

  nextChar = UDR;
  if (uart_str_complete == 0)
  {

    if (nextChar != '\n' && nextChar != '\r' && uart_str_count < UART_MAXSTRLEN - 1)
    {
      uart_string[uart_str_count] = nextChar;
      uart_str_count++;
    }
    else
    {
      uart_string[uart_str_count] = '\0';
      uart_str_count = 0;
      uart_str_complete = 1;
    }
  }
}

int
hex2dez_c(char h)
{
  int res = -1;
  if (h >= '0' && h <= '9')
  {
    res = (h - '0');
  }
  else if (h >= 'A' && h <= 'F')
  {
    res = (h - 'A' + 10);
  }
  else if (h >= 'a' && h <= 'f')
  {
    res = (h - 'a' + 10);
  }
  return res;
}

int
hex2dez(char *h)
{
  int res1 = hex2dez_c(h[0]);
  int res2 = hex2dez_c(h[1]);
  if (res1 < 0 || res2 < 0)
    return -1;
  return (res1 << 4) + res2;
}

#define def_es       0
#define def_ist      1
#define def_fuenfM   2
#define def_zehnM    3
#define def_zwanzigM 4
#define def_dreiM    5
#define def_viertelM 6
#define def_nach     7
#define def_vor      8
#define def_halb     9
#define def_zwoelfH  10
#define def_zweiH    11
#define def_einsH    12
#define def_siebenH  13
#define def_dreiH    14
#define def_fuenfH   15
#define def_elfH     16
#define def_neunH    17
#define def_vierH    18
#define def_achtH    19
#define def_zehnH    20
#define def_sechsH   21
#define def_uhr      22
#define def_mp1      23
#define def_mp2      24
#define def_mp3      25
#define def_mp4      26

const long words[30] PROGMEM =
{ 0b10000000000000000000000000000000, // es
0b01000000000000000000000000000000, // ist
0b00100000000000000000000000000000, // fünf
0b00010000000000000000000000000000, // zehn
0b00001000000000000000000000000000, // zwanzig
0b00000100000000000000000000000000, // drei
0b00000010000000000000000000000000, // viertel
0b00000001000000000000000000000000, // nach
0b00000000100000000000000000000000, // vor
0b00000000010000000000000000000000, // halb
0b00000000001000000000000000000000, // zwölf
0b00000000000110000000000000000000, // zwei
0b00000000000011000000000000000000, // ein
0b00000000000000100000000000000000, // sieben
0b00000000000000010000000000000000, // drei
0b00000000000000001000000000000000, // fünf
0b00000000000000000100000000000000, // elf
0b00000000000000000010000000000000, // neun
0b00000000000000000001000000000000, // vier
0b00000000000000000000100000000000, // acht
0b00000000000000000000010000000000, // zehn
0b00000000000000000000001000000000, // sechs
0b00000000000000000000000100000000, // uhr
0b00000000000000000000000010000000, // m+1
0b00000000000000000000000001000000, // m+2
0b00000000000000000000000000100000, // m+3
0b00000000000000000000000000010000, // m+4
0b00000000000000000000000000001000, //
0b00000000000000000000000000000100, //
0b00000000000000000000000000000010 //
    };

void
TimeInfo(DATETIME time)
{
  char s[100];
  sprintf(s, "time :  %02d:%02d:%02d %s %02d.%02d.%4d UTC%+d %s\r\n", time.hh, time.mm, time.ss, "x", time.DD, time.MM, time.YY + 2000, rtc_offset, time.dst != 0 ? "DST" : "");
  uartPuts(s);
  sprintf(s, "Sun: %%%d %02d:%02d - %02d:%02d\r\n", time.sunrise, time.sunrisehh, time.sunrisemm, time.sunfallhh, time.sunfallmm);
  uartPuts(s);
}

int
main()
{
  cli();

  UCSRB |= (1 << TXEN) | (1 << RXEN) | (1 << RXCIE);
  UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

  UBRRH = 0x00;
  UBRRL = 0x08;

  uartPuts("WordClock V0.1 initializing...\r\n");

  uartPuts("\r\n... PWM");
  InitPWM();
  SetColor(0xFF, 0xFF, 0x00, 0x00);

  uartPuts("\r\n... Shifter");
  shift_init();
  SetColor(0xFF, 0xFF, 0xFF, 0x00);

  uartPuts("\r\n... LDR ADC");
  ldr_init();
  SetColor(0xFF, 0x00, 0xFF, 0xFF);

  uartPuts("\r\n... RTC");
  DATETIME time;
  DATETIME utctime;
  uint8_t i2c_errorcode;
  uint8_t i2c_status;
  if (!i2c_rtc_init(&i2c_errorcode, &i2c_status)) // initialize rtc
  {
    uartPuts(" FAILED !!!\r\n");
    for (int uiCount = 0; uiCount < 10; uiCount++)
    {
      SetColor(0xFF, 0x00, 0x00, 0x00);
      _delay_ms(20);
      SetColor(0xFF, 0xff, 0xff, 0xff);
      _delay_ms(20);
    }
  }

  SetColor(0xFF, 0xFF, 0xFF, 0xFF);
  sei();

  uartPuts("\r\nReady...\r\n");

  int uiR = 0x00;
  int uiG = 0x00;
  int uiB = 0x00;
  int uiRGB = 1;

  uint8_t uiBrightControl = 1;
  read_byte(cBrightControl, &uiBrightControl);

  unsigned long lLEDs_LastValue = 0;

  int uiCount = 0;

  uint16_t Button1 = 0;
  uint16_t Button2 = 0;

  while (1)
  {

    _delay_ms(10);
    uiCount++;

    ldr_read();

    uint8_t uiBright = 0xff;
    if (uiBrightControl != 0)
    {
      uiBright = ldr_get_brightness();
    }
    if (uiBright < 64)
    {
      uiBright = (uiBright >> 1) + 32;
    }
    SetColor(uiBright, uiR, uiG, uiB);

    if (uiCount % 10)
    {
      continue;
    }

    if (uart_str_complete)
    {
      switch (uart_string[0])
      {
      case '?':
        {
//          TimeInfo(time);
        }
        break;
      case 'b':
        {
          uiBrightControl = uiBrightControl != 0 ? 0 : 1;
          save_byte(cBrightControl, uiBrightControl);
          char s[100];
          sprintf(s, "brightness control is %s\r\n", uiBrightControl != 0 ? "active" : "inactive");
          uartPuts(s);
        }
        break;
      case 'c':
        {
          int r = hex2dez((char*) &uart_string[1]);
          int g = hex2dez((char*) &uart_string[3]);
          int b = hex2dez((char*) &uart_string[5]);
          if (r < 0 || g < 0 || b < 0)
          {
            uiRGB = 1;
            break;
          }
          uiRGB = 0;
          uiR = r;
          uiG = g;
          uiB = b;
          SetColor(uiBright, uiR, uiG, uiB);
        }
        break;
      case '+':
        {
          int res = i2c_rtc_read(&time, 0);
          add_minute(&time);
          res = i2c_rtc_write(&time);
        }
        break;
      case '-':
        {
          int res = i2c_rtc_read(&time, 0);
          sub_minute(&time);
          res = i2c_rtc_write(&time);
        }
        break;
      case 'z':
        switch (uart_string[1])
        {
        case '+':
          {
            if (rtc_offset > 11)
              rtc_offset = -13;
            set_offset(rtc_offset + 1);
          }
          break;
        case '-':
          {
            if (rtc_offset < 11)
              rtc_offset = 13;
            set_offset(rtc_offset - 1);
          }
          break;
        default:
          {
            char s[100];
            sprintf(s, "z macht keinen sinn mit '%c'...\r\n", uart_string[1]);
            uartPuts(s);
          }
          break;
        }
        break;
      case 'h':
        switch (uart_string[1])
        {
        case '+':
          {
            int res = i2c_rtc_read(&time, 0);
            add_hour(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        case '-':
          {
            int res = i2c_rtc_read(&time, 0);
            sub_hour(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        default:
          {
            char s[100];
            sprintf(s, "h macht keinen sinn mit '%c'...\r\n", uart_string[1]);
            uartPuts(s);
          }
          break;
        }
        break;
      case 'm':
        switch (uart_string[1])
        {
        case '+':
          {
            int res = i2c_rtc_read(&time, 0);
            add_minute(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        case '-':
          {
            int res = i2c_rtc_read(&time, 0);
            sub_minute(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        default:
          {
            char s[100];
            sprintf(s, "m macht keinen sinn mit '%c'...\r\n", uart_string[1]);
            uartPuts(s);

          }
          break;
        }
        break;
      case 'D':
        switch (uart_string[1])
        {
        case '+':
          {
            int res = i2c_rtc_read(&time, 0);
            add_day(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        case '-':
          {
            int res = i2c_rtc_read(&time, 0);
            sub_day(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        default:
          {
            char s[100];
            sprintf(s, "D macht keinen sinn mit '%c'...\r\n", uart_string[1]);
            uartPuts(s);
          }
          break;
        }
        break;
      case 'M':
        switch (uart_string[1])
        {
        case '+':
          {
            int res = i2c_rtc_read(&time, 0);
            add_month(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        case '-':
          {
            int res = i2c_rtc_read(&time, 0);
            sub_month(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        default:
          {
            char s[100];
            sprintf(s, "M macht keinen sinn mit '%c'...\r\n", uart_string[1]);
            uartPuts(s);
          }
          break;
        }
        break;
      case 'Y':
        switch (uart_string[1])
        {
        case '+':
          {
            int res = i2c_rtc_read(&time, 0);
            add_year(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        case '-':
          {
            int res = i2c_rtc_read(&time, 0);
            sub_year(&time);
            res = i2c_rtc_write(&time);
          }
          break;
        default:
          {
            char s[100];
            sprintf(s, "Y macht keinen sinn mit '%c'...\r\n", uart_string[1]);
            uartPuts(s);
          }
          break;
        }
        break;
      default:
        {
          char s[100];
          sprintf(s, "was soll ich mit '%s' anfangen?\r\n", uart_string);
          uartPuts(s);

        }
      }
      int res1 = i2c_rtc_read(&time, 1);
      int res2 = i2c_rtc_read(&utctime, 0);
      TimeInfo(time);
      uart_str_complete = 0;
    }

    if (!(PINA & (1 << PA7)))
    {
      Button1++;
    }
    else
    {
      Button1 = 0;
    }

    if (!(PINA & (1 << PA6)))
    {
      Button2++;
    }
    else
    {
      Button2 = 0;
    }

    if (Button1 > 1)
    {
      if (((Button1 + 10) % 12) == 0 || Button1 > 36)
      {
        uartPuts("Hour++\r\n");
        int res = i2c_rtc_read(&time, 0);
        add_hour(&time);
        res = i2c_rtc_write(&time);
        char s[100];
        sprintf(s, "time : %02d:%02d:%02d\r\n", time.hh, time.mm, time.ss);
        uartPuts(s);
      }
    }

    //		if (Button2 > 1) {
    //			if (((Button2 + 10) % 12) == 0 || Button2 > 36) {
    //				uartPuts("Min++\r\n");
    //				res = i2c_rtc_read(&time, 1);
    //				SetColor(bright, uiR, uiG, uiB);
    //				add_minute(time);
    //				time.ss = 0;
    //				if (time.mm > 59)
    //					time.mm = 0;
    //				i2c_rtc_write(&time);
    //				char s[100];
    //				sprintf(s, "time : %02d:%02d:%02d\r\n", time.hh, time.mm,
    //						time.ss);
    //				uartPuts(s);
    //			}
    //		}

    //		if(PIND & (1<< PD2))
    //		{
    //			uartPuts("1\r\n");
    //		}
    //		else
    //		{
    //			uartPuts("0\r\n");
    //		}

    if (uiCount % 100)
    {
      continue;
    }

    int res1 = i2c_rtc_read(&time, 1);
    int res2 = i2c_rtc_read(&utctime, 0);
    SetColor(uiBright, uiR, uiG, uiB);
    if (!res1 || !res2)
    {
      uartPuts("RTC error\r\n");
      for (uiCount = 0; uiCount < 40; uiCount++)
      {
        SetColor(0x01, 0x01, 0x01, 0x01);
        _delay_ms(50);
        SetColor(0xff, 0xff, 0xff, 0xff);
        _delay_ms(50);
      }
      continue;
    }

    long lLEDs = 0;
    lLEDs |= pgm_read_dword(words+def_es);
    lLEDs |= pgm_read_dword(words+def_ist);
    lLEDs |= pgm_read_dword(words+def_uhr);

    switch (time.mm % 5)
    {
    case 0:
      break;
    case 1:
      lLEDs |= pgm_read_dword(words+def_mp1);
      break;
    case 2:
      lLEDs |= pgm_read_dword(words+def_mp2);
      break;
    case 3:
      lLEDs |= pgm_read_dword(words+def_mp3);
      break;
    case 4:
      lLEDs |= pgm_read_dword(words+def_mp4);
      break;
    }
    uint8_t hoffset = 0;
    if (time.mm < 5)
    {
    }
    else if (time.mm < 10)
    {
      lLEDs |= pgm_read_dword(words+def_fuenfM);
      lLEDs |= pgm_read_dword(words+def_nach);
    }
    else if (time.mm < 15)
    {
      lLEDs |= pgm_read_dword(words+def_zehnM);
      lLEDs |= pgm_read_dword(words+def_nach);
    }
    else if (time.mm < 20)
    {
      lLEDs |= pgm_read_dword(words+def_viertelM);
      lLEDs |= pgm_read_dword(words+def_nach);
    }
    else if (time.mm < 25)
    {
      lLEDs |= pgm_read_dword(words+def_zwanzigM);
      lLEDs |= pgm_read_dword(words+def_nach);
    }
    else if (time.mm < 30)
    {
      lLEDs |= pgm_read_dword(words+def_fuenfM);
      lLEDs |= pgm_read_dword(words+def_vor);
      lLEDs |= pgm_read_dword(words+def_halb);
      hoffset = 1;
    }
    else if (time.mm < 35)
    {
      lLEDs |= pgm_read_dword(words+def_halb);
      hoffset = 1;
    }
    else if (time.mm < 40)
    {
      lLEDs |= pgm_read_dword(words+def_fuenfM);
      lLEDs |= pgm_read_dword(words+def_nach);
      lLEDs |= pgm_read_dword(words+def_halb);
      hoffset = 1;
    }
    else if (time.mm < 45)
    {
      lLEDs |= pgm_read_dword(words+def_zwanzigM);
      lLEDs |= pgm_read_dword(words+def_vor);
      hoffset = 1;
    }
    else if (time.mm < 50)
    {
      lLEDs |= pgm_read_dword(words+def_viertelM);
      lLEDs |= pgm_read_dword(words+def_vor);
      hoffset = 1;
    }
    else if (time.mm < 55)
    {
      lLEDs |= pgm_read_dword(words+def_zehnM);
      lLEDs |= pgm_read_dword(words+def_vor);
      hoffset = 1;
    }
    else if (time.mm < 60)
    {
      lLEDs |= pgm_read_dword(words+def_fuenfM);
      lLEDs |= pgm_read_dword(words+def_vor);
      hoffset = 1;
    }

    int
        def_h[25] =
        { def_zwoelfH, def_einsH, def_zweiH, def_dreiH, def_vierH, def_fuenfH, def_sechsH, def_siebenH, def_achtH, def_neunH, def_zehnH, def_elfH, def_zwoelfH, def_einsH, def_zweiH, def_dreiH, def_vierH, def_fuenfH, def_sechsH, def_siebenH, def_achtH, def_neunH, def_zehnH, def_elfH, def_zwoelfH };
    lLEDs |= pgm_read_dword(words + def_h[time.hh + hoffset]);

    if (lLEDs_LastValue != lLEDs)
    {
      shift32_output(lLEDs);
      lLEDs_LastValue = lLEDs;
    }

    if (time.ss == 0)
    {
      TimeInfo(time);
    }

    if (uiRGB)
    {
      if (time.sunrise == 0)
      {
        uiR = 0x01;
        uiG = 0x01;
        uiB = 0x33;
      }
      else if (time.sunrise >= 100)
      {
        uiR = 0xff;
        uiG = 0xff;
        uiB = 0xc0;
      }
      else if (time.sunrise < 50)
      {
        uiR = 0x01 + (time.sunrise * 0xfe) / 50;
        uiG = 0x01;
        uiB = 0x33;
      }
      else if (time.sunrise > 50)
      {
        uiR = 0xff;
        long x = (time.sunrise - 50);
        uiG = 0x33 + ((0xcc * (x)) / 50);
        uiB = 0x33 + ((0xac * (x)) / 50);
      }
    }

    if (uiCount % 1000)
    {
      uiCount = 0;
    }
  }
  return 0;
}
