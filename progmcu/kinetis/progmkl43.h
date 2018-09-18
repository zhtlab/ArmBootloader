/*
 * Copyright (c) 2018 zhtlab
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef	_PROGMKL43_H_
#define	_PROGMKL43_H_

/*
 * progmkl43.h:  progmkl43 header
 *
 */

#define	PROGMKL43_NUM_OF_UNIT		1


#define	PROGMKL43_RESULT_SUCCESS	0
#define	PROGMKL43_RESULT_UNKNOWN	(-1)
#define	PROGMKL43_RESULT_NOMEMORY	(-2)	/* can't allocate memory */
#define	PROGMKL43_RESULT_NOSPACE	(-3)	/* no space in the system */
#define	PROGMKL43_RESULT_INVALIDARG	(-4)	/* invalid argument */
#define	PROGMKL43_RESULT_PARAMEXCEEDED	(-5)	/* parameter exceeded */
#define	PROGMKL43_RESULT_PERM		(-6)	/* operation not permitted */
#define	PROGMKL43_RESULT_BUSY		(-7)	/* busy */
#define	PROGMKL43_RESULT_FIFO		(-8)	/* fifo error */


struct _stMkl43FramingPkt {
  uint8_t	start;
#define	PROGMKL43_PACKET_START_BYTE		0x5a

  uint8_t	type;
#define	PROGMKL43_PACKET_TYPE_ACK		0xa1
#define	PROGMKL43_PACKET_TYPE_NAK		0xa2
#define	PROGMKL43_PACKET_TYPE_ACK_ABOART	0xa3
#define	PROGMKL43_PACKET_TYPE_COMMAND		0xa4
#define	PROGMKL43_PACKET_TYPE_DATA		0xa5
#define	PROGMKL43_PACKET_TYPE_PING		0xa6
#define	PROGMKL43_PACKET_TYPE_PING_RESPONSE	0xa7

  uint8_t	size[2];
  uint8_t	crc16[2];
};
struct _stMkl43CommandPkt {
  uint8_t	tag;
#define	API_CMDTAG_FLASH_ERASE_ALL		0x01
#define	API_CMDTAG_FLASH_ERASE_REGION		0x02
#define		API_ERROR_SUCCESS				0
#define		API_ERROR_MEMORY_RANGE_INVALID			0x10200
#define		API_ERROR_MEMORY_FLASH_ALIGNMENT		0x1
#define		API_ERROR_MEMORY_FLASH_ADDRESS			0x2
#define		API_ERROR_MEMORY_FLASH_ACCESS			0x3
#define		API_ERROR_MEMORY_FLASH_PROTECTION_VIOLATION	0x4
#define		API_ERROR_MEMORY_FLASH_COMMAND			0x5

#define	API_CMDTAG_READ_MEMORY			0x03
#define	API_CMDTAG_WRITE_MEMORY			0x04
#define	API_CMDTAG_FILL_MEMORY			0x05
#define	API_CMDTAG_FLASH_SECURITY_DISABLE	0x06
#define	API_CMDTAG_GET_PROPERTY			0x07
#define		API_PROPERTY_LIST_PROPERTIES		0x00
#define		API_PROPERTY_BOOTLOADER_VERSION		0x01
#define		API_PROPERTY_AVAILABLE_PERIPHERALS	0x02
#define			PERIPHERAL_UART		(1 << 0)
#define			PERIPHERAL_I2CSlave	(1 << 1)
#define			PERIPHERAL_SPISlave	(1 << 2)
#define			PERIPHERAL_CAN		(1 << 3)
#define			PERIPHERAL_USB_HID	(1 << 4)
#define			PERIPHERAL_USB_CDC	(1 << 5)
#define			PERIPHERAL_USB_DFU	(1 << 6)
#define			PERIPHERAL_USB_MSC	(1 << 7)



#define		API_PROPERTY_FLASH_START_ADDR		0x03
#define		API_PROPERTY_FLASH_BYTE_SIZE		0x04
#define		API_PROPERTY_FLASH_SECTOR_SIZE		0x05
#define		API_PROPERTY_FLASH_BLOCK_COUNT		0x06
#define		API_PROPERTY_AVAILABLE_COMMANDS		0x07
#define		API_PROPERTY_CRC_STATUS			0x08
#define		API_PROPERTY_RESERVED9			0x09
#define		API_PROPERTY_VERIFY_WRITES		0x0a
#define		API_PROPERTY_MAX_PACKET_SIZE		0x0b
#define		API_PROPERTY_RESERVED_REGIONS		0x0c
#define		API_PROPERTY_VALIDATE_REGIONS		0x0d
#define		API_PROPERTY_RAM_START_ADDR		0x0e
#define		API_PROPERTY_RAM_BYTE_SIZE		0x0f
#define		API_PROPERTY_SYSTEM_DEVICE_ID		0x10
#define		API_PROPERTY_FLASH_SECURITY_STATUS	0x11
#define		API_PROPERTY_UNIQUE_DEVICE_ID		0x12
#define		API_PROPERTY_FAC_SUPPORT		0x13
#define		API_PROPERTY_FLASH_SEGMENT_SIZE		0x14
#define		API_PROPERTY_FLASH_SEGMENT_COUNT	0x15
#define		API_PROPERTY_FLASH_READ_MARGIN		0x16
#define		API_PROPERTY_QSPI_INIT_STATUS		0x17
#define		API_PROPERTY_END_OF_DEFAULT		0x17

#define		API_PROPERTY_STAART_OF_EXTENDED		0x80e0
#define		API_PROPERTY_PRODUCT_ID			0x80e0
#define		API_PROPERTY_PRODUCT_NAME		0x80e1
#define		API_PROPERTY_SERIAL_NUMBER		0x80e2
#define		API_PROPERTY_BOOTLOADER_VERSION_TEXT	0x80e3
#define		API_PROPERTY_BOARDID			0x80e4
#define		API_PROPERTY_END_OF_EXTENDED		0x80e4

#define	API_CMDTAG_RECEIVE_SB_FILE		0x08
#define	API_CMDTAG_EXECUTE			0x09
#define	API_CMDTAG_CALL				0x0a
#define	API_CMDTAG_RESET			0x0b
#define	API_CMDTAG_SET_PROPERTY			0x0c
#define	API_CMDTAG_FLASH_ERASE_ALL_UNSECURE	0x0d
#define	API_CMDTAG_FLASH_PROGARM_ONCE		0x0e
#define	API_CMDTAG_FLASH_RESET_ONCE		0x0f
#define	API_CMDTAG_FLASH_READ_RESOURCE		0x10
#define	API_CMDTAG_RESERVED11			0x11
#define	API_CMDTAG_FLASH_BLANK_CHECK		0x12
#define	API_CMDTAG_FLASH_CHECK_CRC		0x13

#define	API_RSPTAG_GENERIC_RESPONSE		0xa0
#define	API_RSPTAG_GET_PROPERTY_RESPONSE	0xa7
#define	API_RSPTAG_READ_MEMORY_RESPONSE		0xa3
#define	API_RSPTAG_FLASH_READ_ONCE_RESPONSE	0xaf
#define	API_RSPTAG_FLASH_READ_RESOURCE_RESPONSE	0xb0
  uint8_t	flags;
  uint8_t	reserved;
  uint8_t	count;
#define	PROGKL43_NUM_OF_PARAMS		7
#define	PROGKL43_NUM_OF_PaRAM_BYTES	4
  uint8_t	param[PROGKL43_NUM_OF_PARAMS][PROGKL43_NUM_OF_PaRAM_BYTES];
};
struct _stMkl43Pkt {
  struct _stMkl43FramingPkt	frame;
  union {
    struct _stMkl43CommandPkt	cmdres;
    uint8_t			data[32];
  };
};
struct _stMkl43PktPingRes {
  uint8_t	start;
  uint8_t	type;
  uint8_t	bugfix;
  uint8_t	minor;
  uint8_t	major;
  uint8_t	name;
  uint8_t	option[2];
  uint8_t	crc16[2];
};


#define	PROGMKL43_PACKET_PAYLOAD_SIZE		32

#define	PROGMKL43_ERASE_SECTOR_BYTES_SHIFT	10
#define	PROGMKL43_ERASE_SECTOR_BYTES		(1 << (PROGMKL43_ERASE_SECTOR_BYTES_SHIFT))


#define	PROGMKL43_FSEC_ADDR			0x40c



#define MAKE_STATUS(group, code) ((((group)*100) + (code)))
#define	kStatusGroup_Bootloader		(100)
enum _bootloader_status
{
    kStatus_UnknownCommand = MAKE_STATUS(kStatusGroup_Bootloader, 0),
    kStatus_SecurityViolation = MAKE_STATUS(kStatusGroup_Bootloader, 1),
    kStatus_AbortDataPhase = MAKE_STATUS(kStatusGroup_Bootloader, 2),
    kStatus_Ping = MAKE_STATUS(kStatusGroup_Bootloader, 3),
    kStatus_NoResponse = MAKE_STATUS(kStatusGroup_Bootloader, 4),
    kStatus_NoResponseExpected = MAKE_STATUS(kStatusGroup_Bootloader, 5)
};



#ifdef	_PROGMKL43_C_
struct _stProgmkl43Unit {
  int			unit;
  int			up;
#define	PROGMKL43_NUMOFCALLBACK		2
  int			(*pCb[PROGMKL43_NUMOFCALLBACK])(int unit,
					int cb, void *ptr, int size);

  int			modeSerial;
  /* SERIALMODE_XXX are defines in prog.h */


  uint32_t		supportedCmd;
#define	CMDTAG_MASK(x)		(1<<((x)-1))

  int			fCmdExtCheckBlank: 1;
  int			fCmdExtGetInfo: 1;
  int			fCmdExtGetCrc: 1;

  int			chipID;
  

  

};

struct _stProgmkl43 {
  int			up;
#define	PROGMKL43_NUMOFUNITS		1
  struct _stProgmkl43Unit	*sc[PROGMKL43_NUMOFUNITS];
};


struct _stProgmkl43Status {
  int		stat;
  int		sub;
};
#endif


/*******************************************************************
 * function macro
 */


/*******************************************************************
 * prototype
 */
int	Progmkl43Api(int unit, int req, void *ptr, int size);


/* internal prototype  (for static funciton) */
#ifdef	_PROGMKL43_C_
static int	Progmkl43Init(void);
static int	Progmkl43UnInit(void);
static int	Progmkl43InitUnit(void);
static int	Progmkl43UnInitUnit(int unit);
static void	Progmkl43Loop(void);

#if 0
static void *	Progmkl43Malloc(int size);
static void	Progmkl43Free(void *ptr);
#endif

static int	Progmkl43SetCallback(int unit, void *ptr);
static int	Progmkl43ResetCallback(int unit, void *ptr);
static int	Progmkl43IsCallback(int unit, void *ptr);



static int	Progmkl43ConnectDevice(int unit);
static int	Progmkl43DisconnectDevice(int unit);
/*static int	Progmkl43SendGetStatus(int unit);*/
static int	Progmkl43Reset(int unit);
static int	Progmkl43EraseChip(int unit);
static int	Progmkl43EraseBlock(int unit, struct _stProgData *ptr, int size);
static int	Progmkl43Program(int unit, struct _stProgData *ptr, int size);
static int	Progmkl43Read(int unit, struct _stProgData *ptr, int size);
static int	Progmkl43Verify(int unit, struct _stProgData *ptr, int size);
static int	Progmkl43CheckBlockBlank(int unit, struct _stProgData *ptr, int size);
static int	Progmkl43GetSignature(int unit, void *ptr, int size);
static int	Progmkl43GetInfo(int unit, void *ptr, int size);
static int	Progmkl43GetAddrInfo(int unit, void *ptr, int size);
static int	Progmkl43GetCrc(int unit, void *ptr, int size);
static int	Progmkl43CheckBlank(int unit, void *ptr, int size);
static int	Progmkl43GetVersion(int unit, void *ptr, int size);
static int	Progmkl43GetCheckSum(int unit, struct _stProgData *ptr, int size);
static int	Progmkl43SetSecurity(int unit, unsigned char *ptr, int size);
static int	Progmkl43SetSerialMode(int unit, int *pMode, int size);








static int	Progmkl43CmdWriteMemory(int unit, struct _stProgData *ptr, int size);
static int	Progmkl43CmdReadMemory(int unit, struct _stProgData *ptr, int size);
static int	Progmkl43CmdGo(int unit, struct _stProgData *ptr, int size);

static int	Progmkl43CmdGetInfo(int unit, uint8_t *ptr, int size);
static int	Progmkl43CmdGetCrc(int unit, struct _stProgData *ptr, int size);
static int	Progmkl43CmdCheckBlank(int unit, struct _stProgData *ptr, int size);





static void	Progmkl43StoreSupportedCommand(struct _stProgmkl43Unit *psc,
					       uint32_t val);




static int	Progmkl43CmdPing(int unit);
static int	Progmkl43CmdGetProperty(int unit, uint32_t *params, int *cnt);
static int	Progmkl43CmdFlashEraseAll(int unit);
static int	Progmkl43CmdEraseRegion(int unit, struct _stProgData *ptr, int size);



static int	Progmkl43PacketAssemble(struct _stMkl43Pkt *pkt, int type, int size, int tag, uint32_t *params, int cnt);
static int	Progmkl43PacketDisassemble(struct _stMkl43Pkt *pkt, int *type, int *size, int *tag, uint32_t *params, int *pCnt);
static int	Progmkl43SendPktWaitAck(int unit, struct _stMkl43Pkt *pkt);
static int	Progmkl43RecvPktSendAck(int unit, struct _stMkl43Pkt *pkt);


static void	Progmkl43ShowAllProperties(int unit);
static void	Progmkl43ShowErrorCode(int param);
#endif


#endif	/* _PROGMKL43_H_ */
