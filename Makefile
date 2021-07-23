BUILD_PATH = build
CXX = g++
CXX_FLAGS = -std=c++17 -Ofast

OUTPUT_ARCHIVE_NAME = program.zip
OUTPUT_EXECUTABLE_NAME_MAIN = run
OUTPUT_EXECUTABLE_NAME_KB_READER = kb_read
OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER = tw_snd
OUTPUT_EXECUTABLE_NAME_TW_CONFIG = tw_config

DEB_PACKAGE_NAME = typewriter_keyboard

SOURCES_OS = $(wildcard src/os_linux/*.cpp)
SOURCES_KB = $(wildcard src/keyboard/*.cpp)
SOURCES_SND = $(wildcard src/sound/*.cpp)
SOURCES_TW = $(wildcard src/typewriter/*.cpp)

SOURCE_MAIN = src/main.cpp $(SOURCES_KB) $(SOURCES_SND) $(SOURCES_TW) $(SOURCES_OS)
SOURCE_KB_READER = src/kb_reader.cpp $(SOURCES_KB) $(SOURCES_OS)
SOURCE_TW_SND_PLAYER = src/typewriter_sound_player.cpp $(SOURCES_SND) $(SOURCES_TW) $(SOURCES_OS)
SOURCE_TW_CONFIG = src/tw_config.cpp $(SOURCES_OS)

HEADERS = include
LIBS = -lSDL2 -lSDL2_mixer -lrt

BUILD_ABS_PATH = $(realpath .)/$(BUILD_PATH)
GENERATED_ABS_PATH = $(BUILD_ABS_PATH)/generated

ALL_GENERATED_SOURCES = $$(find $(GENERATED_ABS_PATH) -type f)

DEB_PATH = $(BUILD_ABS_PATH)/$(DEB_PACKAGE_NAME)
DEB_PKG_SIZE = "$$( du -ck $(DEB_PATH) | tail -1 | cut -f1 )"

GIT_USERNAME = $$(git config user.name)
GIT_EMAIL = $$(git config user.email)

all: zip app separate tw_config deb

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
	cp scripts/run_separate.sh $(BUILD_ABS_PATH)/
	chmod +x $(BUILD_ABS_PATH)/run_separate.sh

kb_reader:
	mkdir -p $(BUILD_ABS_PATH)
	$(CXX)                  \
		$(CXX_FLAGS)        \
		$(SOURCE_KB_READER) \
		-I$(HEADERS)        \
		-o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_KB_READER)

sound_player: convert_assets_to_code
	mkdir -p $(BUILD_ABS_PATH)
	$(CXX)                       \
		$(CXX_FLAGS)             \
		$(SOURCE_TW_SND_PLAYER)  \
		$(ALL_GENERATED_SOURCES) \
		-I$(HEADERS)             \
		$(LIBS)                  \
		-DINJECT_RESOURCES       \
		-o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER)

tw_config:
	mkdir -p $(BUILD_ABS_PATH)
	$(CXX)                       \
		$(CXX_FLAGS)             \
		$(SOURCE_TW_CONFIG)      \
		-I$(HEADERS)             \
		$(LIBS)                  \
		-o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_CONFIG)

convert_assets_to_code:
	mkdir -p $(GENERATED_ABS_PATH)
	for asset in $(shell cd assets && find . -type f); do                         \
		output_name=$$(echo $${asset} | tr / _ | sed 's/.wav/.cpp/');             \
		$$(cd assets && xxd -i $${asset} > $(GENERATED_ABS_PATH)/$${output_name});\
	done

deb: separate tw_config
	mkdir -p $(BUILD_ABS_PATH)/$(DEB_PACKAGE_NAME)/
	cp -r scripts/deb/* $(BUILD_ABS_PATH)/$(DEB_PACKAGE_NAME)/
	cp $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_KB_READER) $(DEB_PATH)/opt/typewriter_keyboard
	cp $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER) $(DEB_PATH)/opt/typewriter_keyboard
	cp $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_CONFIG) $(DEB_PATH)/opt/typewriter_keyboard
	cp $(BUILD_ABS_PATH)/run_separate.sh $(DEB_PATH)/opt/typewriter_keyboard
	cat scripts/deb/DEBIAN/control                  |\
		sed "s/@APP_SIZE@/$(DEB_PKG_SIZE)/g"        |\
		sed "s/@MAINTAINER_NAME@/$(GIT_USERNAME)/g" |\
		sed "s/@MAINTAINER_EMAIL@/$(GIT_EMAIL)/g"   > $(DEB_PATH)/DEBIAN/control
	chmod 775 $(DEB_PATH)/DEBIAN/prerm
	cd $(BUILD_ABS_PATH) && dpkg-deb --build ./$(DEB_PACKAGE_NAME)


clean:
	rm -rf $(BUILD_ABS_PATH)