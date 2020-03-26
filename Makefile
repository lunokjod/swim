windowManager:
	$(MAKE) -C ./windowManager MAKEFLAGS=
tabbed:
	$(MAKE) -C ./tabbed MAKEFLAGS=
surf: tabbed
	$(MAKE) -C ./surf MAKEFLAGS=
all: windowManager tabbed surf
install:
	$(MAKE) -C ./windowManager install MAKEFLAGS=
	$(MAKE) -C ./tabbed install MAKEFLAGS=
	$(MAKE) -C ./surf install MAKEFLAGS=
	$(MAKE) -C ./tools all MAKEFLAGS=
	$(MAKE) -C ./tools install MAKEFLAGS=
clean:
	$(MAKE) -C ./windowManager clean MAKEFLAGS=
	$(MAKE) -C ./tabbed clean MAKEFLAGS=
	$(MAKE) -C ./surf clean MAKEFLAGS=
	$(MAKE) -C ./tools clean MAKEFLAGS=

.PHONY: all windowManager tabbed surf tools
