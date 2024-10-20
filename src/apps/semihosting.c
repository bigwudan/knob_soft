/**
 *******************************************************************************************************************************************
 * @file        semihosting.c
 * @brief       半主机环境配置
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 * 2024-08-18   lzh          add _sys_xx()
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <rt_sys.h>
#include <rt_misc.h>
#include <time.h>

#include "compiler.h"

#include "SWM221.h"

/** 重定向输出调试日志的硬件串口号 */
#define DEBUG_UART_X         UART1

int fputc(int ch, FILE *f)
{
    while (UART_IsTXFIFOFull(DEBUG_UART_X)) __NOP();
    UART_WriteByte(DEBUG_UART_X, ch);
    while (UART_IsTXBusy(DEBUG_UART_X)) __NOP();
    return ch;
}

void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode)
{
    for (;;) __NOP();
}

#ifndef __MICROLIB
struct __FILE  { 
    int handle; 
    /* Whatever you require here. If the only file you are using is */ 
    /* standard output using printf() for debugging, no file handling */ 
    /* is required. */ 
};

/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;

/**
 * @brief   不使用 MicroLIB 时关闭 Semihosting(default: Semihosting is open)
 * @note    AC5: #pragma import(__use_no_semihosting)
 *          AC6: __asm(".global __use_no_semihosting\n\t");
 */
#if __IS_COMPILER_ARM_COMPILER_6__
    __asm(".global __use_no_semihosting\n\t");
    __asm(".global __ARM_use_no_argv\n\t"); /* compiled with Arm Compiler 6 at -O0 optimization level */
#elif __IS_COMPILER_ARM_COMPILER_5__
    #pragma import(__use_no_semihosting)
#endif

const char __stdin_name[] =  ":tt";
const char __stdout_name[] =  ":tt";
const char __stderr_name[] =  ":tt";

FILEHANDLE _sys_open(const char *name, int openmode){
    return 1;
}

int _sys_close(FILEHANDLE fh){
    return 0;
}

/* If you define __use_no_semihosting without __ARM_use_no_argv,
 * then the library code to handle argc and argv requires you to retarget these functions:
 * void _ttywrch(int ch);
 * void _sys_exit(int return_code);
 * char *_sys_command_string(char *cmd, int len);
 */
char *_sys_command_string(char *cmd, int len){
    return NULL;
}

int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode){
    return 0;
}

int _sys_read(FILEHANDLE fh, unsigned char *buf, unsigned len, int mode){
    return -1;
}

void _ttywrch(int ch){
/* __use_no_semihosting was requested, but _ttywrch was */
}

int _sys_istty(FILEHANDLE fh){
    return 0;
}

int _sys_seek(FILEHANDLE fh, long pos){
    return -1;
}

long _sys_flen(FILEHANDLE fh){
    return -1;
}

void _sys_exit(int return_code) {
    return_code = return_code;
}

int system(const char *string){
    return 0;
}

char *getenv(const char *name){
    return NULL;
}

void _getenv_init(void){
}

#endif // !defined(__MICROLIB)
