/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 1995 - 2018 SEGGER Microcontroller GmbH             *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* Redistribution and use in source and binary forms, with or        *
* without modification, are permitted provided that the following   *
* conditions are met:                                                *
*                                                                    *
* - Redistributions of source code must retain the above copyright  *
*   notice, this list of conditions and the following disclaimer.   *
*                                                                    *
* - Neither the name of SEGGER Microcontroller GmbH                 *
*   nor the names of its contributors may be used to endorse or     *
*   promote products derived from this software without specific    *
*   prior written permission.                                       *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          *
* DISCLAIMED.                                                        *
* IN NO EVENT SHALL SEGGER Microcontroller GmbH BE LIABLE FOR      *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR          *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;   *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF     *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT         *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH  *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************
*                                                                    *
*       RTT version: 6.32d                                           *
*                                                                    *
**********************************************************************

---------------------------END-OF-HEADER------------------------------
File    : SEGGER_RTT_Conf.h
Purpose : Implementation of SEGGER real-time transfer (RTT) which
          allows real-time communication on targets which support
          debugger memory accesses while the CPU is running.
Revision: $Rev: 9599 $

*/

#ifndef SEGGER_RTT_CONF_H
#define SEGGER_RTT_CONF_H

#ifdef __IAR_SYSTEMS_ICC__
  #include <intrinsics.h>
#endif

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

//
// Take in and set to correct values for Cortex-A systems with CPU cache
//
//#define SEGGER_RTT_CPU_CACHE_LINE_SIZE            (32)          // Largest cache line size (in bytes) in the target system.
//#define SEGGER_RTT_UNCACHED_OFF                   (0xFB000000)  // Address alias where RTT CB and buffers can be accessed uncached
//
// Most common case:
// Up-channel 0: RTT
// Up-channel 1: RTT Terminal 1
// Up-channel 2: SysView
// Up-channel 3: J-Scope
//
#ifndef   SEGGER_RTT_MAX_NUM_UP_BUFFERS
  #define SEGGER_RTT_MAX_NUM_UP_BUFFERS             (3)     // Max. number of up-buffers (T->H) available on this target    (Default: 3)
#endif
//
// Most common case:
// Down-channel 0: RTT
// Down-channel 1: RTT Terminal 1
// Down-channel 2: SysView
//
#ifndef   SEGGER_RTT_MAX_NUM_DOWN_BUFFERS
  #define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS           (3)     // Max. number of down-buffers (H->T) available on this target  (Default: 3)
#endif

#ifndef   BUFFER_SIZE_UP
  #define BUFFER_SIZE_UP                            (1024)  // Size of the buffer for terminal output of target, up to host (Default: 1k)
#endif

#ifndef   BUFFER_SIZE_DOWN
  #define BUFFER_SIZE_DOWN                          (16)    // Size of the buffer for terminal input to target from host (Usually keyboard input) (Default: 16)
#endif

#ifndef   SEGGER_RTT_PRINTF_BUFFER_SIZE
  #define SEGGER_RTT_PRINTF_BUFFER_SIZE             (64u)    // Size of buffer for RTT printf to bulk-send chars via RTT     (Default: 64)
#endif

#ifndef   SEGGER_RTT_MODE_DEFAULT
  #define SEGGER_RTT_MODE_DEFAULT                   SEGGER_RTT_MODE_NO_BLOCK_SKIP // Mode for pre-initialized terminal channel (buffer 0)
#endif

/*********************************************************************
*
*       RTT memcpy configuration
*
*       memcpy() is good for large amounts of data, 
*       but the overhead is too big for small amounts.
*       SEGGER_RTT_MEMCPY_USE_BYTELOOP configures
*       when to use memcpy() or byte-loop for memcpy() functionality.
*
*       SEGGER_RTT_MEMCPY() can be used to replace standard memcpy()
*       in RTT functions.
*
*       Example definition of SEGGER_RTT_MEMCPY to external memcpy with GCC:
*       #define SEGGER_RTT_MEMCPY(pDest, pSrc, NumBytes)      __builtin_memcpy((pDest), (pSrc), (NumBytes))
*
**********************************************************************
*/
#ifndef   SEGGER_RTT_MEMCPY_USE_BYTELOOP
  #define SEGGER_RTT_MEMCPY_USE_BYTELOOP              (16)  // Number of bytes to be copied, before RTT uses a possibly faster memcpy. 
#endif

#ifndef   SEGGER_RTT_MEMCPY
  #define SEGGER_RTT_MEMCPY(pDest, pSrc, NumBytes)   SEGGER_RTT_MEMCPY_FUNC((pDest), (pSrc), (NumBytes))
#endif

/*********************************************************************
*
*       RTT lock configuration for SEGGER Embedded Studio,
*       Rowley CrossStudio and GCC
*
**********************************************************************
*/
#if (defined __SES_ARM) || (defined __CROSSWORKS_ARM) || (defined __GNUC__) || (defined __clang__)
  #if (defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_8M_BASE__))
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                    unsigned int _SEGGER_RTT__LockState;                           \
                                  __asm volatile ("mrs   %0, primask  \n\t"                        \
                                                  "movs  r1, #1        \n\t"                       \
                                                  "msr   primask, r1   \n\t"                       \
                                                  : "=r" (_SEGGER_RTT__LockState)                   \
                                                  :                                                 \
                                                  : "r1", "cc"                                      \
                                                  );

    #define SEGGER_RTT_UNLOCK() __asm volatile ("msr   primask, %0  \n\t"                          \
                                                  :                                                 \
                                                  : "r" (_SEGGER_RTT__LockState)                    \
                                                  :                                                 \
                                                  );                                                \
                                }

  #elif (defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_MAIN__))
    #ifndef   SEGGER_RTT_MAX_INTERRUPT_PRIORITY
      #define SEGGER_RTT_MAX_INTERRUPT_PRIORITY         (0x20)   // Interrupt priority to lock on SEGGER_RTT_LOCK on Cortex-M3/M4/M7/M33 (Default: 0x20)
    #endif
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                    unsigned int _SEGGER_RTT__LockState;                           \
                                  __asm volatile ("mrs   %0, basepri  \n\t"                        \
                                                  "mov   r1, %1       \n\t"                        \
                                                  "msr   basepri, r1   \n\t"                       \
                                                  : "=r" (_SEGGER_RTT__LockState)                   \
                                                  : "i"(SEGGER_RTT_MAX_INTERRUPT_PRIORITY)          \
                                                  : "r1", "cc"                                      \
                                                  );

    #define SEGGER_RTT_UNLOCK() __asm volatile ("msr   basepri, %0  \n\t"                          \
                                                  :                                                 \
                                                  : "r" (_SEGGER_RTT__LockState)                    \
                                                  :                                                 \
                                                  );                                                \
                                }

  #elif defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_5TE__)
    //
    // Lock RTT (nestable)   (i.e. disable interrupts)
    //
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int _SEGGER_RTT__LockState;                             \
                                  __asm volatile ("mrs r1, cpsr \n\t"                              \
                                                  "mov %0, r1  \n\t"                               \
                                                  "orr r1, r1, #0xC0 \n\t"                         \
                                                  "msr cpsr, r1 \n\t"                              \
                                                  : "=r" (_SEGGER_RTT__LockState)                   \
                                                  :                                                 \
                                                  : "r1", "cc"                                      \
                                                  );

    #define SEGGER_RTT_UNLOCK() __asm volatile ("msr cpsr, %0 \n\t"                                \
                                                  :                                                 \
                                                  : "r" (_SEGGER_RTT__LockState)                    \
                                                  :                                                 \
                                                  );                                                \
                                }
  #else
    #define SEGGER_RTT_LOCK()
    #define SEGGER_RTT_UNLOCK()
  #endif
#endif

/*********************************************************************
*
*       RTT lock configuration for IAR EWARM
*
**********************************************************************
*/
#ifdef __ICCARM__
  #if (defined (__ARM6M__) && (__CORE__ == __ARM6M__))
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int _SEGGER_RTT__LockState;                             \
                                  _SEGGER_RTT__LockState = __get_PRIMASK();                         \
                                  __set_PRIMASK(1);

    #define SEGGER_RTT_UNLOCK()   __set_PRIMASK(_SEGGER_RTT__LockState);                           \
                                }
  #elif (defined (__ARM7EM__) && (__CORE__ == __ARM7EM__)) || (defined (__ARM7M__) && (__CORE__ == __ARM7M__))
    #ifndef   SEGGER_RTT_MAX_INTERRUPT_PRIORITY
      #define SEGGER_RTT_MAX_INTERRUPT_PRIORITY         (0x20)   // Interrupt priority to lock on SEGGER_RTT_LOCK on Cortex-M3/M4/M7 (Default: 0x20)
    #endif
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int _SEGGER_RTT__LockState;                             \
                                  _SEGGER_RTT__LockState = __get_BASEPRI();                         \
                                  __set_BASEPRI(SEGGER_RTT_MAX_INTERRUPT_PRIORITY);

    #define SEGGER_RTT_UNLOCK()   __set_BASEPRI(_SEGGER_RTT__LockState);                           \
                                }
  #endif
#endif

/*********************************************************************
*
*       RTT lock configuration for KEIL ARM
*
**********************************************************************
*/
#ifdef __CC_ARM
  #if (defined __TARGET_ARCH_6S_M)
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int _SEGGER_RTT__LockState;                             \
                                  register unsigned char _SEGGER_RTT__LockState __asm("primask");  \
                                  _SEGGER_RTT__LockState = __disable_irq();

    #define SEGGER_RTT_UNLOCK() if (!_SEGGER_RTT__LockState) __enable_irq();                       \
                                }
  #elif (defined(__TARGET_ARCH_7_M) || defined(__TARGET_ARCH_7E_M))
    #ifndef   SEGGER_RTT_MAX_INTERRUPT_PRIORITY
      #define SEGGER_RTT_MAX_INTERRUPT_PRIORITY         (0x20)   // Interrupt priority to lock on SEGGER_RTT_LOCK on Cortex-M3/M4/M7 (Default: 0x20)
    #endif
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int _SEGGER_RTT__LockState;                             \
                                  register unsigned char _SEGGER_RTT__LockState __asm("basepri");  \
                                  _SEGGER_RTT__LockState = __disable_irq();                         \
                                  __set_BASEPRI(SEGGER_RTT_MAX_INTERRUPT_PRIORITY);

    #define SEGGER_RTT_UNLOCK() __set_BASEPRI(_SEGGER_RTT__LockState);                             \
                                if (!_SEGGER_RTT__LockState) __enable_irq();                       \
                                }
  #endif
#endif

/*********************************************************************
*
*       RTT lock configuration fallback
*
**********************************************************************
*/
#ifndef   SEGGER_RTT_LOCK
  #define SEGGER_RTT_LOCK()                // Lock RTT (nestable)   (i.e. disable interrupts)
#endif

#ifndef   SEGGER_RTT_UNLOCK
  #define SEGGER_RTT_UNLOCK()              // Unlock RTT (nestable) (i.e. enable previous interrupt lock state)
#endif

#endif
/*************************** End of file ****************************/
