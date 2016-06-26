#ifndef __APP_H__
#define __APP_H__
#define USE_GDIPLUS	//GDI+を使用しない場合はこの行をコメントアウトして下さい。
//-----------------------------------------------------------------------------
#include <clip.h>
//-----------------------------------------------------------------------------
#ifndef USE_PDCURSES
#undef  getch		//PDCursesを使用しない場合は、pdcurses/curses.hで定義された左記のマクロを取り消して、元のDOS関数を呼び出せるようにする。
#undef  ungetch		//PDCursesを使用しない場合は、pdcurses/curses.hで定義された左記のマクロを取り消して、元のDOS関数を呼び出せるようにする。
#endif//USE_PDCURSES
#ifdef  __cplusplus
#if     (defined(USE_GDIPLUS) && !defined(USE_ACTIVETCL))
using namespace Gdiplus;	//GDI+を使用し,かつ,ActiveTclを使用しない場合は、'Font'の定義が衝突しないので、Gdiplus名前空間を導入出来る。必須ではありません。もし何か他の名前が衝突した場合は、この行をコメントアウトして下さい。
#endif//(defined(USE_GDIPLUS) && !defined(USE_ACTIVETCL))
#endif//__cplusplus
#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus
//--- app.cpp ---
void app_init(int argc, char* argv[]);
int  app_main(int argc, char* argv[]);
void app_exit();
//{{アプリケーション特有の関数
//--- ???.c ---
//void foo(int x, int y);
#ifdef  __cplusplus
//--- ???.cpp ---
//void bar(int x, int y = 0);
#endif//__cplusplus
//}}アプリケーション特有の関数
#ifdef  __cplusplus
}//extern "C"
#endif//__cplusplus
#include "work.h"
#endif//__APP_H__
