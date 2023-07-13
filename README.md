# VK Renderer

![GitHub issues](https://img.shields.io/github/issues/AntarticCoder/VK-Renderer)

VK Renderer is a vulkan renderer being built to study computer graphics and understand it better at a lower level.

## How to Build?

To build VK-Renderer, you'll need the following:

* A C++ compiler
* The Vulkan SDK installed on your machine, versions 1.2 and higher
* An open terminal
* A text editor, if you want to edit the code or shaders

Note: Make sure to pull all of the submodules

To build on any unix machine, simply execute my `build.sh` shell script

```bash
sh build.sh
```

If you recieve an error from `glslc` about the output file not existing, then do as follow.

```bash
cd resources/shaders
mkdir compiled
cd compiled
cat > vert.spv 
cat > frag.spv
```

Afterward, run the shell script one more time, and see if you can compile now.

## How to Run?

To run the program, naviagate to main directory then just run in the terminal

```bash
./bin/VK-Renderer
```

## Issues and Contributing

If you run into any issues, or want to contribute please feel free to do so.

For issues please give as much detail to the issue as you can. Things you can provide are:

* Console Logs and Validation Errors
* Screenshots and Screen Captures
* Info in a Debugger
* Renderdoc Snapshot - If you're feeling snazzy

For contributing, I reccommend opening an Issue before opening a PR to discuss what you would like to implement.
This can be helpful because:

* It can stop both of us from developing the same feature
* Can give you more insight on what you should do
* It's nice to get a heads up.

## Conclusion

Thanks for checking out my repo, if you found it to be useful, a star would be great. Thanks!