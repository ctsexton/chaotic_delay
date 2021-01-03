# juce_plugin_template
![build status](https://github.com/ctsexton/juce_plugin_template/workflows/CMake/badge.svg)
Template for JUCE-based plugins using CMake.

# Clone & Build
```
git clone --recursive https://github.com/ctsexton/juce_plugin_template.git
mkdir build && cd build
cmake ..
cmake --build .
```

# Run Tests
After building, simply run the following from the build directory:
```
ctest --verbose
```
