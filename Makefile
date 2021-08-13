VERSION=0.4-alpha
VERSION_GNOME=0.4

BUILD_PATH = build
CXX_FLAGS = -std=c++2a -O3  \
	-Wall                   \
	-Werror                 \
	-Wpedantic              \
	-Wextra                 \
	-Wconversion            \
	-Wcast-align            \
	-Wunused                \
	-Wshadow                \
	-Wold-style-cast        \
	-Wpointer-arith         \
	-Wcast-qual             \
	-Wno-missing-braces


MAINTAINER_NAME = zhekehz
MAINTAINER_EMAIL = zzzheka97@gmail.com

OUTPUT_EXECUTABLE_NAME_KB_READER = kb_read
OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER = tw_snd

DEB_PACKAGE_NAME = typewriter_keyboard
GNOME_EXTENSION_NAME = "typewriter-kb@$$( echo $(MAINTAINER_EMAIL) | tr '@' '.' )"

SOURCES_OS = $(wildcard src/os_linux/*.cpp)
SOURCES_KB = $(wildcard src/keyboard/*.cpp)
SOURCES_SND = $(wildcard src/sound/*.cpp)
SOURCES_TW = $(wildcard src/typewriter/*.cpp)

SOURCE_KB_READER = src/kb_reader.cpp $(SOURCES_KB) $(SOURCES_OS)
SOURCE_TW_SND_PLAYER = src/typewriter_sound_player.cpp $(SOURCES_SND) $(SOURCES_TW) $(SOURCES_OS)

HEADERS = include
LIBS = -lSDL2 -lSDL2_mixer -lrt

BUILD_ABS_PATH = $(realpath .)/$(BUILD_PATH)
ASSETS_PATH = $(realpath .)/assets
GENERATED_ABS_PATH = $(BUILD_ABS_PATH)/generated

ALL_GENERATED_SOURCES = $$(find $(GENERATED_ABS_PATH) -type f)

DEB_PATH = $(BUILD_ABS_PATH)/$(DEB_PACKAGE_NAME)

all: deb gnome-extension

format: 
	clang-format -i -style=file $$( find src -type f -name '*.cpp' )     \
								$$( find src -type f -name '*.c' )       \
								$$( find include -type f -name '*.hpp' ) \
								$$( find include -type f -name '*.h' ) 

separate: kb_reader sound_player
	cp scripts/run_separate.sh $(BUILD_ABS_PATH)/
	chmod 777 $(BUILD_ABS_PATH)/run_separate.sh

kb_reader: format
	mkdir -p $(BUILD_ABS_PATH)
	$(CXX)                  \
		$(CXX_FLAGS)        \
		$(SOURCE_KB_READER) \
		-I$(HEADERS)        \
		-o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_KB_READER)

sound_player: format convert_wav_to_code
	mkdir -p $(BUILD_ABS_PATH)
	$(CXX)                       \
		$(CXX_FLAGS)             \
		$(SOURCE_TW_SND_PLAYER)  \
		$(ALL_GENERATED_SOURCES) \
		-I$(HEADERS)             \
		$(LIBS)                  \
		-DINJECT_RESOURCES       \
		-o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER)

convert_wav_to_code:
	mkdir -p $(GENERATED_ABS_PATH)
	for asset in $(shell cd assets/wav && find . -type f); do                         \
		output_name=$$(echo $${asset} | tr / _ | sed 's/.wav/.cpp/');                 \
		$$(cd assets/wav && xxd -i $${asset} > $(GENERATED_ABS_PATH)/$${output_name});\
	done

deb: separate
	mkdir -p $(BUILD_ABS_PATH)/$(DEB_PACKAGE_NAME)/
	cp -r scripts/deb/* $(BUILD_ABS_PATH)/$(DEB_PACKAGE_NAME)/
	cp $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_KB_READER) $(DEB_PATH)/opt/typewriter_keyboard
	cp $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER) $(DEB_PATH)/opt/typewriter_keyboard
	cp $(BUILD_ABS_PATH)/run_separate.sh $(DEB_PATH)/opt/typewriter_keyboard
	sh scripts/generate_control_script.sh                              \
		$(DEB_PATH) $(VERSION) $(MAINTAINER_NAME) $(MAINTAINER_EMAIL)  \
		$(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_KB_READER)          \
		$(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME_TW_SND_PLAYER) > $(DEB_PATH)/DEBIAN/control 
	chmod 775 $(DEB_PATH)/DEBIAN/prerm
	cd $(BUILD_ABS_PATH) && dpkg-deb --build ./$(DEB_PACKAGE_NAME)

gnome-extension: separate
	mkdir -p $(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME)/
	cp -r scripts/extension/* $(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME)/
	sh scripts/generate_metadata.sh $(VERSION_GNOME) $(GNOME_EXTENSION_NAME) > \
			$(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME)/metadata.json
	cp $(ASSETS_PATH)/typewriter.svg $(BUILD_ABS_PATH)/$(GNOME_EXTENSION_NAME)/typewriter-symbolic.svg
	cd $(BUILD_ABS_PATH) && zip -jD "$(GNOME_EXTENSION_NAME).zip" $(GNOME_EXTENSION_NAME)/*


clean:
	rm -rf $(BUILD_ABS_PATH)
