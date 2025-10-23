cmake_minimum_required(VERSION 3.15)

project(JustWait VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS deployment target
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum macOS deployment version")

# Fetch JUCE
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12
)
FetchContent_MakeAvailable(JUCE)

# Create the plugin
juce_add_plugin(JustWait
    COMPANY_NAME "YourCompany"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Yoco
    PLUGIN_CODE Jstw
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "JustWait"
)

# Add source files
target_sources(JustWait
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

# Set include directories
target_include_directories(JustWait
    PRIVATE
        Source
)

# Link JUCE modules
target_link_libraries(JustWait
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Compile definitions
target_compile_definitions(JustWait
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_COREGRAPHICS_DRAW_ASYNC=1
        JUCE_DISABLE_COREGRAPHICS_FONT_SMOOTHING=1
)
