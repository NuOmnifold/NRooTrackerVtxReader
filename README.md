# Standalone ROOT NRooTrackerVtx Reader

This package provides a standalone application for reading `.root` files containing a `NRooTrackerVtx` TTree without requiring additional framework dependencies.

**IMPORTANT NOTE:** This code is specifically designed for T2K files from Production 6T. If you are using a different production version, the header files may need to be modified accordingly to match the data structure.

## Recent Updates

The code has been significantly refactored to handle dynamic memory allocation correctly. Key changes include:

- Fixed header files to use dynamic array pointers instead of fixed-size arrays
- Updated implementation files to properly allocate, copy, and clean up memory
- Added null pointer checks in the reader to prevent segmentation faults
- Enhanced error handling throughout the codebase

## Files

The package includes:

- `root_reader.cpp`: Main application that reads `.root` file and processes the `NRooTrackerVtx` tree
- `RooTrackerVtxBase.h/cpp`: Base class implementation
- `JNuBeamFlux.h/cpp`: Middle-tier class implementation 
- `NRooTrackerVtx.h/cpp`: Main data class implementation
- `Makefile`: For building the application
- `LinkDef.h`: ROOT dictionary linkage definitions

## Prerequisites

You need ROOT installed on your system with the development headers. This can typically be installed from your package manager or from the [ROOT website](https://root.cern.ch/downloading-root).

## Building

To build the application:

```bash
make clean  # Recommended for a clean rebuild after header changes
make
```

This will create an executable called `root_reader`.

## Usage

Run the application by passing the path to your `.root` file:

```bash
./root_reader.exe /path/to/your/file.root
```

The application will:
1. Open the specified `.root` file
2. Check for both the `NRooTrackerVtx` and `evt` TTrees 
3. Ask how many entries you want to process
4. Optionally filter for events that have reconstruction data
5. Print detailed information about:
   - Event details and vertex position
   - Neutrino flux information
   - Particle kinematics separated by initial state and final state
   - Particle momenta and energies

## Memory Management

The updated code properly handles memory allocation for the dynamic arrays in the `NRooTrackerVtx` class:

- Arrays are initialized to nullptr in the constructor
- Memory is allocated based on the corresponding counter variables (e.g., StdHepN)
- The copy constructor performs deep copies of all arrays
- The destructor properly frees all allocated memory

## Troubleshooting

If you encounter issues:

- Ensure ROOT is properly installed and accessible in your PATH
- Check that your `.root` file actually contains a `NRooTrackerVtx` TTree
- Verify that the structure of the TTree matches the expected class definitions
- If you get segmentation faults, check the code for null pointer access
- Enable debug output by adding `-DDEBUG` to the CXXFLAGS in the Makefile

## Advanced Usage

For more advanced data analysis, you can:

1. Modify the data extraction in `processRootFile()` in `root_reader.cpp`
2. Add new analysis functions to process the extracted data
3. Export the data to other formats for further analysis