PKG_NAME=lua
PKG_URL=https://github.com/lua/lua.git
PKG_VERSION=v5-3-4
PKG_LICENSE=MIT

.PHONY: all

all: 
#	The lower case "makefile" is so that it copies over the lua makefile
	@cp Makefile.lua $(PKG_BUILDDIR)/makefile
	"$(MAKE)" -C $(PKG_BUILDDIR)

include $(RIOTBASE)/pkg/pkg.mk
