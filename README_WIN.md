# Pareto Calculator on Windows

_What is the Pareto Calculator?_

See [README.md](README.md) for a description.

------------------------------------------------------------------

## How do I use / install it on Windows using Visual Studio?

1. if you do not have the `libxml2` library installed, first follow the instructions [below](#building-libxml).

2. Open the solution file `ParetoCalculator.sln` in Visual Studio

3. Provide the path to the `libxml2` library for the linker as follows.
   Select from the menu: `View -> Property Manager`.
     Under the `ParetoCalculatorLibrary` project, under any of its configurations (it does not matter which one you use), find the property sheet named `ExternalLibraries`
     Double click it and on the properties page go to: `Common Properties -> User Macros`.
     Then edit the `LibxmlDir` property appropriately. Make sure it has a trailing `\` symbol.
     It should for instance be something like:
     `D:\software\libraries\libxml2-2.9.3\`

5. Build the solution in the desired configuration either `Debug` or `Release` and run the `ParetoCalculatorUI` project.

You can then use the calculator to run the examples in the `examples` directory.

Note that the project `ParetoCalculatorLibrary` project creates a static library in the `lib` directory with all functionality for your own Pareto Algebra calculations.

Note that the `Debug` and `Release` configurations build libraries for the Common-Language Runtime of Windows / .Net. The `Debug Static Lib` and `Release Static Lib` configurations build 'ordinary' libraries to link to.

## Building LibXml

Install and compile `libxml2` as follows:

- Get the latest copy from: <https://gitlab.gnome.org/GNOME/libxml2/-/releases> (e.g., `libxml2-2.9.5.tar.gz`). Use at least v.2.9.4.
- Extract it and open the `libxml2` solution in `win32\VC10`
  It will complain about a missing project, `libiconv`, we fix this in the next step.
  It may also ask to upgrade the compilers and libraries to the Visual Studio version you are using. Click OK to do so.
- Remove the `libiconv` project (right mouse -> remove)
- open `include/libxml/xmlversion.h` and
  disable `LIBXML_ICONV_ENABLED` on line 276 (set the `#if 1` to `#if 0`)
  disable `LIBXML_ZLIB_ENABLED` on line 399 (set the `#if 1` to `#if 0`)
  disable `LIBXML_LZMA_ENABLED` on line 408 (set the `#if 1` to `#if 0`)
  save the file
- open `win32/VC10/config.h` and add the following two lines 
  after Line 8.

  ``` c
  #define SEND_ARG2_CAST
  #define GETHOSTBYNAME_ARG_CAST
   ```

  save the file
- Create a new release configuration:
  Open the configuration manager (Build -> Configuration manager)
  Click on the project `libxml2`.
  Under `Active solution configuration` select `<New...>`
  Give it a name 'Release PC' and copy the settings from the 'Debug' configuration (Release will not work!)
- For the Debug configuration, change the output filename to `$(ProjectName)d`
  (select the `libxml2` project in the solution explorer, then select project->properties go to the General->Target Name property and change it to `$(ProjectName)d`)
- Update the new 'Release PC' configuration of the libxml and runsuite projects to link against the Multi-threaded DLL library.
  (for each of the projects with configuration 'Release PC' selected: project->properties->C/C++->code generation->runtime library -> Multi-threaded DLL /MD)
- To avoid some warning in SDF3 build later, update the new 'Release PC' configuration of the libxml and runsuite projects to not generate debug information for edit and continue.
  (for each of the projects with configuration 'Release PC' selected: project->properties->C/C++->General->Debug Information Format -> Program Database (/Zi) )
- Build the 'Release PC' configuration and / or Debug configuration.

If you intend to build x64 versions of SDF3, you need to build x64 versions of `libxml2` as follows.

 - Open the configuration manager and under 'Active solution platform' select '<New...>'.
 - Select 'x64' as new platform and select Win32 to copy settings from. Keep 'Create new project platforms' checked. Click 'OK' and click 'Close' in the configuration manager main window.
 - Go to the properties of the `libxml2` project. Select Configuration 'Debug' and platform 'x64'. Under the 'General' category, change `Target Name` to: `$(ProjectName)_x64d` and set 'Output Directory' to `$(ProjectDir)..\..\lib\` and click 'Apply'.
 - Select Configuration 'Release PC'. Under the 'General' category, change 'Target Name' to: `$(ProjectName)_x64` and set `Output Directory` to `$(ProjectDir)..\..\lib\` and click 'Apply'.
 - Close the properties window by clicking 'OK'.
 - Build the desired configurations (Debug and/or Release PC).
 - 