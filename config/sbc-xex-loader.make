SBC_XEX_LOADER_ATR := $(BUILD_DIR)/sbc/xex/xex

$(SBC_XEX_LOADER_ATR): config/xex-boot-loader.ld

.PHONY: sbc-xex-loader-distribute
sbc-xex-loader-distribute: $(SBC_XEX_LOADER_ATR)
	$(echo_build_message)
	$(echo_recipe)cp $< ../atari-fw/builtin/!sbc-xex-loader.atr

.PHONY: distribute
distribute: sbc-xex-loader-distribute
