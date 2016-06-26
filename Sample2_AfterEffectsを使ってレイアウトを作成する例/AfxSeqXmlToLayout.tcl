#//	
#//	AfxSeqXmlToLayout.tcl - AfterEffects⇒レイアウト定義 変換スクリプト
#//	Copyright (C) 2016 Naoyuki Sawa
#//	
#//	* Wed Jun 22 23:18:16 JST 2016 Naoyuki Sawa
#//	- 1st リリース。
#//	
set VERSION "20160622"	;#//最終更新日
#//----------------------------------------------------------------------------
#//□参照資料
#//│「tDOM manual: Table of Contents」(https://tdom.github.io/doc-index.html)
package require tdom
#//----------------------------------------------------------------------------
proc writeIndent {indent str} {
	append s [string repeat "\t" $indent] $str
	puts $s
}
#//----------------------------------------------------------------------------
proc processComp {comp indent} {
#//不要	#//コンポジション名を取得する。
#//不要	set compName [$comp getAttribute "name"]
#//不要	if {$compName eq ""} then {DIE}
	#//この<comp>直下の、全ての<layers>ノードについて…			実際には一つしか無いはず。
	foreach layers [$comp selectNodes "./layers"] {
		#//この<layers>直下の、全ての<layer>ノードについて…
		foreach layer [$layers selectNodes "./layer"] {
			#//再帰処理を行う。
			processLayer $layer $indent
		}
	}
}
#//----------------------------------------------------------------------------
proc processLayer {layer indent} {
	#//レイヤ名を取得する。
	set layerName [$layer getAttribute "name"]
	if {$layerName eq ""} then {DIE}
	#//キー名を出力する。
	set layerName [string toupper $layerName]	;#//キー名は大文字に統一する事にしました。必須ではありません。レイヤ名の大文字小文字をそのまま維持したい場合は、この行を削除して下さい。
	writeIndent $indent "$layerName:"
	#//インデントを増やす。
	incr indent
	#//この<layer>直下の、全ての<frames>ノードについて…	実際は一つしか無いはず。
	foreach frames [$layer selectNodes "./frames"] {
		#//この<frames>直下の、全ての<frame>ノードについて…
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
			#//最初のフレームを取得したら、後は不要なので抜ける。
			break
		}
	}
	#//フレームデータが取得出来たことを確認する。
	if {[llength $anchorPoint] != 2 ||
	    [llength $position   ] != 2 ||
	    [llength $topLeft    ] != 2 ||
	    [llength $bottomRight] != 2} {DIE}
	#//フレームデータを出力する。
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
		#//画像ファイル名が有れば、画像ファイル名も出力しておく。	"w","h"キーを出力済みなので、"img"キーは使用されないが、一応出力しておく事にした。
		set fileName [$fileName text]
		writeIndent $indent "img:\t$fileName"
	}
	#//この<layer>直下の、全ての<comp>ノードについて…	実際は0個,又は,1個であるはず。
	foreach comp [$layer selectNodes "./comp"] {
		#//再帰処理を行う。
		processComp $comp $indent
	}
	#//インデントを減らす。
	incr indent -1
}
#//----------------------------------------------------------------------------
proc processXml {fname} {
	#//XMLファイルを開く。
	set fd [open $fname]
	#//XMLドキュメントを読み込む。
	set dom [dom parse -channel $fd]
	#//XMLファイルを閉じる。
	close $fd
	#//ルートノードを取得する。
	set doc [$dom documentElement]
	#//ルートノードが<AfxSeq>である事を確認する。
	if {[$doc nodeName] ne "AfxSeq"} then {DIE}
	#//ルートノードの<AfxSeq>直下の、全ての<comp>ノードについて…
	foreach comp [$doc selectNodes "./comp"] {
		#//再帰処理を行う。
		processComp $comp 0
	}
	#//XMLドキュメントを解放する。
	$dom delete
}
#//----------------------------------------------------------------------------
proc usage {} {
	global VERSION
	puts stderr "AfxSeqXmlToLayout.tcl - AfterEffects⇒レイアウト定義 変換スクリプト ($VERSION)"
	puts stderr {Copyright (C) 2016 Naoyuki Sawa}
	puts stderr {}
	puts stderr {USAGE:}
	puts stderr {  tclsh.exe AfxSeqXmlToLayout.tcl XMLファイル名 [XMLファイル名 ...]}
	puts stderr {}
	puts stderr {EXAMPLE:}
	puts stderr {  tclsh.exe AfxSeqXmlToLayout.tcl AfxSeq1.xml AfxSeq2.xml > Layout.yml}
	puts stderr {  AfxSeq1.xmlとAfxSeq2.xmlを読み込んで、レイアウト定義をLayout.ymlへ出力します。}
	puts stderr {}
	puts stderr {NOTE:}
	puts stderr {  AfxSeq1.xml等は、/clip/tool/MkAfxSeqXml1.jsx,又は,MkAfxSeqXml2.jsxで生成して下さい。}
	puts stderr {  Layout.ymlは、dplayout.exeに読み込ませて、C言語ヘッダファイルに変換して下さい。}
	puts stderr {  動作確認したTclのバージョン: ActiveState ActiveTcl 8.6.4.1.299124 Jun 06, 2015 (x86)}
	exit 1
}
#//----------------------------------------------------------------------------
if {$argv eq ""} then {usage}
foreach xmlFname $argv {
	processXml $xmlFname
}
