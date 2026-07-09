# README

## The Build Sequence (Under the Hood)
When you trigger a build, here is the sequence of events CMake executes:
- **The Master Dispatch:** CMake starts at the root `workspace/CMakeLists.txt`. It applies the global C++17 standard to everything, then reads `add_subdirectory(HelloWorld)` and hands off control to that specific folder.
- **Source Gathering:** Inside `HelloWorld/`, the local CMake file hits `file(GLOB SOURCES "src/*.cpp")`. It scans the `src/` directory and grabs `main.cpp` (and any other `.cpp` files you add later).
- **The Header Link:** When it hits `target_include_directories(HelloWorld PRIVATE inc)`, it tells the compiler, "Whenever a `.cpp` file asks to `#include "something.h"`, look inside the `inc/` folder for it." This is what allows you to separate your declarations (headers) from your logic (source).
- **Compilation & Routing:** The compiler generates the raw binary inside the messy `build/` folder. Finally, `RUNTIME_OUTPUT_DIRECTORY` acts as a delivery mechanism, grabbing the finished HelloWorld executable and dropping it cleanly back into your `workspace/HelloWorld/` folder.

## Practical Workflow
**Step 1:** The Initial Setup (Do this once)
- From your root workspace/ directory:
```bash
mkdir build
cd build
cmake ..
```

**Step 2:** The Iterative Loop (Do this all day)
- Now, you write your C++ code.
- E.g. You create `inc/MyClass.h` and `src/MyClass.cpp`, and update your `main.cpp`. When you are ready to test it, you don't need to run cmake again. Just stay inside the build/ directory and run:
```bash
make
```

**Step 3:** Running Your Code
You can execute it straight from the terminal to see your output:
```bash
../HelloWorld/HelloWorld
```

---

## To Debug your C++ Code
1. Go to run & debug section in the VS Code
2. Create a Debug Configuration file (a dynamic config file)
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Current Active Subfolder",
            "type": "cppdbg",
            "request": "launch",
            // Dynamically points to: /workspace/relative_folder_path/relative_folder_name
            "program": "${workspaceFolder}/${relativeFileDirname}/${relativeFileDirname}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}/${relativeFileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```
3. Modify your code in any sub-directory
4. Recompile manually in your terminal:
```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

```
3. Set you break point.
4. Make sure main.cpp is your active tab open on the screen.
5. Press `F5`

---