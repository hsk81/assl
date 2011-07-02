# $assl$

# Attempt to include platform specific makefile.
# OSNAME may be passed in.
OSNAME ?= $(shell uname -s)
OSNAME := $(shell echo $(OSNAME) | tr A-Z a-z)
-include config/Makefile.$(OSNAME)

# Default paths.
LOCALBASE ?= /usr/local
BINDIR ?= ${LOCALBASE}/bin
LIBDIR ?= ${LOCALBASE}/lib
INCDIR ?= ${LOCALBASE}/include
MANDIR ?= $(LOCALBASE)/share/man

# Use obj directory if it exists.
OBJPREFIX ?= obj/
ifeq "$(wildcard $(OBJPREFIX))" ""
	OBJPREFIX =	
endif

# Get shared library version.
-include shlib_version
SO_MAJOR = $(major)
SO_MINOR = $(minor)

# System utils.
AR ?= ar
CC ?= gcc
INSTALL ?= install
LN ?= ln
LNFORCE ?= -f
LNFLAGS ?= -sf
MKDIR ?= mkdir
RM ?= rm -f

# Compiler and linker flags.
CPPFLAGS += -DNEED_LIBCLENS
INCFLAGS += -I$(INCDIR)/clens -I$(LOCALBASE)/ssl/include
WARNFLAGS ?= -Wall -Werror
DEBUG += -g
CFLAGS += $(INCFLAGS) $(WARNFLAGS) $(DEBUG)
LDFLAGS +=
SHARED_OBJ_EXT ?= o

LIB.NAME = assl
LIB.SRCS = assl.c assl_event.c ssl_privsep.c
LIB.HEADERS = assl.h
LIB.MANPAGES = assl.3
LIB.MLINKS  = assl.3 assl_initialize.3
LIB.MLINKS += assl.3 assl_alloc_context.3
LIB.MLINKS += assl.3 assl_set_cert_flags.3
LIB.MLINKS += assl.3 assl_load_file_certs.3
LIB.MLINKS += assl.3 assl_connect.3
LIB.MLINKS += assl.3 assl_serve.3
LIB.MLINKS += assl.3 assl_accept.3
LIB.MLINKS += assl.3 assl_read.3
LIB.MLINKS += assl.3 assl_write.3
LIB.MLINKS += assl.3 assl_gets.3
LIB.MLINKS += assl.3 assl_puts.3
LIB.MLINKS += assl.3 assl_poll.3
LIB.MLINKS += assl.3 assl_close.3
LIB.MLINKS += assl.3 assl_fatalx.3
LIB.MLINKS += assl.3 assl_event_serve.3
LIB.MLINKS += assl.3 assl_event_serve_stop.3
LIB.MLINKS += assl.3 assl_event_accept.3
LIB.MLINKS += assl.3 assl_event_enable_write.3
LIB.MLINKS += assl.3 assl_event_disable_write.3
LIB.MLINKS += assl.3 assl_event_connect.3
LIB.MLINKS += assl.3 assl_event_close.3
LIB.OBJS = $(addprefix $(OBJPREFIX), $(LIB.SRCS:.c=.o))
LIB.SOBJS = $(addprefix $(OBJPREFIX), $(LIB.SRCS:.c=.$(SHARED_OBJ_EXT)))
LIB.DEPS = $(addsuffix .depend, $(LIB.OBJS))
ifneq "$(LIB.OBJS)" "$(LIB.SOBJS)"
	LIB.DEPS += $(addsuffix .depend, $(LIB.SOBJS))
endif
LIB.MDIRS = $(foreach page, $(LIB.MANPAGES), $(subst ., man, $(suffix $(page)))) 
LIB.MLINKS := $(foreach page, $(LIB.MLINKS), $(subst ., man, $(suffix $(page)))/$(page)) 
LIB.LDFLAGS = $(LDFLAGS.EXTRA) $(LDFLAGS)

all: $(OBJPREFIX)$(LIB.SHARED) $(OBJPREFIX)$(LIB.STATIC)

obj:
	-$(MKDIR) obj

$(OBJPREFIX)$(LIB.SHARED): $(LIB.SOBJS)
	$(CC) $(LDFLAGS.SO) $^ $(LIB.LDFLAGS) -o $@

$(OBJPREFIX)$(LIB.STATIC): $(LIB.OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJPREFIX)%.$(SHARED_OBJ_EXT): %.c
	@echo "Generating $@.depend"
	@$(CC) $(INCFLAGS) -MM $(CPPFLAGS) $< | \
	sed 's,$*\.o[ :]*,$@ $@.depend : ,g' > $@.depend
	$(CC) $(CFLAGS) $(PICFLAG) $(CPPFLAGS) -o $@ -c $<

$(OBJPREFIX)%.o: %.c
	@echo "Generating $@.depend"
	@$(CC) $(INCFLAGS) -MM $(CPPFLAGS) $< | \
	sed 's,$*\.o[ :]*,$@ $@.depend : ,g' >> $@.depend
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $< 

depend: 
	@echo "Dependencies are automatically generated.  This target is not necessary."

install:
	$(INSTALL) -m 0644 $(OBJPREFIX)$(LIB.SHARED) $(LIBDIR)/
	$(LN) $(LNFLAGS) $(LIB.SHARED) $(LIBDIR)/$(LIB.SONAME)
	$(LN) $(LNFLAGS) $(LIB.SHARED) $(LIBDIR)/$(LIB.DEVLNK)
	$(INSTALL) -m 0644 $(OBJPREFIX)$(LIB.STATIC) $(LIBDIR)/
	$(INSTALL) -m 0644 $(LIB.HEADERS) $(INCDIR)/
	$(INSTALL) -m 0755 -d $(addprefix $(MANDIR)/, $(LIB.MDIRS))
	$(foreach page, $(LIB.MANPAGES), \
		$(INSTALL) -m 0444 $(page) $(addprefix $(MANDIR)/, \
		$(subst ., man, $(suffix $(page))))/; \
	)
	@set $(addprefix $(MANDIR)/, $(LIB.MLINKS)); \
	while : ; do \
		case $$# in \
			0) break;; \
			1) echo "Warning: Unbalanced MLINK: $$1"; break;; \
		esac; \
		page=$$1; shift; link=$$1; shift; \
		echo $(LN) $(LNFORCE) $$page $$link; \
		$(LN) $(LNFORCE) $$page $$link; \
	done

uninstall:
	$(RM) $(LIBDIR)/$(LIB.DEVLNK)
	$(RM) $(LIBDIR)/$(LIB.SONAME)
	$(RM) $(LIBDIR)/$(LIB.SHARED)
	$(RM) $(LIBDIR)/$(LIB.STATIC)
	$(RM) $(addprefix $(INCDIR)/, $(LIB.HEADERS))
	@set $(addprefix $(MANDIR)/, $(LIB.MLINKS)); \
	while : ; do \
		case $$# in \
			0) break;; \
			1) echo "Warning: Unbalanced MLINK: $$1"; break;; \
		esac; \
		page=$$1; shift; link=$$1; shift; \
		echo $(RM) $$link; \
		$(RM) $$link; \
	done
	$(foreach page, $(LIB.MANPAGES), \
		$(RM) $(addprefix $(MANDIR)/, \
		$(subst ., man, $(suffix $(page))))/$(page); \
	)

clean:
	$(RM) $(LIB.SOBJS)
	$(RM) $(OBJPREFIX)$(LIB.SHARED)
	$(RM) $(OBJPREFIX)/$(LIB.SONAME)
	$(RM) $(OBJPREFIX)/$(LIB.DEVLNK)
	$(RM) $(LIB.OBJS)
	$(RM) $(OBJPREFIX)$(LIB.STATIC)
	$(RM) $(LIB.DEPS)

-include $(LIB.DEPS)

.PHONY: clean depend install uninstall
