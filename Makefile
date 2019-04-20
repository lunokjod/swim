windowManager:
	$(MAKE) -C ./windowManager MAKEFLAGS=
surf:
	$(MAKE) -C ./surf MAKEFLAGS=
tools:
	$(MAKE) -C ./tools MAKEFLAGS=
all: windowManager surf tools
install:
	$(MAKE) -C ./windowManager install MAKEFLAGS=
	$(MAKE) -C ./surf install MAKEFLAGS=
	$(MAKE) -C ./tools install MAKEFLAGS=
clean:
	$(MAKE) -C ./windowManager clean MAKEFLAGS=
	$(MAKE) -C ./surf clean MAKEFLAGS=
	$(MAKE) -C ./tools clean MAKEFLAGS=

.PHONY: all windowManager surf tools
