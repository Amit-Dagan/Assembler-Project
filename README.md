# Project Description: Assembler for Imaginary Computer Architecture

## Overview

Welcome to the README file for our final project, an Assembler for an imaginary computer architecture. This project aims to create a robust and efficient tool that translates assembly language programs into machine code, enabling the execution of programs on a virtual computer.

## Introduction

In the realm of computer programming, machine code, a sequence of bits representing instructions, is not user-friendly. To bridge the gap, assembly language provides a symbolic representation of machine instructions, making it more convenient for programmers. An assembler is the key tool that translates programs written in assembly language into machine code. This project specifically focuses on implementing an assembler for an imaginary computer architecture.

## Virtual Computer and Assembly Language

### Hardware Model

The imaginary computer in this project comprises a Central Processing Unit (CPU), registers, and Random Access Memory (RAM). Notably, the CPU contains eight general registers and a Program Status Word (PSW) register. The memory is divided into 256 cells, each 10 bits in size.

### Machine Instructions

The assembly language supports 32 unique instructions, each represented by a base-32 code. The machine instructions are encoded into memory words, ranging from one to five words. The project utilizes a 2's complement method for arithmetic operations and supports ASCII character representation.

## Project Structure

### First Pass Algorithm

The assembler operates in two passes: the first pass handles symbol definitions, data storage instructions, and external instructions. The algorithm involves symbol table management, identification of instruction types, and encoding of operands.

### Second Pass Algorithm

The second pass completes the encoding of operands, including addressing methods, and generates output files. It further ensures the identification of external and entry instructions, marking symbols accordingly.

### Internal Data Management

The assembler maintains arrays for instructions and data, using counters (IC and DC) to manage memory. A symbol table records encountered labels during the assembly process.

## How the Assembler Works

The assembler reads the source file, interprets instruction types, and encodes operands based on addressing methods. It builds a symbolic picture of the imaginary machine's memory, managing instruction and data arrays along with counters. The symbol table keeps track of encountered labels.

## Conclusion

This assembler project lays the foundation for executing programs on an imaginary computer architecture. Its modular design, comprehensive algorithms, and efficient data management make it a valuable tool for translating assembly language programs into executable machine code. Feel free to explore, contribute, and utilize this assembler for educational or experimental purposes.

