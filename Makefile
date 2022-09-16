BUILD_DIR ?= build
SOURCE_DIR ?= source

CXXFLAGS := -Wall -Wextra
CXXFLAGS += -std=c++11
CXXFLAGS += -fsanitize=address
CXXFLAGS += -O2

LDFLAGS := -fsanitize=address

LDLIBS := -lc++

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cc
	mkdir -p $(BUILD_DIR)
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(BUILD_DIR)/dos4gw-chunks: $(BUILD_DIR)/dos4gw-chunks.o

$(BUILD_DIR)/dos4gw-chunks.o: $(SOURCE_DIR)/dos4gw-chunks.cc

clean:
	rm -rf $(BUILD_DIR)
