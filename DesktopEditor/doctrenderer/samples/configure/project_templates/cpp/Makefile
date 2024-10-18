CXX 		= [COMPILER]
CXXFLAGS	= -std=gnu++11 -Wall -W -fPIC
INCPATH		= -I[BUILDER_DIR]/include -I[ROOT_DIR]
LINK		= [COMPILER]
LFLAGS		= [LFLAGS]
LIBS		= -L[BUILDER_DIR] -ldoctrenderer

BUILD_DIR 	= build

SRC    		= ../../../cpp/[TEST_NAME]/main.cpp
OBJ  	  	= $(BUILD_DIR)/main.o
TARGET      = $(BUILD_DIR)/[TEST_NAME]

.PHONY: all run clean

all: $(TARGET) run

$(TARGET): $(OBJ)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

$(OBJ): $(SRC)
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ) $(SRC)

run: $(TARGET)
	[ENV_LIB_PATH]="[BUILDER_DIR]" ./$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)
