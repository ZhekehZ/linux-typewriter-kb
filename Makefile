BUILD_PATH = build
CXX_FLAGS = -Ofast

OUTPUT_ARCHIVE_NAME = program.zip
OUTPUT_EXECUTABLE_NAME = run

SOURCES = $$(find src -type f)
HEADERS = include
LIBS = -lSDL2 -lSDL2_mixer

BUILD_ABS_PATH = $(realpath $(BUILD_PATH))
GENERATED_ABS_PATH = $(BUILD_ABS_PATH)/generated

ALL_GENERATED_SOURCES = $$(find $(GENERATED_ABS_PATH) -type f)

all: build_app build_single_file

build_app:
	g++ -std=c++17 $(SOURCES) -I$(HEADERS) $(LIBS) $(CXX_FLAGS) -o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME)
	cp -r assets $(BUILD_ABS_PATH)
	cd $(BUILD_ABS_PATH) && zip $(OUTPUT_ARCHIVE_NAME) -r assets $(OUTPUT_EXECUTABLE_NAME)

build_single_file: convert_assets_to_code
	g++ -std=c++17 $(SOURCES) $(ALL_GENERATED_SOURCES) -I$(HEADERS) $(LIBS) $(CXX_FLAGS) -DINJECT_RESOURCES -o $(BUILD_ABS_PATH)/$(OUTPUT_EXECUTABLE_NAME)

convert_assets_to_code:
	mkdir -p $(GENERATED_ABS_PATH)
	for asset in $(shell cd assets && find . -type f); do                         \
		output_name=$$(echo $${asset} | tr / _ | sed 's/.wav/.cpp/');             \
		$$(cd assets && xxd -i $${asset} > $(GENERATED_ABS_PATH)/$${output_name});\
	done
