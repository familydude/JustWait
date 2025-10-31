# JustWait - MIDI Delay Plugin

A VST3/AU MIDI effect plugin for Logic Pro and other DAWs that delays MIDI events by a specified time and adds probabilistic note filtering for generative music effects.

## Features

- **Dual Knob Interface**: Intuitive two-knob control for easy operation
- **Wait Time Range**: 0 to 10 seconds (10,000 ms) in 50 ms increments
- **Likelihood Control**: 0-100% probability filter for generative effects
- **MIDI Effect**: Processes all incoming MIDI events (notes, CC, etc.)
- **Sample-Accurate**: Precise timing using priority queue scheduling
- **Cross-Platform**: Supports macOS (AU/VST3) and Windows/Linux (VST3)

## How It Works

JustWait captures all incoming MIDI events and applies two transformations:

1. **Likelihood Filter**: Each event has a probability (0-100%) of being allowed through. At 100%, all events pass. At 50%, approximately half the events pass randomly. At 0%, no events pass.

2. **Time Delay**: Events that pass the likelihood filter are delayed by the specified wait time before being sent to output.

This combination is useful for:

- Creating generative/probabilistic music patterns
- Adding humanization and variation to MIDI sequences
- Creating evolving textures with partial note playback
- Compensating for latency in your setup
- Creative rhythmic delay effects
- Synchronizing MIDI with audio tracks

## Building the Plugin

### Prerequisites

- CMake 3.15 or higher
- A C++17 compatible compiler:
  - macOS: Xcode 10.0+
  - Windows: Visual Studio 2019+
  - Linux: GCC 7+ or Clang 5+

### Build Instructions

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd JustWait
   ```

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Generate build files:
   ```bash
   cmake ..
   ```

4. Build the plugin:
   ```bash
   cmake --build . --config Release
   ```

### Installation

After building, the plugin will be automatically copied to your system's plugin folder:

- **macOS AU**: `~/Library/Audio/Plug-Ins/Components/JustWait.component`
- **macOS VST3**: `~/Library/Audio/Plug-Ins/VST3/JustWait.vst3`
- **Windows VST3**: `C:\Program Files\Common Files\VST3\JustWait.vst3`
- **Linux VST3**: `~/.vst3/JustWait.vst3`

## Using in Logic Pro

1. Insert JustWait on a MIDI track or software instrument track
2. The plugin will appear as a MIDI FX in Logic Pro
3. Adjust the **"Wait For..."** knob (left) to set your desired delay time
4. Adjust the **"Likelihood"** knob (right) to control the probability of notes playing
5. Values are displayed below each knob

**Tips:**
- Set Likelihood to 100% for consistent delay without randomization
- Lower Likelihood values create more sparse, generative patterns
- Combine with delay for evolving, probabilistic textures

## Technical Details

- **Plugin Type**: MIDI Effect
- **Parameters**:
  - **Wait For (ms)**
    - Range: 0-10000 ms
    - Step Size: 50 ms
    - Default: 0 ms
  - **Likelihood (%)**
    - Range: 0-100%
    - Step Size: 1%
    - Default: 100%
- **MIDI Processing**:
  - Probabilistic filtering using Mersenne Twister RNG
  - Sample-accurate delay using priority queue
- **Formats**: VST3, AU, Standalone

## Project Structure

```
JustWait/
├── CMakeLists.txt              # Build configuration
├── Source/
│   ├── PluginProcessor.h       # Plugin logic header
│   ├── PluginProcessor.cpp     # MIDI delay implementation
│   ├── PluginEditor.h          # UI header
│   └── PluginEditor.cpp        # UI implementation
└── README.md
```

## Development

The plugin is built using the JUCE framework, which is automatically downloaded during the CMake configuration step. The MIDI delay is implemented using a priority queue that schedules events based on their target sample position.

### Key Components

- **PluginProcessor**: Handles MIDI event buffering, probabilistic filtering, and delay processing
- **PluginEditor**: Provides the UI with dual rotary knob controls
- **Parameter System**: Uses JUCE's AudioProcessorValueTreeState for state management
- **Random Engine**: Mersenne Twister (std::mt19937) for high-quality random number generation

## License

[Add your license information here]

## Credits

Built with [JUCE](https://juce.com/) framework.
