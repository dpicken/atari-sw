export PATH := ../third-party/AtariSio/tools:$(PATH)

BUILD_DIR_SBC_BOOT := ./build_sbc-boot

SBC_BOOT_ATR := $(BUILD_DIR_SBC_BOOT)/!sbc-boot.atr

$(BUILD_DIR_SBC_BOOT):
	$(echo_recipe)mkdir $(BUILD_DIR_SBC_BOOT)

$(SBC_BOOT_ATR): ./build_release/sbc/sbc | $(BUILD_DIR_SBC_BOOT)
	$(echo_build_message)
	$(echo_recipe)cp $< $(BUILD_DIR_SBC_BOOT)
	$(echo_recipe)dir2atr -S -b PicoBoot406 $@ $(BUILD_DIR_SBC_BOOT)

sbc-boot: $(SBC_BOOT_ATR)

.PHONY: sbc-boot-distribute
sbc-boot-distribute: sbc-boot
	$(echo_build_message)
	$(echo_recipe)cp $(SBC_BOOT_ATR) ../atari-fw/atr

.PHONY: distribute
distribute: sbc-boot-distribute

all: sbc-boot

PHONY: sbc-boot-clean
sbc-boot-clean:
	$(echo_recipe)rm -rf $(BUILD_DIR_SBC_BOOT)

clean: sbc-boot-clean
