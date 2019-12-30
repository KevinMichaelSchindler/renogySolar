
bin_PROGRAMS=
noinst_LIBRARIES=

wwwdir=$(datadir)/renogySolar
www_DATA=
www_DATA+=www/index.html
www_DATA+=www/subframe.html
www_DATA+=www/style.css
www_DATA+=www/dark_style.css

noinst_LIBRARIES+=libRenogySolarCommon.a
libRenogySolarCommon_a_SOURCES=
libRenogySolarCommon_a_SOURCES+=src/Args.cpp
libRenogySolarCommon_a_SOURCES+=src/Common.cpp
libRenogySolarCommon_a_SOURCES+=src/ModBus.cpp
libRenogySolarCommon_a_SOURCES+=src/XML2JSON.cpp
libRenogySolarCommon_a_SOURCES+=src/XMLParser.cpp

bin_PROGRAMS+=rs485-tcp
rs485_tcp_SOURCES=
rs485_tcp_SOURCES+=src/rs485-tcp.cpp
rs485_tcp_SOURCES+=src/SerialPort.cpp
rs485_tcp_LDADD=libRenogySolarCommon.a

bin_PROGRAMS+=controller_read
controller_read_SOURCES=
controller_read_SOURCES+=src/controller_read.cpp
controller_read_LDADD=libRenogySolarCommon.a

bin_PROGRAMS+=history
history_SOURCES=
history_SOURCES+=src/history.cpp
history_SOURCES+=src/HistoryTable.cpp
history_LDFLAGS=-lsqlite3
history_LDADD=libRenogySolarCommon.a

bin_PROGRAMS+=webserver
webserver_CPPFLAGS="-DWWW_DATA=\"$(wwwdir)\""
webserver_SOURCES=src/webserver.cpp
webserver_LDADD=libRenogySolarCommon.a
