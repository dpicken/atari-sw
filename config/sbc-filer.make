SBC_FILER_XEX := ./$(BUILD_DIR)/sbc/filer/filer

.PHONY: sbc-filer-distribute
sbc-filer-distribute: $(SBC_FILER_XEX)
	$(echo_build_message)
	$(echo_recipe)cp $< ../atari-fw/builtin/!sbc-filer.xex

.PHONY: distribute
distribute: sbc-filer-distribute
