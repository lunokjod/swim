windowManager:
	$(MAKE) -C ./windowManager MAKEFLAGS=
all: windowManager
.PHONY: all windowManager
