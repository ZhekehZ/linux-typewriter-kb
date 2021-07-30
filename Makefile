VERSION=0.2-alpha

BUILD_PATH = build
CXX_FLAGS = -std=c++17 -Ofast

OUTPUT_EXECUTABLE_NAME_KB_READER = kb_read
OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER = tw_snd

DEB_PACKAGE_NAME = typewriter_keyboard
GNOME_EXTENSION_NAME = $$(scripts/get_extension_name.sh)

SOURCES_OS = $(wildcard src/os_linux/*.cpp)
SOURCES_KB = $(wildcard src/keyboard/*.cpp)
SOURCES_SND = $(wildcard src/sound/*.cpp)
SOURCES_TW = $(wildcard src/typewriter/*.cpp)

SOURCE_KB_READER = src/kb_reader.cpp $(SOURCES_KB) $(SOURCES_OS)
SOURCE_TW_SND_PLAYER = src/typewriter_sound_player.cpp $(SOURCES_SND) $(SOURCES_TW) $(SOURCES_OS)

HEADERS = include
LIBS = -lSDL2 -lSDL2_mixer -lrt

BUILD_ABS_PATH = $(realpath .)/$(BUILD_PATH)
GENERATED_ABS_PATH = $(BUILD_ABS_PATH)/generated

ALL_GENERATED_SOURCES = $$(find $(GENERATED_ABS_PATH) -type f)

DEB_PATH = $(BUILD_ABS_PATH)/$(DEB_PACKAGE_NAME)

all: deb gnome-extension

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

convert_assets_to_code:
	mkdir -p $(GENERATED_ABS_PATH)
	for asset in $(shell cd assets && find . -type f); do                         \
		output_name=$$(echo $${asset} | tr / _ | sed 's/.wav/.cpp/');             \
		$$(cd assets && xxd -i $${asset} > $(GENERATED_ABS_PATH)/$${output_name});\
	done

deb: separate
	mkdir -p $(BUILD_ABS_PATH)/$(DEB_PACKAGE_NAME)/
	cp -r scripts/deb/* $(BUILD_ABS_PATH)/$(DEB_PACKAGE_NAME)/
	cp $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_KB_READER) $(DEB_PATH)/opt/typewriter_keyboard
	cp $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER) $(DEB_PATH)/opt/typewriter_keyboard
	cp $(BUILD_ABS_PATH)/run_separate.sh $(DEB_PATH)/opt/typewriter_keyboard
	sh scripts/generate_control_script.sh $(DEB_PATH) $(VERSION)  \
		$(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_KB_READER)     \
		$(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER) > $(DEB_PATH)/DEBIAN/control 
	chmod 775 $(DEB_PATH)/DEBIAN/prerm
	cd $(BUILD_ABS_PATH) && dpkg-deb --build ./$(DEB_PACKAGE_NAME)

gnome-extension: separate
	mkdir -p $(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME)/
	cp -r scripts/extension/* $(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME)/
	cp $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_KB_READER) $(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME)/
	cp $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER) $(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME)/
	zip -jD "$(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME).zip" $(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME)/*


clean:
	rm -rf $(BUILD_ABS_PATH)
