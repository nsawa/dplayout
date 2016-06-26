#ifndef __APP_H__
#define __APP_H__
#define USE_GDIPLUS	//GDI+���g�p���Ȃ��ꍇ�͂��̍s���R�����g�A�E�g���ĉ������B
//-----------------------------------------------------------------------------
#include <clip.h>
//-----------------------------------------------------------------------------
#ifndef USE_PDCURSES
#undef  getch		//PDCurses���g�p���Ȃ��ꍇ�́Apdcurses/curses.h�Œ�`���ꂽ���L�̃}�N�����������āA����DOS�֐����Ăяo����悤�ɂ���B
#undef  ungetch		//PDCurses���g�p���Ȃ��ꍇ�́Apdcurses/curses.h�Œ�`���ꂽ���L�̃}�N�����������āA����DOS�֐����Ăяo����悤�ɂ���B
#endif//USE_PDCURSES
#ifdef  __cplusplus
#if     (defined(USE_GDIPLUS) && !defined(USE_ACTIVETCL))
using namespace Gdiplus;	//GDI+���g�p��,����,ActiveTcl���g�p���Ȃ��ꍇ�́A'Font'�̒�`���Փ˂��Ȃ��̂ŁAGdiplus���O��Ԃ𓱓��o����B�K�{�ł͂���܂���B�����������̖��O���Փ˂����ꍇ�́A���̍s���R�����g�A�E�g���ĉ������B
#endif//(defined(USE_GDIPLUS) && !defined(USE_ACTIVETCL))
#endif//__cplusplus
#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus
//--- app.cpp ---
void app_init(int argc, char* argv[]);
int  app_main(int argc, char* argv[]);
void app_exit();
//{{�A�v���P�[�V�������L�̊֐�
//--- ???.c ---
//void foo(int x, int y);
#ifdef  __cplusplus
//--- ???.cpp ---
//void bar(int x, int y = 0);
#endif//__cplusplus
//}}�A�v���P�[�V�������L�̊֐�
#ifdef  __cplusplus
}//extern "C"
#endif//__cplusplus
#include "work.h"
#endif//__APP_H__
