
#ifndef __DEFS_H__
#define __DEFS_H__

/***********************************************************************************
 * @file    : defs.h
 * @brief   : keyword define
 *           
 *           
 * @version : 2.0
 * @date    : 2022/11/20
 * @author  : Yoon-Hwan Joo (tp.joo@daum.net or toppy_joo@naver.com) 
 ************************************************************************************/



#define RD_BUF_SIZ 				1024*10

#define MULTI_IN_FILES_CNT 			(4096)
#define LENGTH_OF_FILENAME			16 /* 16*16 -> 256 */
#define MAX_CHARS 					16
#define MAX_VERSION_LEN 			16 // 2017.12.12
#define MAX_CRC_LEN_CODE 			16 // 2017.11.21
#define MAX_HASH_CODE 				128 // 2017.11.21
#define MAX_32CHARS 				32 // 2022-10-18

#define MAX_FILENAME_LEN 			256

#define SPACE_FILL1 		'\0'
#define SPACE_FILL2 		'\0' //0xFF
#define SPACE_FILL3 		'\0'
#define SPACE_FILL4 		'\0'

#define DUMMY_FILL 			"TOPJOOhdr"
#define DUMMY_FILL_LEN 		9

#define TRUE 		1
#define FALSE 		0


#define CHECK_BIT_IN 		0x01
#define CHECK_BIT_OUT 		0x02


#define ATT_VERSION 		0x0001 /* 16Bytes */
#define ATT_DATEorCRC 		0x0002 /* 16Bytes or variable */
#define ATT_MODEL 			0x0004 /* 16Bytes */
#define ATT_BOARD 			0x0008 /* 16Bytes */

#define ATT_MCU_32BYTE 		0x0010 /* 32Bytes */


#define EXTRACT_FILE_SIZE 		1024




#define SHIFT_QUALITY_DATA_SORTING 	1 /* 2022-11-13 - Shift Quality Data Sorting Program */
#define QUAL_DATA_MAX_SIZE 	 		(1024)
#define QUAL_DATA_ITEM_NUM 			17



#endif /* __DEFS_H__ */



