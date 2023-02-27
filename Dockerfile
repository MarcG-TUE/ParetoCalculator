FROM ubuntu:20.04

WORKDIR /usr
ENV TZ=Europe/Amsterdam
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update
RUN apt-get -y install default-jdk build-essential cmake libxml2-dev gradle dos2unix

# Create directory
WORKDIR /usr/paretocalculator

# compile xtext stuff
COPY ./ .

RUN cmake .
RUN make
RUN ./ParetoCalculatorConsole/ParetoCalculatorConsole ./examples/betsy.xml

# RUN make test

WORKDIR /usr/paretocalculator/dsl/eclipse-workspace/org.xtext.paretoalgebra.calculator.parent
RUN dos2unix ./gradlew
RUN chmod a+x ./gradlew
RUN ./gradlew installdist

