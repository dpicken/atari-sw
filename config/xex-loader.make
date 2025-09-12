XEX_LOADER_ATR := $(BUILD_DIR)/xex/xex

$(XEX_LOADER_ATR) : config/dos-boot-loader.ld

.PHONY: xex-loader
xex-loader: $(XEX_LOADER_ATR)

.PHONY: xex-loader-distribute
xex-loader-distribute: xex-loader
	$(echo_build_message)
	$(echo_recipe)cp $(XEX_LOADER_ATR) ../atari-fw/builtin/!xex-loader.atr

.PHONY: distribute
distribute: xex-loader-distribute
