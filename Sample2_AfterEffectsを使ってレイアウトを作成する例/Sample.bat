tclsh.exe AfxSeqXmlToLayout.tcl Sample.xml > layout.yml
dplayout.exe Layout.yml | TabAlign.exe > layout.h
DEL layout.yml
