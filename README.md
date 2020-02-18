# C++ on Mac OS X (or other platforms) Quickstart Guide

This guide will show you how to quickly set up a modern C++ development environment on Mac OS X. The goals here are:

* Quick and simple setup for most cases
* Modern C++ (i.e. C++ 2014)
* Easy management of dependent libraries
* Easy unit tests
* Basically as little messing around with the development environment as possible
* Portability and support for IDE's would be nice

The good news is that all of this is possible with very little effort on Mac, with the right tools:

* [CMake](https://cmake.org) for builds
* [Conan.io](https://conan.io) for dependencies
* Unit tests using [googletest](https://github.com/google/googletest)
* [Crypto++](https://www.cryptopp.com/) as an example dependency library

The sample code here will just read in two strings containing long hex numbers and calculate the xor of them. It uses Crypto++ for decoding and encoding the strings, mainly to demonstrate how to use a separate library. It'll also show how to use googletest for unit tests.

# Mac vs Other Platforms

After writing this, I've realized that this setup _should_ work on pretty much all platforms. The only thing that's platform-specific is the installation of the dev tools, CMake, and Conan. If you install those three (easy on Linux, should be doable on Windows with Microsoft's Visual Studios or some other C++ compiler available), then Step 3 onwards is the same.

# Assumptions

This guide will show you how to set up a C++ development environment, and assumes that you're at least somewhat development with software development and the command line. It won't show you how to write C++ code or use the command line.

# Step 0: Install the Developer Tools

As a prerequisite, you'll need Apple's developer tools or some form of developer tools. There's a thousand sites out there that list how to do, so I'll just point you to [Google](https://www.google.com/search?q=mac+developer+tools+install).

Alternatively, you can install all of the developer tools using [Homebrew](https://brew.sh/). Again, there are a tonne of guides out there on how to do that, and since the goal of this guide is the simplest possible setup I'm not going to cover that. Homebrew does have the benefit of being able to then install cmake, conan, and other tools easily if you choose that route. I'll leave that to you to decide.

# Step 1: Install CMake

Kitware has very conveniently packaged CMake as a binary just like any Mac app. Just go to their [download page](https://cmake.org/download/), grab the binary, and drag it to your Applications folder. You can then use the GUI or command-line version as you'd like.

Note that the command-line commands are not going to be in your path, so to access them you'll have to use the path to CMake.app (for example: `/Applications/CMake.app/Contents/bin/cmake`). If you want to be able to access those easily from the command line without typing that path all of the time, I'd suggest making a few symbolic links:

```bash
pushd /usr/local/bin
for i in /Applications/CMake.app/Contents/bin/*; do sudo ln -s "$i"; done
popd
```

You can also add that folder to your bash `PATH` variable in `.bash_profile` if you'd like, but I find the symbolic links cleaner. Setting up bash aliases also works, but you'll run into problems later with scripts that expect cmake to be available, so I don't recommend that.

As mentioned in the previous section, the other option is to use homebrew to install cmake. If you use homebrew you won't need to deal with the path issue, as they should already be in your path.

# Step 2: Install Conan.io

Conan should be as easy to install as `pip install conan`. You can also install Conan from Homebrew if you prefer.

If you don't have pip installed (I think it might come with Xcode, but I'm not sure). You can follow [pypi's instructions](https://pip.pypa.io/en/stable/installing/). It used to be as easy as `sudo easy_install pip` but that might not work in the future.

# Step 3: Hello World

Ok, so now you have all of the tools installed, lets get the most basic program possible running. We'll write a simple CMake configuration, a Hello World, and then build and run it. I suggest starting by making a folder called for your sources, such as:

    mkdir -p "~/Source/C++ Example" && cd "~/Source/C++ Example"

 Then use your favourite editor to add the following to CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("C++ Example")

SET(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example main.cc)
```

All this does is:

* Give your project a name ("C++ Example"), feel free to change this to any name you like
* Tell CMake that it must be version 3.1 or above, which is really any recent version of CMake
* Set the C++ version to be C++14. I recommend using at least C++14 for any new projects, if not C++17.
* Creates an executable called example that is compiled from main.cc only

Now add a standard Hello World in main.cc:

```C++
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
```

I recommend doing out-of-source builds to make it easier to clean up, so let's make a folder for our builds and then use that to build the project:

```bash
mkdir build && cd build
cmake .. -G "Unix Makefiles"
make
./example
```

Pretty simple so far, right? A few quick notes:

* -G tells cmake to generate for a particular build environment. Use `cmake --help` to get a list of supported options. You can also build for Xcode, Sublime Text, or Eclipse as examples.
* If you change the name of the executable you'll obviously need to run it with your name and not "example"
* You can simply delete the build folder if you want to start again, or make a different build folder for a different build type

# Step 4: Library Skeleton

You'll want the vast majority of your code to be in a library to make it easier to test and avoid recompiling it across targets. Let's create a simple library and write a simple unit test.

First, add `add_library(utils utils.cc)` to your `CMakeLists.txt` to add a library to your project, and then the line `target_link_libraries(example utils)` after the executable so that the library is linked in. It should look like this:

```cmake
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("C++ Quickstart")

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(utils utils.cc)

add_executable(example main.cc)
target_link_libraries(example utils)
```

Now create a simple skeleton for your library. Here's the header `utils.h`:

```C++
#ifndef EXAMPLE_UTILS_H
#define EXAMPLE_UTILS_H

#include <vector>

typedef std::vector<unsigned char> byte_array;

void xor_blocks(const byte_array& b1, const byte_array& b2, byte_array& out);

#endif  // EXAMPLE_UTILS_H
```

And then the empty function in `utils.cc`:

```C++
#include "utils.h"

void xor_blocks(const byte_array& b1, const byte_array& b2, byte_array& out) {}
```

Go back to the build folder and run `make` again. This time you should see it build libutils.

# Step 5: Add googletest

Let's follow TDD and build a unit test for our library before we write the implementation. But there's one catch: we don't have googletest yet. We could download the source and include it in our project, but this is exactly why installed Conan, and as you'll see Conan will make this _very_ easy. [Conan's docs](https://docs.conan.io/) are pretty extensive and easy to read, so head there if you'd like any more details on what I've got here.

First, let's find the latest version of googletest to use. Run `conan search gtest --remote all` on any command line. You should get an output like:

```
Existing package recipes:

Remote 'conan-center':
gtest/1.7.0@bincrafters/stable
gtest/1.8.0@bincrafters/stable
gtest/1.8.1
gtest/1.8.1@bincrafters/stable
gtest/1.10.0
```

Anything with @ and a username/channel is a specific build from a user. I suggest going for the latest version that isn't specified that way. In this case it's `gtest/1.10.0`. If you want to see package details you can use `conan inspect gtest/1.10.0`. Let's add a dependency to gtest to our project. Create a file called `conanfile.txt` to the top level of your project and add the following to it:

```conan
[requires]
gtest/1.10.0

[generators]
cmake
```

All this does is tell Conan:
* We require googltest 1.10.0
* Generate CMake files

We'll have to add a few lines to `CMakeLists.txt` to use this. Here's the updated file:

```cmake
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("C++ Quickstart")

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(utils utils.cc)

add_executable(example main.cc)
target_link_libraries(example utils ${CONAN_LIBS})
```

Notice that all that's changed is:
1. Include Conan's cmake file
2. Call Conan's setup
3. Add the Conan generated libraries to the linking of the executable

Now let's build this. Note that we haven't yet written the unit tests, but we'll get to that in a moment. From your project directory do the following:

```bash
cd build
conan install ..
cmake .. -G "Unix Makefiles"
make
```

You should see Conan download and set up gtest, and then cmake and make execute the build.

# Step 6: Unit Test

Now that we have gtest, let's use it! First, add an executable for the unit test to your `CMakeLists.txt` with the following two sections:

```cmake
enable_testing()

add_executable(UtilsTest utils_test.cc)
target_link_libraries(UtilsTest utils ${CONAN_LIBS})
add_test(UtilsTest bin/UtilsTest)
```

For some reason, when Conan is introduced, it starts putting builds under the `bin/` subfolder. If anyone can shed some light on that, please send me a message.

Now let's write a unit test for this. Personally, I like adding my tests right beside the code itself. It reinforces that tests are part of code. You can put them in a subfolder if you'd like; I'll leave that as an exercise. For now, add the following to `utils_test.cc`.

```C++
#include "utils.h"

#include <gtest/gtest.h>
#include <utility>

using std::pair;
using std::make_pair;
using std::string;
using std::tuple;
using ::testing::TestWithParam;
using ::testing::Values;

namespace testing {

class XorBlocks : public TestWithParam<tuple<byte_array, byte_array, byte_array>> {
protected:
  ~XorBlocks() override {}
  void SetUp() override {}
  void TearDown() override {}
};

TEST_P(XorBlocks, Examples) {
  const auto& b1 = get<0>(GetParam());
  const auto& b2 = get<1>(GetParam());
  const auto& exp = get<2>(GetParam());
  byte_array result;
  result.resize(b1.size());
  xor_blocks(b1, b2, result);
  EXPECT_EQ(result, exp);
}

INSTANTIATE_TEST_SUITE_P(XorBlocksExamples,
                         XorBlocks,
                         testing::Values(
                          make_tuple(byte_array{}, byte_array{}, byte_array{}),
                          make_tuple(byte_array{0x00}, byte_array{0x00}, byte_array{0x00}),
                          make_tuple(byte_array{0xde, 0xad, 0xbe, 0xef},
                            byte_array{0xaa, 0x55, 0xaa, 0x55},
                            byte_array{0x74, 0xf8, 0x14, 0xba})
                          ));

} // namespace testing
```

This uses a parameterized test in gtest. Feel free to add more examples or modify the test, but I'm not going to cover writing tests here. To run the test from your project folder:

```bash
cd build
make && make test
```

You should see something like the following:

```
-- Conan: Adjusting output directories
-- Conan: Using cmake global configuration
-- Conan: Adjusting default RPATHs Conan policies
-- Conan: Adjusting language standard
-- Current conanbuildinfo.cmake directory: /Users/kshehata/Source/Mac-Cpp-Quickstart/build
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/kshehata/Source/Mac-Cpp-Quickstart/build
[ 33%] Built target utils
[ 66%] Built target example
[100%] Built target UtilsTest
Running tests...
Test project /Users/kshehata/Source/Mac-Cpp-Quickstart/build
    Start 1: UtilsTest
1/1 Test #1: UtilsTest ........................***Failed    0.01 sec

0% tests passed, 1 tests failed out of 1

Total Test time (real) =   0.03 sec

The following tests FAILED:
	  1 - UtilsTest (Failed)
Errors while running CTest
make: *** [test] Error 8
```

Notice that CMake's generated Makefiles hide the test details so you can't see why it failed. If you want to see that detail, just run the test directly using `bin/UtilsTest`. Here's what you should see:

```
Running main() from gmock_main.cc
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from XorBlocksExamples/XorBlocks
[ RUN      ] XorBlocksExamples/XorBlocks.Examples/0
[       OK ] XorBlocksExamples/XorBlocks.Examples/0 (0 ms)
[ RUN      ] XorBlocksExamples/XorBlocks.Examples/1
[       OK ] XorBlocksExamples/XorBlocks.Examples/1 (0 ms)
[ RUN      ] XorBlocksExamples/XorBlocks.Examples/2
/Users/kshehata/Source/Mac-Cpp-Quickstart/utils_test.cc:29: Failure
Expected equality of these values:
  result
    Which is: { '\0', '\0', '\0', '\0' }
  exp
    Which is: { 't' (116, 0x74), '\xF8' (248), '\x14' (20), '\xBA' (186) }
[  FAILED  ] XorBlocksExamples/XorBlocks.Examples/2, where GetParam() = ({ '\xDE' (222), '\xAD' (173), '\xBE' (190), '\xEF' (239) }, { '\xAA' (170), 'U' (85, 0x55), '\xAA' (170), 'U' (85, 0x55) }, { 't' (116, 0x74), '\xF8' (248), '\x14' (20), '\xBA' (186) }) (0 ms)
[----------] 3 tests from XorBlocksExamples/XorBlocks (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 2 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] XorBlocksExamples/XorBlocks.Examples/2, where GetParam() = ({ '\xDE' (222), '\xAD' (173), '\xBE' (190), '\xEF' (239) }, { '\xAA' (170), 'U' (85, 0x55), '\xAA' (170), 'U' (85, 0x55) }, { 't' (116, 0x74), '\xF8' (248), '\x14' (20), '\xBA' (186) })

 1 FAILED TEST
```

Woohoo, it failed exactly the say it was supposed to! I'll leave it to you to add the code to `utils.cc` to implement this function, or you can find my solution in this repo.

# Step 7: Crypto++

Now we have a function implemented and tested, great. We can't add it to main yet, because we can't yet parse user input. Sure, we could write our own function for that, but why not use the ones in Crypto++? Conan makes it easy for us to find and use a dependency like this. First, search for the package using `conan search "crypto*" --remote all`. You should see something like:

```
Existing package recipes:

Remote 'conan-center':
cryptopp/5.6.5@bincrafters/stable
cryptopp/7.0.0@bincrafters/stable
cryptopp/8.2.0@bincrafters/stable
```

Looks like the only package available is from the bincrafters channel. Let's add that to conanfile.txt. It should look like:

```conan
[requires]
gtest/1.10.0
cryptopp/8.2.0@bincrafters/stable

[generators]
cmake
```

Now run conan again and rebuild. From you build folder:

```bash
conan install ..
make
```

You should see Conan go out and get Crypto++ and then your project build. Congratulations! You now have Crypto++ ready to use in your project, all you have to do is call it from your code. Here are some steps from here:

1. Add functions to utils to convert strings in hex to byte arrays and vice versa
2. Add unit tests for those function (or better yet: write empty functions, then unit tests, then the implementations)
3. Add code to main to take user input on either the command line or standard input, calculate the xor, and output

You can take those as an exercise, or look at the code in the repo for my solution.

# Final Thoughts

If you've made it this far, hopefully that means that you have a C++ project that uses Crypto++, has unit tests, and builds cleanly in most environments. This is a great starting point for your own development. A few suggestions on next steps:

* You're probably already using git and github, but if not I'd suggest putting even your toy code in it. You never know when you'll need to figure out what you did to make something work.
* You can have CMake generate for your preferred IDE, whether that's Xcode, Sublime Text, Eclipse, or something else. I'd highly recommend using an editor that works for you rather than what other people say is best.
* I've heard good things about Bazel but haven't used it yet. Conan can also work with Bazel and it would probably be just as easy as CMake. If you do use it, please let me know how it works out for you.
* I've avoided Homebrew here as I didn't want to have yet another dependency, but I know lots of people use it. If you use this setup with Homebrew, please let me know. I'd love to hear how it works out and if these instructions work.
* Finally, the code generated here should work on all platforms not just Mac OS X. Only the installation instructions for CMake would be different, the rest is actually the same. If you have access to another platform, please try it out and let me know how it works out.

Thanks and happy coding!
