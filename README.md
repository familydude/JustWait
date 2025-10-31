# JustWait - MIDI Delay Plugin

A VST3/AU MIDI effect plugin for Logic Pro and other DAWs that delays MIDI events, adds probabilistic note filtering, and limits polyphony for creative generative music effects.

## Features

- **Triple Knob Interface**: Three intuitive controls for comprehensive MIDI processing
- **Wait Time Range**: 0 to 10 seconds (10,000 ms) in 50 ms increments
- **Likelihood Control**: 0-100% probability filter for generative effects
- **Polyphony Limiting**: Control maximum simultaneous notes (1-128)
- **MIDI Effect**: Processes all incoming MIDI events (notes, CC, etc.)
- **Sample-Accurate**: Precise timing using priority queue scheduling
- **Cross-Platform**: Supports macOS (AU/VST3) and Windows/Linux (VST3)

## How It Works

JustWait captures all incoming MIDI events and applies three transformations:

1. **Polyphony Limiting**: Monitors active notes and prevents new note-ons when the maximum is reached. When set to 128, polyphony is unlimited. Lower values create sparse, controlled textures.

2. **Likelihood Filter**: Each event has a probability (0-100%) of being allowed through. At 100%, all events pass. At 50%, approximately half the events pass randomly. At 0%, no events pass.

3. **Time Delay**: Events that pass both polyphony and likelihood filters are delayed by the specified wait time before being sent to output.

This combination is useful for:

- Preventing MIDI "pile-ups" and controlling texture density
- Creating generative/probabilistic music patterns
- Limiting CPU usage of downstream instruments
- Adding humanization and variation to MIDI sequences
- Creating evolving textures with partial note playback
- Voice-stealing effects and dynamic note control
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
3. Adjust the three knobs to shape your MIDI:
   - **"Wait For..."** (left) - Set your desired delay time
   - **"Likelihood"** (middle) - Control the probability of notes playing
   - **"Max Notes"** (right) - Limit maximum simultaneous notes
4. Values are displayed below each knob

**Tips:**
- Set Likelihood to 100% and Max Notes to 128 for consistent delay without filtering
- Lower Likelihood values (30-70%) create sparse, generative patterns
- Use Max Notes at 4-8 for chord limiting or voice-leading effects
- Set Max Notes to 1 for monophonic output regardless of input
- Combine low Max Notes (2-4) with moderate delay for arpeggiated textures
- Use Max Notes limiting to prevent CPU overload with complex instruments

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
  - **Max Notes**
    - Range: 1-128 notes
    - Step Size: 1 note
    - Default: 128 (unlimited)
- **MIDI Processing**:
  - Active note tracking with channel awareness
  - Polyphony limiting at the output stage
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

- **PluginProcessor**: Handles MIDI event buffering, polyphony tracking, probabilistic filtering, and delay processing
- **PluginEditor**: Provides the UI with triple rotary knob controls
- **Parameter System**: Uses JUCE's AudioProcessorValueTreeState for state management
- **Note Tracking**: std::set-based active note tracking with channel awareness
- **Random Engine**: Mersenne Twister (std::mt19937) for high-quality random number generation

## License

[Add your license information here]

## Credits

Built with [JUCE](https://juce.com/) framework.
