# Makefile for WireframeRenderer project

TARGET_CPU = cpu_wireframe
TARGET_GPU = gpu_wireframe
BUILD_DIR_CPU = build_cpu_wireframe
BUILD_DIR_GPU = build_gpu_wireframe

.PHONY: all clean fclean re

all:
	@echo "Building using CMake..."
	@mkdir -p $(BUILD_DIR_CPU)
	@cd $(BUILD_DIR_CPU) && cmake ../cpu_wireframing
	@cd $(BUILD_DIR_CPU) && make
	@cp $(BUILD_DIR_CPU)/$(TARGET_CPU) .

$(TARGET_CPU): all

gpu:
	@echo "Building using CMake..."
	@mkdir -p $(BUILD_DIR_GPU)
	@cd $(BUILD_DIR_GPU) && cmake ../gpu_wireframing
	@cd $(BUILD_DIR_GPU) && make
	@cp $(BUILD_DIR_GPU)/$(TARGET_GPU) .


clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR_CPU)
	@rm -rf $(BUILD_DIR_GPU)

fclean: clean
	@echo "Removing executable..."
	@rm -f $(TARGET_CPU)
	@rm -f $(TARGET_GPU)

re: fclean all
	@echo "Remaking..."
