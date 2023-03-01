# Pareto Calculator and Pareto Algebra DSL

## What are the Pareto Calculator and Pareto Algebra DSL?

The Pareto Calculator is a library that implements Pareto
Algebra [1]. You can find more information about Pareto Algebra and
the calculator at

- <https://computationalmodeling.info/wp/models/pareto-algebra/>
- <https://computationalmodeling.info/wp/tools/the-pareto-calculator/>

In addition there is a definition of a Domain-Specific Language for Pareto Algebra models. It can convert a specification in the DSL into an xml file that can be processed by the Pareto Calculator

------------------------------------------------------------------

## Pareto Calculator

### How do I use / install it?

#### Dependencies

_Which software dependencies does the Pareto Calculator have?_

The ParetoCalculator needs the `libxml2` library to access XML files. This library is available in most Linux distributions and Windows and Linux versions can be downloaded from [www.libxml2.org](www.libxml2.org).

#### Using Docker

The docker image defined by the `Dockerfile` builds the Pareto Calculator and the DSL tools.

#### On Windows

See [README_WIN.md](README_WIN.md)

#### On Linux

The static library with all Pareto Algebra operations can be compiled with Linux/gcc. The `ParetoCalculatorConsole` directory contains a small example application that uses the library.

See [README_LINUX.md](README_LINUX.md)

------------------------------------------------------------------

### Documentation

Is there any documentation for the Pareto Calculator?

There is currently no documentation for the Pareto Calculator. The Linux build generates doxygen documentation.

Documentation for the library can be found at <http://www.es.ele.tue.nl/pareto.>

------------------------------------------------------------------

## Pareto Algebra DSL

The Pareto Algebra DSL defines a domain-specific language for Pareto Algebra using Eclipse Xtext.
A gradle project is defined that builds a language server infrastructure and a converter application that transforms a model in the DSL into an xml file that can be used as input for the Pareto Calculator.

``` sh
cd dsl/eclipse-workspace/org.xtext.paretoalgebra.calculator.parent
./gradlew installDist
```

## License

What license is used for the Pareto Calculator?

The software is licensed under the MIT License.

------------------------------------------------------------------

## CONTACT INFORMATION

   Marc Geilen <m.c.w.geilen@tue.nl>

## Acknowledgements

The development of the Pareto Calculator was supported by

- the **Betsy** project under the European IST FP6 program, proj. nr. 004042
- the **MNEMEE** project under proj. nr. IST-216224. 
- the **FitOpTiVis** project under the Horizon 2020 ECSEL program H2020-ECSEL-2017-2-RIA, proj. nr. 783162

[1] M.C.W. Geilen, T. Basten, B.D. Theelen, R.H.J.M. Otten. [An Algebra of Pareto Points.](https://dl.acm.org/citation.cfm?id=1366010)
   Fundamenta Informaticae. 78(1):35-74, 2007. (Special issue with best papers of ACSD 2005.) IOS Press.

[2] M.C.W. Geilen, T. Basten. [A Calculator for Pareto Points](https://ieeexplore.ieee.org/document/4211810). in Proceedings of the 10th Design, Automation and Test in Europe Conference and Exhibition (DATE 2007) 16-20 April 2007, Nice, France. Institute of Electrical and Electronics Engineers, Piscataway, New Jersey, pp. 285-291, DOI: 10.1109/DATE.2007.364605


[2] H. Shojaei, T. Basten, M.C.W. Geilen, P. Stanley-Marbell, [SPaC: A symbolic Pareto Calculator](https://dl.acm.org/citation.cfm?id=1450176). in Proceedings of the International Conference on Hardware-Software Codesign and System Synthesis, CODES+ISSS 2008. Association for Computing Machinery, LNCS l., pp. 179-184.

