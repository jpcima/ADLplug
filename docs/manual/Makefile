
PREFIX = /usr/local
DOCDIR = $(PREFIX)/share/doc
LANGS = fr

all:
	$(foreach lang,$(LANGS),$(MAKE) -C $(lang))

clean:
	$(foreach lang,$(LANGS),$(MAKE) -C $(lang) clean)

install:
	install -d $(DESTDIR)$(DOCDIR)/ADLplug/manual/
	cp -rf resources $(DESTDIR)$(DOCDIR)/ADLplug/manual/
	$(foreach lang,$(LANGS),install -D $(lang)/manual.html $(DESTDIR)$(DOCDIR)/ADLplug/manual/$(lang)/manual.html)
