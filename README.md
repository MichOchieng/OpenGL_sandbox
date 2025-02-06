# OpenGL Sandbox
A collection of examples from [learn OpenGL](https://learnopengl.com)

## Requirements
[Conan](https://conan.io/)
[CMake](https://cmake.org/)
C Compiler of your choice:
- [GCC](https://gcc.gnu.org/)
- [MSBuild](https://github.com/dotnet/msbuild)
- [Apple Clang (comes with XCode)](https://opensource.apple.com/projects/llvm-clang)
## Setup

### Mac OS
#### Install Git

##### Xcode
Apple ships a binary package of Git with [Xcode](https://developer.apple.com/xcode/)

##### Homebrew
Install [homebrew](https://brew.sh/) if you don't already have it, then:  

```bash

brew install git

```

#### Install Conan

##### Python (Recommended as python is needed)

```bash

pip install conan

```

##### Homebrew

```bash

brew install conan

```

##### Adding a Profile to Conan

Once Conan is installed create a profile:

```bash

conan profile detect

```

#### Install Cmake

```bash

brew install cmake

```

## Installing dependancies and Building files

On a fresh pull from GitHub run the following in the root directory:

```bash

conan install . --build=missing

```
  
Use Cmake to build the files with:

```bash

cmake --preset conan-release

```

Followed by

```bash

cmake --build --preset conan-release

```

Now you can run the build:
  
```bash
%% Inside the Build Directory %%
./hello_triangle

```