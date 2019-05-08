SRC = ..\src\*.cpp 
INCLUDES = ..\src\*.h 
LIBS = ..\src\*.lib 
INCLUDEPATH = C:\bc31\include;..\src;
LIBSPATH = C:\bc31\lib;..\src;
TARGET = Kernel.exe
$(TARGET):	$(SRC) $(INCLUDES) configuration.xmi
	bcc.exe -mh -v -e$(TARGET) -I$(INCLUDEPATH) -L$(LIBSPATH) $(SRC) $(LIBS)
all:	$(TARGET)
clean:
	-del *.obj
	-del $(TARGET)
	-del *.asm
configuration.xmi:	
	configuration.xmi