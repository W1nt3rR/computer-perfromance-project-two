# MSYS2 with Visual Studio Code

This guide will help you set up MSYS2 to work with Visual Studio Code for C/C++ development.

## Prerequisites

-   [Visual Studio Code](https://code.visualstudio.com/)
-   [MSYS2](https://www.msys2.org/)

## Installation Steps

### 1. Install MSYS2

1. Download the MSYS2 installer from the [official website](https://www.msys2.org/).
2. Run the installer and follow the instructions to install MSYS2.

### 2. Update MSYS2

1. Open the MSYS2 terminal (MSYS2 MSYS).

2. Update the package database and core system packages with the following command:

    ```sh
    pacman -Syu
    ```

### 3. Install Required Packages

1. Open the MSYS2 terminal (MSYS2 MSYS).

2. Install the development tools and other required packages. For example, to install the GNU Compiler Collection (GCC) and make:

    ```sh
    pacman -S base-devel gcc
    ```

### 4. Set Up Environment Variables.

1. Add the MSYS2 binary paths to your system PATH. Open your environment variables settings and add the following paths (assuming default installation directory):

    ```
    C:\msys64\usr\bin
    ```

    ```
    C:\msys64\mingw64\bin
    ```

### 5. Run with VS Code Debug
