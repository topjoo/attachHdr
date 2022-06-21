
/************************************************************************************
 * @file  : attachHdr.h
 * @brief : 
 *  - 2008/12/05 : Attached Header Created.
 *
 *
 * How to build :
 *     1) Install cygwin
 *     2) command as below:
 *        gcc -o AttachHdr -mno-cygwin AttachHdr.c  or
 *        gcc-3  -o AttachHdr -mno-cygwin AttachHdr.c 
 *
 * How to profiling
 *     1) gcc -pg -g -o AttachHdr -mno-cygwin AttachHdr.c 
 *     2) AttachHdr  -> running and then create file, gmon.out.
 *     3) gprof -p -b AttachHdr.exe
 *     4) result is as below:
 *         Flat profile:
 *
 * @version : 2.0
 * @date	: 2021/10/07
 * @author	: Yoon-Hwan Joo (tp.joo@daum.net or toppy_joo@naver.com) 
 *
 ************************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#if 0
#define SNDERR(...) snd_lib_error(__FILE__, __LINE__, __FUNCTION__, 0, __VA_ARGS__) /**< Shows a sound error message. */
#define SNDERR(args...) snd_lib_error(__FILE__, __LINE__, __FUNCTION__, 0, ##args) /**< Shows a sound error message. */
#endif



char Attversion[] = "2.23"; /* ver 2.21 : 2021.06.25 : Version up */
char EmailText[]  = "tp.joo@daum.net";


enum {
	MOT_HEX_S0 = 0x0001,
	MOT_HEX_S1 = 0x0002,
	MOT_HEX_S2 = 0x0004,
	MOT_HEX_S3 = 0x0008,
	MOT_HEX_S4 = 0x0010,
	MOT_HEX_S5 = 0x0020,
	MOT_HEX_S6 = 0x0040,
	MOT_HEX_S7 = 0x0080,
	MOT_HEX_S8 = 0x0100,
	MOT_HEX_S9 = 0x0200
};


#define HDR_CRC16 				0x0015
#define HDR_KSC_CRC16 			0x0016
#define HDR_CRC16CCITT 			0x0017
#define HDR_ADLER32 			0x0018 /// zlib
#define HDR_CRC32 				0x0019
#define HDR_CRC64 				0x001a
#define HDR_CRC64_ISC 			0x001b
#define HDR_JOAAT 				0x001c // 2020.07.22

#define HDR_SHA1 				0x0020 // 2020.06.10
#define HDR_SHA224 				0x0021
#define HDR_SHA256 				0x0022
#define HDR_SHA384 				0x0023
#define HDR_SHA512 				0x0024
#define HDR_SHA3_224 			0x0025
#define HDR_SHA3_256 			0x0026
#define HDR_SHA3_384 			0x0027
#define HDR_SHA3_512 			0x0028
#define HDR_SHAKE128 			0x0029
#define HDR_SHAKE256 			0x002a

#define HDR_MD2 				0x0030
#define HDR_MD4 				0x0031
#define HDR_MD5 				0x0032
#define HDR_MD6 				0x0033

#define HDR_BLAKE224 			0x0035
#define HDR_BLAKE256 			0x0036
#define HDR_BLAKE384 			0x0037
#define HDR_BLAKE512 			0x0038


#define HDR_RMD128 				0x0040
#define HDR_RMD160 				0x0041
#define HDR_RMD320 				0x0042

#define HDR_CRC_UNKNOWN 			0xffff



#define LOG_ERR(format, ...) 		fprintf(stderr, format "\n", ## __VA_ARGS__)
#define LOG_INF(format, ...)  		fprintf(stdout, format "\n", ## __VA_ARGS__)
#define LOG_V(format, ...) 			fprintf(stderr, format, ## __VA_ARGS__)


#define COMMA_BUF_SIZE  			256 /// 2014.07.23





/// --float / double to hex --------------------------
typedef union {
	float		   flt_val;		
	unsigned char  flt_bin[sizeof(float)]; 
} __float_hex_union; /// = { 0.866f };	 

typedef union { 	  
	double	       dbl_val;	   
	unsigned char  dbl_bin[sizeof(double)];    
} __double_hex_union;
/// --float / double to hex --------------------------



