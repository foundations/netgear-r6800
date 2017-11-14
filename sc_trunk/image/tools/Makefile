SUBDIRS = squashfs4.2/squashfs-tools/
all clean:
	@for i in $(SUBDIRS) ; do echo -e "\\e[35mapps: make \\e[36m$$i $@\\e[0m"; $(MAKE) -C src/$$i $@ || exit 1; done
