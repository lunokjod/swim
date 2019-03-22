windowManager:
	$(MAKE) -C ./windowManager MAKEFLAGS=
surf:
	$(MAKE) -C ./surf MAKEFLAGS=
all: windowManager surf
install:
	$(MAKE) -C ./windowManager install MAKEFLAGS=
	$(MAKE) -C ./surf install MAKEFLAGS=
clean:
	$(MAKE) -C ./windowManager clean MAKEFLAGS=
	$(MAKE) -C ./surf clean MAKEFLAGS=

.PHONY: all windowManager surf
