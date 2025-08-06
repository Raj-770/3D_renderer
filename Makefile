# Makefile for WireframeRenderer project

TARGET = WireframeRenderer
BUILD_DIR = build

.PHONY: all clean fclean re

all:
	@echo "Building using CMake..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. 
	@cd $(BUILD_DIR) && make
	@cp $(BUILD_DIR)/$(TARGET) .

$(TARGET): all

clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

fclean: clean
	@echo "Removing executable..."
	@rm -f $(TARGET)

re: fclean all
	@echo "Remaking..."
