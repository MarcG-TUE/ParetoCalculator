# Pareto Calculator on Windows

_What is the Pareto Calculator?_

See [README.md](README.md) for a description.

------------------------------------------------------------------

## How do I use / install it on Windows using Visual Studio?

1. if you do not have the `libxml2` library installed, follow the instructions [below](#building-libxml).

2. Open the solution file `ParetoCalcultor.sln` in Visual Studio

3. Provide the path to the `libxml2` library for the linker.
   Select from the menu: `View -> Property Manager`.
     Under any of the different configurations (it does not matter which one you use), find the property sheet named `ExternalLibraries`
     Double click it and on the properties page go to: `Common Properties -> User Macros`.
     Then edit the `LibxmlDir` property appropriately. Make sure it has a trailing `\` symbol.
     It should for instance be something like:
     `D:\software\libraries\libxml2-2.9.3\`

5. Build the solution in the desired configuration `Debug` or `Release` and run the `ParetoCalculatorUI` project.

You can then use the calculator to run the examples in the `test` directory.

Note that the project `ParetoCalculatorLibrary` creates a static library with all functionality for your own Pareto Algebra calculations.

## Building LibXml

Install and compile `libxml2` as follows:

- Get the latest copy from: [ftp://xmlsoft.org/libxml2/](ftp://xmlsoft.org/libxml2/) (e.g., `libxml2-2.9.5.tar.gz`, use at least version 2.9.4)
- Extract it and open the `libxml2` solution in the `win32\VC10` directory.
  It will complain about missing, `libiconv`. We fix this in the next step.
  It may also ask to upgrade the compilers and libraries to the Visual Studio version you are using. Click OK to do so.
- Remove the `libiconv` project (right mouse -> remove)
- Open `include/libxml/xmlversion.h` and

  - disable `LIBXML_ICONV_ENABLED` on line 276 (set the `#if 1` to `#if 0`)
  - disable `LIBXML_ZLIB_ENABLED` on line 399 (set the `#if 1` to `#if 0`)
  - disable `LIBXML_LZMA_ENABLED` on line 408 (set the `#if 1` to `#if 0`)

  Save the file.
- Open `win32/VC10/config.h` and add the following two lines after line 8.

  - `#define SEND_ARG2_CAST`
  - `#define GETHOSTBYNAME_ARG_CAST`

  Save the file.
- Create a new release configuration:

  Open the configuration manager (`Build -> Configuration manager`)
  
  Click on the project `libxml2`.
  Under `Active solution configuration` select `<New...>`

  Give it the name `Release PC` and copy the settings from the `Debug` configuration (using `Release` will not work!)

- For the `Debug` configuration, change the output filename to `$(ProjectName)d`
  (select the `libxml2` project in the solution explorer, then select `project->properties`, go to the `General->Target Name` property and change it to `$(ProjectName)d`)
- Update the new `Release PC` configuration of the `libxml` and `runsuite` projects to link against the Multi-threaded DLL library.

  (for each of the projects, with configuration `Release PC` selected, `project->properties->C/C++->code generation->runtime library -> Multi-threaded DLL /MD`)
- To avoid some warning in the `ParetoCalculator` build later, update the new `Release PC` configuration of the `libxml` and `runsuite` projects to not generate debug information for edit and continue.
  (for each of the projects with configuration `Release PC` selected: `project->properties->C/C++->General->Debug Information Format -> Program Database (/Zi)`)
- Build the `Release PC` configuration and / or `Debug` configuration.
