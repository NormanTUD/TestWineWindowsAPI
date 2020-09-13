CC = i686-w64-mingw32-g++ 
CFLAGS  = -g -Wall --debug -static-libgcc -lopengl32 -lgdi32 -lkernel32 -luser32 -lcomdlg32

build:
	@echo Creating targets
	test -s "$(TARGET).modified.c" || perl modify.pl "$(TARGET)"
	test -s $(TARGET).exe || $(CC) $(CFLAGS) -o "$(TARGET).exe" "$(TARGET).modified.c"

clean: 
	@echo Cleaning targets
	$(RM) *.exe

run:	build
	@echo Running wine
	wine $(TARGET).exe
	@EXIT_CODE=$$?
	@echo "command exited with $$EXIT_CODE"
	@exit $$EXIT_CODE
