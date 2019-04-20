PREFIX = /home/sharek
all: tools
tools: brightness
brightness:
	gcc brightness.c -o brightness
install:
	@cp -f brightness ${DESTDIR}${PREFIX}/bin
	@echo "Note: root is required to chmod/chown suid binary!"
	sudo chown root ${DESTDIR}${PREFIX}/bin/brightness
	sudo chmod 755 ${DESTDIR}${PREFIX}/bin/brightness
	sudo chmod u+s ${DESTDIR}${PREFIX}/bin/brightness
	
clean:
	@rm brightness || true
.PHONY: all clean install tools brightness