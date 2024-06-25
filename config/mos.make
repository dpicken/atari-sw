export PATH := $(HOME)/projects/third-party/llvm-mos/bin:$(PATH)

#CXX = mos-atari8-cart-std-clang++
CXX = mos-atari8-dos-clang++

CXXFLAGS += -Os
CXXFLAGS += -flto

bin_recipe_lib_flags_pre_$(CXX) :=
bin_recipe_lib_flags_post_$(CXX) :=
