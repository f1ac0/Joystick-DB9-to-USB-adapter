/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_device.c
  * @version        : v2.0_Cube
  * @brief          : This file implements the USB Device
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customhid.h"
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/


// HID Game
struct gameHID_t {
  uint8_t id;
  uint8_t hat; //4bits hat + 4 placeholders
  uint8_t buttons; //8 buttons
//  int8_t x;
//  int8_t y;
};
struct gameHID_t gameHID;
struct gameHID_t gameHIDold;

// HID Mouse
struct mouseHID_t {
  uint8_t id;
  uint8_t buttons;
  int8_t x;
  int8_t y;
//  int8_t w;
};
struct mouseHID_t mouseHID;
struct mouseHID_t mouseHIDold;

// HID Keyboard
struct keyboardHID_t {
  uint8_t id;
  uint8_t modifiers;
  uint8_t key[6];
};
struct keyboardHID_t keyboardHID;

/*  // HID Media
struct mediaHID_t {
  uint8_t id;
  uint8_t keys;
};
struct mediaHID_t mediaHID;
*/

uint8_t keys_azerty[] = {
    0x00,0x2c,  //space
    0x00,0x38,  //!
    0x00,0x20,  //"
    0x40,0x20,  //#
    0x00,0x30,  //$
    0x02,0x34,  //%
    0x00,0x1e,  //&
    0x00,0x21,  //'
    0x00,0x22,  //(
    0x00,0x2d,  //)
    0x00,0x31,  //*
    0x02,0x2e,  //+
    0x00,0x10,  //,
    0x00,0x23,  //-
    0x02,0x36,  //.
    0x02,0x37,  ///
    0x02,0x27,  //0
    0x02,0x1e,
    0x02,0x1f,
    0x02,0x20,
    0x02,0x21,
    0x02,0x22,
    0x02,0x23,
    0x02,0x24,
    0x02,0x25,
    0x02,0x26,  //9
    0x00,0x37,  //:
    0x00,0x36,  //;
    0x00,0x64,  //<
    0x00,0x2e,  //=
    0x02,0x64,  //>
    0x02,0x10,  //?
    0x40,0x27,  //@
    0x02,0x14,  //A
    0x02,0x05,
    0x02,0x06,
    0x02,0x07,
    0x02,0x08,
    0x02,0x09,
    0x02,0x0a,
    0x02,0x0b,
    0x02,0x0c,
    0x02,0x0d,
    0x02,0x0e,
    0x02,0x0f,
    0x02,0x33,  //M
    0x02,0x11,
    0x02,0x12,
    0x02,0x13,
    0x02,0x04,  //Q
    0x02,0x15,
    0x02,0x16,
    0x02,0x17,
    0x02,0x18,
    0x02,0x19,
    0x02,0x1d,  //W
    0x02,0x1b,
    0x02,0x1c,
    0x02,0x1a,  //Z
    0x40,0x22,  //[
    0x40,0x25,  //backslash
    0x40,0x2d,  //]
    0x00,0x2f,  //^
    0x00,0x25,  //_
    0x40,0x24,  //`
    0x00,0x14,  //a
    0x00,0x05,
    0x00,0x06,
    0x00,0x07,
    0x00,0x08,
    0x00,0x09,
    0x00,0x0a,
    0x00,0x0b,
    0x00,0x0c,
    0x00,0x0d,
    0x00,0x0e,
    0x00,0x0f,
    0x00,0x33,  //m
    0x00,0x11,
    0x00,0x12,
    0x00,0x13,
    0x00,0x04,  //q
    0x00,0x15,
    0x00,0x16,
    0x00,0x17,
    0x00,0x18,
    0x00,0x19,
    0x00,0x1d,  //w
    0x00,0x1b,
    0x00,0x1c,
    0x00,0x1a,  //z
    0x40,0x21,  //{
    0x40,0x23,  //|
    0x40,0x2E,  //}
    0x40,0x1f,  //~
    0x00,0x2a,  //Delete
    0x00,0x26,  //128Ç
    0x00,0x18,  //129ü*
    0x00,0x1f,  //130é
    0x00,0x14,  //131â*
    0x00,0x14,  //132ä*
    0x00,0x27,  //133à
    0x00,0x14,  //134å*
    0x00,0x26,  //135ç
    0x00,0x08,  //136ê*
    0x00,0x08,  //137ë*
    0x00,0x24,  //138è
    0x00,0x0c,  //139ï*
    0x00,0x0c,  //140î*
    0x00,0x0c,  //141ì*
    0x02,0x14,  //142Ä*
    0x02,0x14,  //143Å*
    0x02,0x08,  //144É*
    0x00,0x14,  //145æ*
    0x02,0x14,  //146Æ*
    0x00,0x12,  //147ô*
    0x00,0x12,  //148ö*
    0x00,0x12,  //149ò*
    0x00,0x18,  //150û*
    0x00,0x34,  //151ù
    0x00,0x1c,  //152ÿ*
    0x02,0x12,  //153Ö*
    0x02,0x18,  //154Ü*
    0x02,0x27,  //155ø*
    0x02,0x08,  //156£*
    0x02,0x27,  //157Ø*
    0x00,0x1b,  //158×*
    0x00,0x09,  //159ƒ*
    0x00,0x14,  //160á*
    0x00,0x0c,  //161í*
    0x00,0x12,  //162ó*
    0x00,0x34,  //163ú

};

uint8_t mode=0;
uint8_t cd32=0;
uint32_t counter=0;

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceFS;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

void GameInit(void)
{
  gameHID.id=1;
  gameHID.hat=0;
  gameHID.buttons=0;
}

void MouseInit(void)
{
  mouseHID.id=2;
  mouseHID.buttons=0;
  mouseHID.x=0;
  mouseHID.y=0;
//  mouseHID.w=0;
//  mouseHID.motionwakeup=0;
}

void KbdInit(void)
{
  keyboardHID.id = 3;
  keyboardHID.modifiers = 0;
  keyboardHID.key[0] = 0;
  keyboardHID.key[1] = 0;
  keyboardHID.key[2] = 0;
  keyboardHID.key[3] = 0;
  keyboardHID.key[4] = 0;
  keyboardHID.key[5] = 0;
//  mediaHID.id = 2;
//  mediaHID.keys = 0;
}

#define SYSTICK_LOAD (SystemCoreClock/1000000U)
#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)

#define DELAY_US(us) \
    do { \
         uint32_t start = SysTick->VAL; \
         uint32_t ticks = (us * SYSTICK_LOAD)-SYSTICK_DELAY_CALIB;  \
         while((start - SysTick->VAL) < ticks); \
    } while (0)

uint16_t CD32Buttons(void)
{
  uint16_t b=0;
  //fire1=CLK low
  GPIOB->BSRR = 0b00001000000000000000000000000000;
  //fire1 = output
  GPIOB->CRH |= 0b00000000000000000011000000000000;
  DELAY_US(15); //HAL_Delay(1); //have to lower all these delays to a few microseconds
  //fire3=LOAD_SHIFT low, fire1=CLK low
  GPIOB->BSRR = 0b00001000000000000000000000000000;
  //fire3 and fire1 = output
  GPIOB->CRH |= 0b00000000000000000011000000110000; //fire3=pin B9 and fire1=B11 to output 50MHz open drain MODE (CNF for floating input and open drain is the same)
  DELAY_US(15);

  for(uint8_t i=9; i>0; i--){
    //shift fire2 serial value
    b = (b>>1) | (((~GPIOB->IDR) & 0b0000010000000000)>>2);
    //pulse fire1=CLK high to shift the next value
    GPIOB->BSRR = 0b00000000000000000000100000000000;
    DELAY_US(15);
    //pulse fire1=CLK low
    GPIOB->BSRR = 0b00001000000000000000000000000000;
    DELAY_US(15);
  }

  //fire1 = CLK input
  GPIOB->CRH &= ~0b00000000000000000011000000000000; //fire3=pin B9 and fire1=B11 to input
  DELAY_US(15);
  //fire3 = LOAD_SHIFT input
  GPIOB->CRH &= ~0b00000000000000000011000000110000; //fire3=pin B9 and fire1=B11 to input

  //switch red and blue buttons
  b = (b & 0b111111100) | (b & 0b01)<<1 | (b & 0b10)>>1;

  //if last two fire2 values =01 then we have a cd32 pad

  return b;
}

//Move the pointer randomly
void MouseMove(void)
{
//  mouseHID.id=1;
//  mouseHID.buttons=0;
  mouseHID.x=rand()/(RAND_MAX/20)-10;
  mouseHID.y=rand()/(RAND_MAX/20)-10;
//  mouseHID.w=0;
  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&mouseHID, sizeof(struct mouseHID_t));

}


/*
void KbdVolDec(void)
{
    mediaHID.keys = USB_HID_VOL_DEC;
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&mediaHID, sizeof(struct mediaHID_t));
    HAL_Delay(30);
    mediaHID.keys = 0;
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&mediaHID, sizeof(struct mediaHID_t));
    HAL_Delay(30);
}*/

/*
#define USB_HID_KEY_L     0x0F
void KbdShiftL(void)
{
    keyboardHID.modifiers = USB_HID_MODIFIER_RIGHT_SHIFT;
    keyboardHID.key1 = USB_HID_KEY_L;
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboardHID, sizeof(struct keyboardHID_t));
    HAL_Delay(30);
    keyboardHID.modifiers = 0;
    keyboardHID.key1 = 0;
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboardHID, sizeof(struct keyboardHID_t));
    HAL_Delay(30);
}*/

void KbdKeys(uint8_t modifiers, uint8_t* keys){
    keyboardHID.modifiers = modifiers;
    keyboardHID.key[1] = keys[0];
}

void KbdKey(uint8_t modifiers, uint8_t key1, uint8_t release)
{
    if(keyboardHID.key[5] == key1){ //if same key then release before sending another key
        KbdKey(0,0,0);
        HAL_Delay(10);
    }

    keyboardHID.modifiers = modifiers;
    keyboardHID.key[5] = key1;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboardHID, sizeof(struct keyboardHID_t));
    HAL_Delay(20);
    if(release) {
        KbdKey(0,0,0);
        HAL_Delay(10);
    }
}

void KbdChar(char c, uint8_t release)
{
    KbdKey(keys_azerty[((int)(c-' '))*2], keys_azerty[((int)(c-' '))*2+1], release);
}

void KbdStr2(char* str)
{
    int i;
    for (i = 0; str[i] != 0; i++){
        KbdChar(str[i],0);
    }
    KbdKey(0,0,0);
}

void KbdStr(char* str)
{
    int i=0,b=0;
    uint8_t modifiers=0, key=0, keyinbuf=0;
    keyboardHID.key[0]=keyboardHID.key[1]=keyboardHID.key[2]=keyboardHID.key[3]=keyboardHID.key[4]=keyboardHID.key[5]=0;

    while(str[i] != 0){
        modifiers=keys_azerty[((int)(str[i]-' '))*2];
        key=keys_azerty[((int)(str[i]-' '))*2+1];
        keyinbuf= key==keyboardHID.key[0] ||key==keyboardHID.key[1] ||key==keyboardHID.key[2] ||key==keyboardHID.key[3] ||key==keyboardHID.key[4] ||key==keyboardHID.key[5]; //set if key already in buffer

        if(b==6 || (b>0 && modifiers!=keyboardHID.modifiers) || keyinbuf) {

            USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboardHID, sizeof(struct keyboardHID_t));
            HAL_Delay(3);
            b=0;
//            if(keyinbuf){ //If already in buffer we have to release it before sending it again
                keyinbuf=0;
                keyboardHID.key[0]=keyboardHID.key[1]=keyboardHID.key[2]=keyboardHID.key[3]=keyboardHID.key[4]=keyboardHID.key[5]=0;
                USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboardHID, sizeof(struct keyboardHID_t));
                HAL_Delay(3);
//            }
        }
        keyboardHID.modifiers = modifiers;
        keyboardHID.key[b] = key;
        b++;
        i++;
    }
    if(b>0){ //If we have a buffer we send it
        USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboardHID, sizeof(struct keyboardHID_t));
        HAL_Delay(3);
    }
    //Then clear all the keys
    keyboardHID.modifiers=keyboardHID.key[0]=keyboardHID.key[1]=keyboardHID.key[2]=keyboardHID.key[3]=keyboardHID.key[4]=keyboardHID.key[5]=0;
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboardHID, sizeof(struct keyboardHID_t));
    HAL_Delay(3);

}

int16_t hatdir[16] = {8, 0, 4, 8, 6, 7, 5, 8, 2, 1, 3, 8, 8, 8, 8, 8};

void USB_DEVICE_Process(void)
{
	if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED)
		return;

  uint32_t pb = ~GPIOB->IDR;
  static uint32_t opb=0;

  switch(mode){
    case 0: //game
      gameHID.hat = hatdir[(pb>>12)&0b0000000000001111];
      if(cd32)
        gameHID.buttons = CD32Buttons();
      else
        gameHID.buttons = (pb>>7 & 0b100) | (pb>>9 & 0b010) | (pb>>11 & 0b001); //swap button order;
      if(gameHID.buttons != gameHIDold.buttons || gameHID.hat != gameHIDold.hat)
	    if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&gameHID, sizeof(struct gameHID_t)) == USBD_OK) {
	    	gameHIDold.hat = gameHID.hat;
	    	gameHIDold.buttons = gameHID.buttons;
	    }
      break;

    case 1: //amiga mouse
      //x=down & right ; y=up & left
      if((pb & 0b1000000000000000) && !(opb & 0b1000000000000000))
        mouseHID.x += (pb & 0b0010000000000000)?10:-10;
      if((pb & 0b0100000000000000) && !(opb & 0b0100000000000000))
        mouseHID.y += (pb & 0b0001000000000000)?10:-10;
      opb = pb;
      mouseHID.buttons = (pb>>7 & 0b100) | (pb>>9 & 0b010) | (pb>>11 & 0b001); //swap button order;
      if(mouseHID.x != 0 || mouseHID.y != 0 || mouseHID.buttons != mouseHIDold.buttons)
        if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&mouseHID, sizeof(struct mouseHID_t)) == USBD_OK) {
          mouseHIDold.buttons = mouseHID.buttons;
          mouseHID.x = 0;
          mouseHID.y = 0;
        }
      break;

    case 2: //game as mouse
      if(counter++ > 5000) {
        counter=0;
        mouseHID.x += ((pb & 0b1000000000000000)?1:0) + ((pb & 0b0100000000000000)?-1:0);
        mouseHID.y += ((pb & 0b0010000000000000)?1:0) + ((pb & 0b0001000000000000)?-1:0);
      }
      mouseHID.buttons = (pb>>7 & 0b100) | (pb>>9 & 0b010) | (pb>>11 & 0b001); //swap button order
      if(mouseHID.x != 0 || mouseHID.y != 0 || mouseHID.buttons != mouseHIDold.buttons)
        if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&mouseHID, sizeof(struct mouseHID_t)) == USBD_OK) {
          mouseHIDold.buttons = mouseHID.buttons;
          mouseHID.x = 0;
          mouseHID.y = 0;
        }
      break;

    case 3: //atari mouse
      //x=down & up ; y=right & left
      if((pb & 0b0001000000000000) && !(opb & 0b0001000000000000))
        mouseHID.x += (pb & 0b0010000000000000)?10:-10;
      if((pb & 0b0100000000000000) && !(opb & 0b0100000000000000))
        mouseHID.y += (pb & 0b1000000000000000)?10:-10;
      opb = pb;
      mouseHID.buttons = (pb>>7 & 0b100) | (pb>>9 & 0b010) | (pb>>11 & 0b001); //swap button order;
      if(mouseHID.x != 0 || mouseHID.y != 0 || mouseHID.buttons != mouseHIDold.buttons)
        if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&mouseHID, sizeof(struct mouseHID_t)) == USBD_OK) {
          mouseHIDold.buttons = mouseHID.buttons;
          mouseHID.x = 0;
          mouseHID.y = 0;
        }
     break;
  }

}

/* USER CODE END 1 */

/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_DEVICE_Init(void)
{
  /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */
  KbdInit();
  GameInit();
  MouseInit();

  //Configuration at power up
  //mode 0 = no button = game
  //mode 1 = right button = amiga mouse
  //mode 2 = left button = game as mouse
  //mode 3 = both button = atari mouse
  mode = ((~GPIOB->IDR)>>10) & 0b11;

  //Autodetect CD32 buttons
  //fire3 = output low
  //fire1 = output clock
  //pulse fire1 8 times
  //shift fire2 values
  //if last fire2 value =0 then 1 we have a cd32 pad
  cd32 = (CD32Buttons()>>7) == 0b10;


  /* USER CODE END USB_DEVICE_Init_PreTreatment */

  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CUSTOM_HID) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceFS, &USBD_CustomHID_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN USB_DEVICE_Init_PostTreatment */

  /* USER CODE END USB_DEVICE_Init_PostTreatment */
}

/**
  * @}
  */

/**
  * @}
  */

