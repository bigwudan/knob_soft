
/**
 * @file   compiler.h
 * @brief  识别当前编译器环境
 * 在编译选项中以全局头文件包含的形式将专门存放用户配置的头文件引用进来，比如：
 * 在 gcc、clang 和 arm compiler 6 中使用 -include "头文件" 的方式:
 * -include "compiler.h"
 * 在 arm compiler 5 中使用 --preinclude=头文件 的方式:
 * --preinclude="compiler.h"
 */
#ifndef __COMPILER_H__
#define __COMPILER_H__

//! \note for IAR
#undef __IS_COMPILER_IAR__
#if defined(__IAR_SYSTEMS_ICC__)
#   define __IS_COMPILER_IAR__                  1
#endif

//! \note for arm compiler 5
#undef __IS_COMPILER_ARM_COMPILER_5__
#if defined ( __CC_ARM ) || ((__ARMCC_VERSION >= 5000000) && (__ARMCC_VERSION < 6000000))
#   define __IS_COMPILER_ARM_COMPILER_5__       1
#endif
//! @}

//! \note for arm compiler 6
#undef __IS_COMPILER_ARM_COMPILER_6__
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#   define __IS_COMPILER_ARM_COMPILER_6__       1
#endif

#undef __IS_COMPILER_ARM_COMPILER__
#if defined(__IS_COMPILER_ARM_COMPILER_5__) && __IS_COMPILER_ARM_COMPILER_5__   \
||  defined(__IS_COMPILER_ARM_COMPILER_6__) && __IS_COMPILER_ARM_COMPILER_6__

#   define __IS_COMPILER_ARM_COMPILER__         1

#endif

#undef  __IS_COMPILER_LLVM__
#if defined(__clang__) && !__IS_COMPILER_ARM_COMPILER_6__
#   define __IS_COMPILER_LLVM__                 1
#else
//! \note for gcc
#   undef __IS_COMPILER_GCC__
#   if defined(__GNUC__) && !(  defined(__IS_COMPILER_ARM_COMPILER__)           \
                            ||  defined(__IS_COMPILER_LLVM__)                   \
                            ||  defined(__IS_COMPILER_IAR__))
#       define __IS_COMPILER_GCC__              1
#   endif
//! @}
#endif

#endif //__COMPILER_H__
