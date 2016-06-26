@ECHO OFF
pipefail.exe -q dplayout.exe -v -DDISP_X=128,DISP_Y=88 Sample.yml ^
             ^| TabAlign.exe ^
 > layout.h
