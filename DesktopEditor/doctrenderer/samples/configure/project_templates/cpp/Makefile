CXX 		= g++
CXXFLAGS	= -std=gnu++11 -Wall -W -fPIC
INCPATH		= -I[BUILDER_DIR]/include -I[ROOT_DIR]
LINK		= g++
LFLAGS		= -Wl,--unresolved-symbols=ignore-in-shared-libs -Wl,-rpath,[BUILDER_DIR]
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
	./$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)
