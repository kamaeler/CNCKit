﻿/**
 * @file    wordring/debug.h
 *
 * @brief   
 *
 * @details
 *          
 *          

 *          
 *          
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#ifndef WORDRING_DEBUG_H
#define WORDRING_DEBUG_H

#ifdef _WIN32
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__) 

#include <iostream>

#endif // _DEBUG
#endif // _WIN32

#ifndef WORDRING_DEBUG_CONTROL
//#define WORDRING_DEBUG_CONTROL
#endif // WORDRING_DEBUG_CONTROL

namespace wordring
{
namespace debug
{

void debug_memory_leak();

} // namespace debug
} // namespace wordring

#endif // WORDRING_DEBUG_H