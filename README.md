# Standalone ROOT NRooTrackerVtx Reader

This package provides a standalone application for reading `.root` files containing a `NRooTrackerVtx` TTree without requiring additional framework dependencies.

## Files

The package includes:

- `root_reader.cpp`: Main application that reads the `.root` file and processes the `NRooTrackerVtx` tree
- `RooTrackerVtxBase.h/cpp`: Base class implementation
- `JNuBeamFlux.h/cpp`: Middle-tier class implementation
- `NRooTrackerVtx.h/cpp`: Main data class implementation
- `Makefile`: For building the application

## Prerequisites

You need ROOT installed on your system with the development headers. This can typically be installed from your package manager or from the [ROOT website](https://root.cern.ch/downloading-root).

## Building

To build the application:

```bash
make
```

This will create an executable called `root_reader`.

## Usage

Run the application by passing the path to your `.root` file:

```bash
./root_reader /path/to/your/file.root
```

The application will:
1. Open the specified `.root` file
2. Access the `NRooTrackerVtx` TTree
3. Loop over the first few entries
4. For each entry, print information about:
   - Event details
   - Neutrino flux information
   - Particle information

## Customizing

To modify what data is extracted or how it's processed:

1. Edit the `processRootFile` function in `root_reader.cpp`
2. Modify the loop to process more entries (currently limited to 3 for testing)
3. Add additional analysis or data extraction as needed

## Extending

If you need to process additional data structures or implement other functionality:

1. Add your own processing functions to `root_reader.cpp`
2. Add new output formats or visualization capabilities
3. Implement custom analysis based on the extracted data

## Troubleshooting

If you encounter issues:

- Ensure ROOT is properly installed and accessible in your PATH
- Check that your `.root` file actually contains a `NRooTrackerVtx` TTree
- Verify that the structure of the TTree matches the expected class definitions