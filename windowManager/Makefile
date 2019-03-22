# swim dwm based - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

all: options slstatus swim

options:
	@echo swim build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

#config.h:
#	cp config.def.h $@
slstatus:
	$(MAKE) -C ./slstatus MAKEFLAGS=
st:
	$(MAKE) -C ./st MAKEFLAGS=
dmenu:
	$(MAKE) -C ./dmenu MAKEFLAGS=
slock:
	$(MAKE) -C ./slock MAKEFLAGS=
swim: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f swim ${OBJ} swim-${VERSION}.tar.gz
	$(MAKE) -C ./slstatus clean MAKEFLAGS=
	$(MAKE) -C ./st clean MAKEFLAGS=
	$(MAKE) -C ./dmenu clean MAKEFLAGS=
	$(MAKE) -C ./slock clean MAKEFLAGS=

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f swim ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/swim
	$(MAKE) -C ./slstatus install MAKEFLAGS=
	$(MAKE) -C ./st install MAKEFLAGS=
	$(MAKE) -C ./dmenu install MAKEFLAGS=
	$(MAKE) -C ./slock install MAKEFLAGS=

#	mkdir -p ${DESTDIR}${MANPREFIX}/man1
#	sed "s/VERSION/${VERSION}/g" < dwm.1 > ${DESTDIR}${MANPREFIX}/man1/dwm.1
#	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/swim
#		${DESTDIR}${MANPREFIX}/man1/dwm.1

.PHONY: all options clean install uninstall slstatus st dmenu slock
