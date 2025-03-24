# Compiler and flags
CXX = g++
CXXFLAGS = -g -I C:/Qt/Qt5.14.2/5.14.2/mingw73_32/include \
           -I C:/Qt/Qt5.14.2/Tools/mingw730_32/lib/gcc/i686-w64-mingw32/7.3.0/include/c++ \
           -I C:/Qt/Qt5.14.2/Tools/mingw730_32/lib/gcc/i686-w64-mingw32/7.3.0/include/c++/i686-w64-mingw32 \
           -I C:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtWidgets \
           -I C:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtGui \
           -I C:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtANGLE \
           -I C:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtNetwork \
           -I C:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtCore

LDFLAGS = -L C:/Qt/Qt5.14.2/5.14.2/mingw73_32/lib \
          -lQt5Core -lQt5Gui -lQt5Widgets -lQt5Network -lqtmain -liconv -lkernel32

# Paths
SRC_DIR = .
MOC_DIR = moc
OBJ_DIR = obj
BIN_DIR = bin

# Sources and objects
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c) $(wildcard $(MOC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp)) \
            $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c)) \
            $(patsubst $(MOC_DIR)/%.cpp, $(OBJ_DIR)/moc_%.o, $(wildcard $(MOC_DIR)/*.cpp))

# Executable
TARGET = $(BIN_DIR)/my_program

# Default target
all: $(TARGET)

# Generate MOC files
generate-moc:
	@echo "Generating MOC files..."
	powershell -ExecutionPolicy Bypass -File ./createMOC.ps1


# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/moc_%.o: $(MOC_DIR)/%.cpp
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files into the final executable
$(TARGET): $(OBJ_FILES)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

	@echo "build successful..."


# Clean up
clean:
	@if exist $(OBJ_DIR)\*.o del /Q $(OBJ_DIR)\*.o
	@if exist $(BIN_DIR)\my_program.exe del /Q $(BIN_DIR)\my_program.exe
