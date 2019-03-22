windowManager:
	$(MAKE) -C ./windowManager MAKEFLAGS=
all: windowManager
install:
	$(MAKE) -C ./windowManager install MAKEFLAGS=
clean:
	$(MAKE) -C ./windowManager clean MAKEFLAGS=

.PHONY: all windowManager
