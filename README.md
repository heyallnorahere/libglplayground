# libglplayground [![License](https://img.shields.io/github/license/yodasoda1219/libglplayground)](https://github.com/yodasoda1219/libglplayground/blob/main/LICENSE) ![GitHub Repo size](https://img.shields.io/github/repo-size/yodasoda1219/libglplayground) ![LGTM Alerts](https://img.shields.io/lgtm/alerts/github/yodasoda1219/libglplayground) ![LGTM Grade](https://img.shields.io/lgtm/grade/cpp/github/yodasoda1219/libglplayground)

## About

This is a framework for writing OpenGL applications without the hassle of setting up contexts and all of that.

### Branch: model-loading

This branch is for development of the `libplayground::gl::model` class. It will be merged into main when basic loading and skeletal animation is finished.

## Building

This project currently runs on [CMake](https://cmake.org). Once I get a release ready, I will create a [Premake](https://premake.github.io) configuration.

### Build status

| System  | Status |
| ---     | ---    |
| Ubuntu  | [![build-ubuntu](https://img.shields.io/github/workflow/status/yodasoda1219/libglplayground/build-ubuntu)](https://github.com/yodasoda1219/libglplayground/actions/workflows/build-ubuntu.yml)    |
| Windows | [![build-windows](https://img.shields.io/github/workflow/status/yodasoda1219/libglplayground/build-windows)](https://github.com/yodasoda1219/libglplayground/actions/workflows/build-windows.yml) |
| MacOS   | [![build-macosx](https://img.shields.io/github/workflow/status/yodasoda1219/libglplayground/build-macosx)](https://github.com/yodasoda1219/libglplayground/actions/workflows/build-macosx.yml)    |

## How do I use this?

See the [examples folder](examples/).