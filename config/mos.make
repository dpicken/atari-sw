export PATH := ../third-party/llvm-mos/bin:$(PATH)

CXX = mos-atari8-dos-clang++

CXXFLAGS += -DBUILD_MOS
CXXFLAGS += -Os
CXXFLAGS += -flto
CXXFLAGS += -fnonreentrant

bin_recipe_lib_flags_pre_$(CXX) :=
bin_recipe_lib_flags_post_$(CXX) :=

PHONY: mos-clean
mos-clean:
	$(echo_recipe)rm -f $(build_dir)/*/*

clean: mos-clean
