/**
 * @file    wordring/debug.cpp
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
 * @par     �z�[��
 *          https://github.com/wordring/
 * @par     ���C�Z���X
 *          PDS
 */

#include <wordring/debug.h>

#ifdef _WIN32

void wordring::debug::debug_memory_leak()
{
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
}

#endif // _WIN32
