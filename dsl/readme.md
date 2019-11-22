# Pareto Calculator DSL

This folder contains material related to a domain specific language (DSL) for the Pareto Calculator

## Xtext project

The DSL is provided in the form of an Eclipse-Xtext project

### Building the DSL Language Server

The following procedure builds a java application that acts as a Language Server (<https://langserver.org/>, <https://microsoft.github.io/language-server-protocol/>) for the Pareto Calculator DSL.

TODO: build process to be streamlined/simplified.

- Make sure that you have Eclipse with Xtex and Gradle support
- Open Eclipse and open the work space `dsl/eclipse-workspace`. It will appear empty in Eclipse at first.
- Import the project `org.xtext.paretoalgebra.calculator.parent` as an `Existing Gradle Project`.
- Under the project `org.xtext.paretoalgebra.calculator`, in the directory `src/org.xtext.paretoalgebra.calculator`, find the file called, `ParetoSpecification.xtext`, right-click on it and select `Run As` -> `Generate Xtext Artifacts`.
- In a command shell, go to the directory `dsl/eclipse-workspace/org.xtext.paretoalgebra.calculator.parent` and type

  ``` bash
  ./gradlew installDist
  ```

  Then the directory `dsl/eclipse-workspace/org.xtext.paretoalgebra.calculator.parent\org.xtext.paretoalgebra.calculator.ide/build/install/xtext-server` contains the Language Server application.
