
MACRO (CONFIG_DEFINE_VARIABLES)
    
    SET (DEB_PACKAGE_NAME      typewriter-keyboard )
    SET (DEB_VERSION           0.6-alpha           )
    SET (DEB_MAINTAINER_NAME   zhekehz             )
    SET (DEB_MAINTAINER_EMAIL  zzzheka97@gmail.com )
    SET (DEB_DESCRIPTION       "Typewriter sounds for keyboard")

    SET (GEXT_VERSION          0.6                 )
    SET (GEXT_NAME             Typewriter\ Keyboard)
    SET (GEXT_SHELL_VERSIONS   "\"3.38\", \"40\", \"41\", \"42\"")
    SET (GEXT_DESCRIPTION      "Add a typewriter effect to your keyboard."
                               " "
                               "This extension is based on typewriter-kb application "
                               "(https://github.com/ZhekehZ/linux-typewriter-kb), which "
                               "is required to run the extension, but is not part of it."
                               " "
                               "-------------------------------------------"
                               "Icons made by Freepik from www.flaticon.com")

ENDMACRO()

MACRO (CONFIG_EVAL_VARIABLES)
    STRING(REPLACE "@" "." _DEB_MAINTAINER_EMAIL ${DEB_MAINTAINER_EMAIL})
    SET(GNOME_EXTENSION_NAME typewriter-kb@${_DEB_MAINTAINER_EMAIL})

    STRING(JOIN "\\n" GEXT_DESCRIPTION ${GEXT_DESCRIPTION})
ENDMACRO()

CONFIG_DEFINE_VARIABLES()
CONFIG_EVAL_VARIABLES()