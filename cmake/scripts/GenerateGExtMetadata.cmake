INCLUDE(${MAIN_CMAKE}/Config.cmake)

SET (GEXT_UUID ${GNOME_EXTENSION_NAME})

CONFIGURE_FILE(${MAIN_TEMPLATES_DIR}/gnome_extension/metadata.json.in ${GEXT_DIR}/metadata.json)
