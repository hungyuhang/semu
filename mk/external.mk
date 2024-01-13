# For each external target, the following must be defined in advance:
#   _DATA_URL : the hyperlink which points to archive.
#   _DATA : the file to be read by specific executable.
#   _DATA_SHA1 : the checksum of the content in _DATA

COMMON_URL = https://github.com/hungyuhang/semu/raw/blob

# kernel
KERNEL_DATA_URL = $(COMMON_URL)/Image_rtc_feature.bz2
KERNEL_DATA = Image_rtc_feature
KERNEL_DATA_SHA1 = 9d9a216d3bd283a30fb9dafd8096ce598c481e21

# initrd
INITRD_DATA_URL = $(COMMON_URL)/rootfs.cpio.bz2
INITRD_DATA = rootfs.cpio
INITRD_DATA_SHA1 = fad749d0a9eb33178525f961d6b82e7c0ce917a7

define download
$($(T)_DATA):
	$(VECHO) "  GET\t$$@\n"
	$(Q)curl --progress-bar -O -L -C - "$(strip $($(T)_DATA_URL))"
	$(Q)echo "$(strip $$($(T)_DATA_SHA1)) $$@.bz2" | $(SHA1SUM) -c
	$(Q)bunzip2 $$@.bz2
endef

EXTERNAL_DATA = KERNEL INITRD
$(foreach T,$(EXTERNAL_DATA),$(eval $(download))) 
