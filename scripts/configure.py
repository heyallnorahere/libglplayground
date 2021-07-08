import scripts.util as util
import subprocess
import os, os.path
def cmake(build_type: str, args: list[str]):
    cwd = os.getcwd()
    cmake_args = [
        "cmake",
        os.getcwd(),
        "-B",
        os.path.join(cwd, "build"),
        "-DCMAKE_BUILD_TYPE=" + build_type
    ]
    for arg in args:
        cmake_args.append(arg)
    return subprocess.call(cmake_args)
def run(args: list[str]):
    if len(args) < 1:
        print("Usage: $PYTHON_COMMAND -m scripts configure <build system> [<build type>] [<option>...]")
        return 3
    build_system = args[0]
    build_type = "Release"
    if len(args) >= 2:
        build_type = args[1]
    configure_args = util.strip_args(2, args)
    build_systems = {
        "cmake": util.Data(function=cmake, name="CMake")
        # todo: add the promised premake configuration
    }
    try:
        build_system_data = build_systems[build_system]
    except KeyError:
        print("The selected build system is not available!")
        return 4
    print("Configuring {} projects".format(build_system_data.name))
    print("Build type: {}".format(build_type))
    return build_system_data.function(build_type, configure_args)