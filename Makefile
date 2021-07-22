BUILD_PATH = build
CXX = g++
CXX_FLAGS = -std=c++17 -Ofast

OUTPUT_ARCHIVE_NAME = program.zip
OUTPUT_EXECUTABLE_NAME_MAIN = run
OUTPUT_EXECUTABLE_NAME_KB_READER = kb_read
OUTPUT_EXECUTABLE_NAMETW_SND_PLAYER= tw_snd

SOURCES_KB = $(wildcard src/keyboard/*.cpp)
SOURCES_SND = $(wildcard src/sound/*.cpp)
SOURCES_TW = $(wildcard src/typewriter/*.cpp)

SOURCE_MAIN = src/main.cpp $(SOURCES_KB) $(SOURCES_SND) $(SOURCES_TW)
SOURCE_KB_READER = src/kb_reader.cpp $(SOURCES_KB)
SOURCE_TW_SND_PLAYER = src/typewriter_sound_player.cpp $(SOURCES_SND) $(SOURCES_TW)

HEADERS = include
LIBS = -lSDL2 -lSDL2_mixer

BUILD_ABS_PATH = $(realpath .)/$(BUILD_PATH)
GENERATED_ABS_PATH = $(BUILD_ABS_PATH)/generated

ALL_GENERATED_SOURCES = $$(find $(GENERATED_ABS_PATH) -type f)

all: zip app separate_utils

zip:
	mkdir -p $(BUILD_PATH)
	$(CXX)             \
		$(CXX_FLAGS)   \
		$(SOURCE_MAIN) \
		-I$(HEADERS)   \
		$(LIBS)        \
		-o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_MAIN)
	cp -r assets $(BUILD_ABS_PATH)
	cd $(BUILD_ABS_PATH) && zip $(OUTPUT_ARCHIVE_NAME) -r assets $(OUTPUT_EXECUTABLE_NAME_MAIN)

app: convert_assets_to_code
	$(CXX)                        \
		$(CXX_FLAGS)              \
		$(SOURCE_MAIN)            \
		$(ALL_GENERATED_SOURCES)  \
		-I$(HEADERS)              \
		$(LIBS)                   \
		-DINJECT_RESOURCES        \
		-o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_MAIN)

separate: kb_reader sound_player

kb_reader:
	$(CXX)                  \
		$(CXX_FLAGS)        \
		$(SOURCE_KB_READER) \
		-I$(HEADERS)        \
		-o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_KB_READER)

sound_player: convert_assets_to_code
	$(CXX)                       \
		$(CXX_FLAGS)             \
		$(SOURCE_TW_SND_PLAYER)  \
		$(ALL_GENERATED_SOURCES) \
		-I$(HEADERS)             \
		$(LIBS)                  \
		-DINJECT_RESOURCES       \
		-o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAMETW_SND_PLAYER)

convert_assets_to_code:
	mkdir -p $(GENERATED_ABS_PATH)
	for asset in $(shell cd assets && find . -type f); do                         \
		output_name=$$(echo $${asset} | tr / _ | sed 's/.wav/.cpp/');             \
		$$(cd assets && xxd -i $${asset} > $(GENERATED_ABS_PATH)/$${output_name});\
	done
