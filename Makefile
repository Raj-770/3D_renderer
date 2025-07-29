# Makefile for WireframeRenderer project

# Target executable
TARGET = WireframeRenderer
BUILD_DIR = build

.PHONY: all clean fclean

all: $(TARGET)

# Build the executable by configuring & compiling in $(BUILD_DIR)
$(TARGET):
	@echo "Compiling..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && make
	@cp $(BUILD_DIR)/$(TARGET) .

clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

fclean: clean
	@echo "Removing executable..."
	@rm -f $(TARGET)
