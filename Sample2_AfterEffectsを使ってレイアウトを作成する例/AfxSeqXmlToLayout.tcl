#//	
#//	AfxSeqXmlToLayout.tcl - AfterEffects�˃��C�A�E�g��` �ϊ��X�N���v�g
#//	Copyright (C) 2016 Naoyuki Sawa
#//	
#//	* Wed Jun 22 23:18:16 JST 2016 Naoyuki Sawa
#//	- 1st �����[�X�B
#//	
set VERSION "20160622"	;#//�ŏI�X�V��
#//----------------------------------------------------------------------------
#//���Q�Ǝ���
#//���utDOM manual: Table of Contents�v(https://tdom.github.io/doc-index.html)
package require tdom
#//----------------------------------------------------------------------------
proc writeIndent {indent str} {
	append s [string repeat "\t" $indent] $str
	puts $s
}
#//----------------------------------------------------------------------------
proc processComp {comp indent} {
#//�s�v	#//�R���|�W�V���������擾����B
#//�s�v	set compName [$comp getAttribute "name"]
#//�s�v	if {$compName eq ""} then {DIE}
	#//����<comp>�����́A�S�Ă�<layers>�m�[�h�ɂ��āc			���ۂɂ͈���������͂��B
	foreach layers [$comp selectNodes "./layers"] {
		#//����<layers>�����́A�S�Ă�<layer>�m�[�h�ɂ��āc
		foreach layer [$layers selectNodes "./layer"] {
			#//�ċA�������s���B
			processLayer $layer $indent
		}
	}
}
#//----------------------------------------------------------------------------
proc processLayer {layer indent} {
	#//���C�������擾����B
	set layerName [$layer getAttribute "name"]
	if {$layerName eq ""} then {DIE}
	#//�L�[�����o�͂���B
	set layerName [string toupper $layerName]	;#//�L�[���͑啶���ɓ��ꂷ�鎖�ɂ��܂����B�K�{�ł͂���܂���B���C�����̑啶�������������̂܂܈ێ��������ꍇ�́A���̍s���폜���ĉ������B
	writeIndent $indent "$layerName:"
	#//�C���f���g�𑝂₷�B
	incr indent
	#//����<layer>�����́A�S�Ă�<frames>�m�[�h�ɂ��āc	���ۂ͈���������͂��B
	foreach frames [$layer selectNodes "./frames"] {
		#//����<frames>�����́A�S�Ă�<frame>�m�[�h�ɂ��āc
		foreach frame [$frames selectNodes "./frame"] {
			#//anchorPoint
			set anchorPoint [$frame selectNodes "./anchorPoint"]		;#//(node)
			set anchorPoint [$anchorPoint text]				;#//"123,456"
			set anchorPoint [split $anchorPoint ","]			;#//[123,456]
			#//position
			set position [$frame selectNodes "./position"]			;#//(node)
			set position [$position text]					;#//"123,456"
			set position [split $position ","]				;#//[123,456]
			#//cornerPin.topLeft
			set topLeft [$frame selectNodes "./cornerPin/topLeft"]		;#//(node)
			set topLeft [$topLeft text]					;#//"123,456"
			set topLeft [split $topLeft ","]				;#//[123,456]
			#//cornerPin.bottomRight
			set bottomRight [$frame selectNodes "./cornerPin/bottomRight"]	;#//(node)
			set bottomRight [$bottomRight text]				;#//"123,456"
			set bottomRight [split $bottomRight ","]			;#//[123,456]
			#//�ŏ��̃t���[�����擾������A��͕s�v�Ȃ̂Ŕ�����B
			break
		}
	}
	#//�t���[���f�[�^���擾�o�������Ƃ��m�F����B
	if {[llength $anchorPoint] != 2 ||
	    [llength $position   ] != 2 ||
	    [llength $topLeft    ] != 2 ||
	    [llength $bottomRight] != 2} {DIE}
	#//�t���[���f�[�^���o�͂���B
	set x [expr [lindex $position 0] - [lindex $anchorPoint 0]]
	set y [expr [lindex $position 1] - [lindex $anchorPoint 1]]
	set w [expr [lindex $bottomRight 0] - [lindex $topLeft 0]]
	set h [expr [lindex $bottomRight 1] - [lindex $topLeft 1]]
	writeIndent $indent "x:\t$x"
	writeIndent $indent "y:\t$y"
	writeIndent $indent "w:\t$w"
	writeIndent $indent "h:\t$h"
	#//footage.fileName
	set fileName [$layer selectNodes "./footage/fileName"]
	if {$fileName ne ""} then {
		#//�摜�t�@�C�������L��΁A�摜�t�@�C�������o�͂��Ă����B	"w","h"�L�[���o�͍ς݂Ȃ̂ŁA"img"�L�[�͎g�p����Ȃ����A�ꉞ�o�͂��Ă������ɂ����B
		set fileName [$fileName text]
		writeIndent $indent "img:\t$fileName"
	}
	#//����<layer>�����́A�S�Ă�<comp>�m�[�h�ɂ��āc	���ۂ�0��,����,1�ł���͂��B
	foreach comp [$layer selectNodes "./comp"] {
		#//�ċA�������s���B
		processComp $comp $indent
	}
	#//�C���f���g�����炷�B
	incr indent -1
}
#//----------------------------------------------------------------------------
proc processXml {fname} {
	#//XML�t�@�C�����J���B
	set fd [open $fname]
	#//XML�h�L�������g��ǂݍ��ށB
	set dom [dom parse -channel $fd]
	#//XML�t�@�C�������B
	close $fd
	#//���[�g�m�[�h���擾����B
	set doc [$dom documentElement]
	#//���[�g�m�[�h��<AfxSeq>�ł��鎖���m�F����B
	if {[$doc nodeName] ne "AfxSeq"} then {DIE}
	#//���[�g�m�[�h��<AfxSeq>�����́A�S�Ă�<comp>�m�[�h�ɂ��āc
	foreach comp [$doc selectNodes "./comp"] {
		#//�ċA�������s���B
		processComp $comp 0
	}
	#//XML�h�L�������g���������B
	$dom delete
}
#//----------------------------------------------------------------------------
proc usage {} {
	global VERSION
	puts stderr "AfxSeqXmlToLayout.tcl - AfterEffects�˃��C�A�E�g��` �ϊ��X�N���v�g ($VERSION)"
	puts stderr {Copyright (C) 2016 Naoyuki Sawa}
	puts stderr {}
	puts stderr {USAGE:}
	puts stderr {  tclsh.exe AfxSeqXmlToLayout.tcl XML�t�@�C���� [XML�t�@�C���� ...]}
	puts stderr {}
	puts stderr {EXAMPLE:}
	puts stderr {  tclsh.exe AfxSeqXmlToLayout.tcl AfxSeq1.xml AfxSeq2.xml > Layout.yml}
	puts stderr {  AfxSeq1.xml��AfxSeq2.xml��ǂݍ���ŁA���C�A�E�g��`��Layout.yml�֏o�͂��܂��B}
	puts stderr {}
	puts stderr {NOTE:}
	puts stderr {  AfxSeq1.xml���́A/clip/tool/MkAfxSeqXml1.jsx,����,MkAfxSeqXml2.jsx�Ő������ĉ������B}
	puts stderr {  Layout.yml�́Adplayout.exe�ɓǂݍ��܂��āAC����w�b�_�t�@�C���ɕϊ����ĉ������B}
	puts stderr {  ����m�F����Tcl�̃o�[�W����: ActiveState ActiveTcl 8.6.4.1.299124 Jun 06, 2015 (x86)}
	exit 1
}
#//----------------------------------------------------------------------------
if {$argv eq ""} then {usage}
foreach xmlFname $argv {
	processXml $xmlFname
}
