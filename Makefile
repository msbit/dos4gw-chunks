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
$(BUILD_DIR)/dos4gw-chunks: $(BUILD_DIR)/extract.o
$(BUILD_DIR)/dos4gw-chunks: $(BUILD_DIR)/stream.o
$(BUILD_DIR)/dos4gw-chunks: $(BUILD_DIR)/string.o

$(BUILD_DIR)/dos4gw-chunks.o: $(SOURCE_DIR)/dos4gw-chunks.cc
$(BUILD_DIR)/extract.o: $(SOURCE_DIR)/extract.cc
$(BUILD_DIR)/stream.o: $(SOURCE_DIR)/stream.cc
$(BUILD_DIR)/string.o: $(SOURCE_DIR)/string.cc

clean:
	rm -rf $(BUILD_DIR)
