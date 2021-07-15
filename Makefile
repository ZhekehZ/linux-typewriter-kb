BUILD_PATH = build

SOURCES = $$(find src -type f)
HEADERS = include


BUILD_ABS_PATH = $(realpath $(BUILD_PATH))
GENERATED_ABS_PATH = $(BUILD_ABS_PATH)/generated

ALL_GENERATED_SOURCES = $$(find $(GENERATED_ABS_PATH) -type f)

build_bundle:
	g++ -std=c++17 $(SOURCES) -I$(HEADERS) -lSDL2 -lSDL2_mixer -Ofast -o $(BUILD_ABS_PATH)/run
	cp -r assets $(BUILD_ABS_PATH)


build_single_file: convert_assets_to_code
	g++ -std=c++17 $(SOURCES) $(ALL_GENERATED_SOURCES) -I$(HEADERS) -lSDL2 -lSDL2_mixer -Ofast -DINJECT_RESOURCES -o $(BUILD_ABS_PATH)/run

convert_assets_to_code:
	mkdir -p $(GENERATED_ABS_PATH)
	for asset in $(shell cd assets && find . -type f); do                         \
		output_name=$$(echo $${asset} | tr / _ | sed 's/.wav/.cpp/');             \
		$$(cd assets && xxd -i $${asset} > $(GENERATED_ABS_PATH)/$${output_name});\
	done