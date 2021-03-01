PROG=./main.out
CXX=g++ -fPIC
CC=gcc -fPIC
CXXFLAGS=-I. -std=c++11 -Wall -g
CFLAGS=-I. -Wall -g -DHAVE_CONFIG_H -DHAVE_ICONV -DUSE_OUR_OWN_MD5
SONAME=./instalextract.so
LINK=-shared -fvisibility=hidden
main.o : ./main.cpp
	$(CXX) $(CXXFLAGS) -c ./main.cpp
unicode_winapi.o : ./unicode_winapi.cpp
	$(CXX) $(CXXFLAGS) -c ./unicode_winapi.cpp
buffer.o : ./buffer.cpp
	$(CXX) $(CXXFLAGS) -c ./buffer.cpp
parse.o : ./parse.cpp
	$(CXX) $(CXXFLAGS) -c ./parse.cpp
ipackermatch.o : ./ipackermatch.cpp
	$(CXX) $(CXXFLAGS) -c ./ipackermatch.cpp
AdvancedInstallerUnpacker.o : ./AdvancedInstallerUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./AdvancedInstallerUnpacker.cpp
C7zip.o : ./C7zip.cpp
	$(CXX) $(CXXFLAGS) -c ./C7zip.cpp
GenteeUnpacker.o : ./GenteeUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./GenteeUnpacker.cpp
MoleBoxUnpacker.o : ./MoleBoxUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./MoleBoxUnpacker.cpp
MSIUnpacker.o : ./MSIUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./MSIUnpacker.cpp
NSISInstallerUpacker.o : ./NSISInstallerUpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./NSISInstallerUpacker.cpp
SetupFactory.o : ./SetupFactory.cpp
	$(CXX) $(CXXFLAGS) -c ./SetupFactory.cpp
SmartInstUnpacker.o : ./SmartInstUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./SmartInstUnpacker.cpp
ViseUnpacker.o : ./ViseUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./ViseUnpacker.cpp
WinRar.o : ./WinRar.cpp
	$(CXX) $(CXXFLAGS) -c ./WinRar.cpp
WiseInstallerUnpacker.o : ./WiseInstallerUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./WiseInstallerUnpacker.cpp
ZipSfxUnpacker.o : ./ZipSfxUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./ZipSfxUnpacker.cpp
C7zOffical.o : ./C7zOffical.cpp
	$(CXX) $(CXXFLAGS) -c ./C7zOffical.cpp
InnoUnpacker.o : ./InnoUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./InnoUnpacker.cpp
WinZip.o : ./WinZip.cpp
	$(CXX) $(CXXFLAGS) -c ./WinZip.cpp
ShieldInstUnpacker.o : ./ShieldInstUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./ShieldInstUnpacker.cpp
NestedPEUnpacker.o : ./NestedPEUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./NestedPEUnpacker.cpp
DotnetInstallerUnpacker.o : ./DotnetInstallerUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./DotnetInstallerUnpacker.cpp
CmdLineParser.o : ./CmdLineParser.cpp
	$(CXX) $(CXXFLAGS) -c ./CmdLineParser.cpp
C7zOfficalUnpacker.o : ./C7zOfficalUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./C7zOfficalUnpacker.cpp
FileAssist.o : ./FileAssist.cpp
	$(CXX) $(CXXFLAGS) -c ./FileAssist.cpp
UnpackManager.o : ./UnpackManager.cpp
	$(CXX) $(CXXFLAGS) -c ./UnpackManager.cpp
BaseUnpacker.o : ./BaseUnpacker.cpp
	$(CXX) $(CXXFLAGS) -c ./BaseUnpacker.cpp
unshield.o : ./unshield.c
	$(CC) $(CFLAGS) -c ./unshield.c
export.o : ./export.cpp
	$(CXX) $(CXXFLAGS) -c ./export.cpp
global.o : ./global.cpp
	$(CXX) $(CXXFLAGS) -c ./global.cpp

OBJS=\
	unicode_winapi.o \
 	buffer.o \
	parse.o \
	ipackermatch.o \
	AdvancedInstallerUnpacker.o \
	C7zip.o \
	GenteeUnpacker.o \
	MoleBoxUnpacker.o \
	MSIUnpacker.o \
	NSISInstallerUpacker.o \
	SetupFactory.o \
	SmartInstUnpacker.o \
	ViseUnpacker.o \
	WinRar.o \
	WiseInstallerUnpacker.o \
	ZipSfxUnpacker.o \
	C7zOffical.o \
	InnoUnpacker.o \
	WinZip.o \
	ShieldInstUnpacker.o \
	NestedPEUnpacker.o \
	CmdLineParser.o \
	FileAssist.o \
	BaseUnpacker.o \
	UnpackManager.o \
	C7zOfficalUnpacker.o \
	DotnetInstallerUnpacker.o \
	unshield.o \
	export.o \
	global.o \
	main.o

all : $(OBJS)
	$(CXX) $(CXXFLAGS) $(LINK) -o $(SONAME) $(OBJS) -static-libstdc++ -l7z -lpthread -ldl -llibunshield -lmd5 -lconvert_utf -lz -L. 
	$(CXX) $(CXXFLAGS) -o main.out  $(OBJS) -static-libstdc++ -l7z -lpthread -ldl -llibunshield -lmd5 -lconvert_utf -lz -L.

clean:
	rm -rf *.o main.out instalextract.so
