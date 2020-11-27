<!-- omit in toc -->
# Sound Design Toolkit - Crumpling Model VST3 Plugin
<!-- omit in toc -->
### A plugin realization of the crumpling model from the [Sound Design Toolkit](http://www.soundobject.org/SDT/) created with [JUCE](https://juce.com/)

<!-- omit in toc -->
## Contents
- [Introduction](#introduction)
- [Software Architecture](#software-architecture)
- [Impact Model](#impact-model)
    - [Description](#description)
    - [Synthesis Algorithm](#synthesis-algorithm)
- [Crumpling Model Plugin](#impact-model-plugin)
- [References](#references)
 
## Introduction
The Sound Design Toolkit consists in a library of ecologically founded (e.g physics based) sound synthesis algorithms. These algorithms have been implemented according to three main points:
- auditory perceptual relevance;
- cartoonification;
- parametric temporal control;

In this project the entire SDT library is ported in C++ and integrated in the JUCE framework for the realization of a VST3 plugin version of the crumpling model.
