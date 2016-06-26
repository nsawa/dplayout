@ECHO OFF
SETLOCAL
SET BATDIR=%~dp0

REM *** Visual Studio ***
IF EXIST	Debug			RMDIR /S /Q	Debug
IF EXIST	Release			RMDIR /S /Q	Release
IF EXIST	*.ncb			DEL		*.ncb
IF EXIST	*.opt			DEL		*.opt
IF EXIST	*.plg			DEL		*.plg
IF EXIST	*.aps			DEL		*.aps

REM *** Bison/Flex ***
IF EXIST	y.tab.c			DEL		y.tab.c
IF EXIST	y.tab.h			DEL		y.tab.h
IF EXIST	y.output		DEL		y.output
IF EXIST	lex.yy.c		DEL		lex.yy.c

REM *** �񋓒�` ***
IF EXIST	EnmDef.h		DEL		EnmDef.h
IF EXIST	RegEnmDef.txt		DEL		RegEnmDef.txt

REM *** �v���p�e�B�e�[�u�� ***
IF EXIST	PrpTbl.h		DEL		PrpTbl.h
IF EXIST	RegPrpTbl.txt		DEL		RegPrpTbl.txt

REM *** �L����ԋ@�B ***
IF EXIST	FsmMap.c		DEL		FsmMap.c
IF EXIST	FsmMap.h		DEL		FsmMap.h
IF EXIST	RegFsmMap.txt		DEL		RegFsmMap.txt
IF EXIST	EnmFsmMap.txt		DEL		EnmFsmMap.txt
IF EXIST	PrpFsmMap.txt		DEL		PrpFsmMap.txt

REM *** ���W�X�g���e�[�u�� ***
IF EXIST	RegTbl.h		DEL		RegTbl.h
IF EXIST	RegTbl.bin		DEL		RegTbl.bin

REM *** TAP�V�[�P���T ***
IF EXIST	TapDef.h		DEL		TapDef.h
IF EXIST	TapDef.bin		DEL		TapDef.bin

REM *** �A�j���[�V�����V�[�P���T ***
IF EXIST	AnmSeq.h		DEL		AnmSeq.h
IF EXIST	AnmSeq.bin		DEL		AnmSeq.bin

REM *** NCS��ԃV�[�P���T ***
IF EXIST	NcsSeq.h		DEL		NcsSeq.h
IF EXIST	NcsSeq.bin		DEL		NcsSeq.bin

REM *** ���I�e�[�u�� ***
IF EXIST	LotTbl.h		DEL		LotTbl.h
IF EXIST	LotTbl.bin		DEL		LotTbl.bin

REM *** DxLib ***
IF EXIST	Log.txt			DEL		Log.txt

REM *** ���̑� ***
IF EXIST	Const.h			DEL		Const.h
IF EXIST	syslog.out		DEL		syslog.out