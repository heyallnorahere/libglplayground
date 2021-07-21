import scripts.util as util
import subprocess
import os, os.path
import yaml
class Option:
    def __init__(self, data):
        self.name = str(data["name"])
        self.value = str(data["value"])
    def serialize(self):
        return "-D" + self.name + "=" + self.value
class OptionList:
    def __init__(self, options: list):
        self.options = list[Option]()
        for option in options:
            self.options.append(Option(option))
    def serialize(self):
        option_list = list[str]()
        for option in self.options:
            option_list.append(option.serialize())
        return option_list
def cmake(build_type: str, args: list[str]):
    cwd = os.getcwd()
    cmake_args = [
        "cmake",
        os.getcwd(),
        "-B",
        os.path.join(cwd, "build"),
        "-DCMAKE_BUILD_TYPE=" + build_type
    ]
    options = None
    with open("cmake-options.yml", "r") as stream:
        try:
            options = dict[str](yaml.safe_load(stream))
            stream.close()
        except:
            pass # "options" is already set to null
    if options != None:
        if "universal" in options:
            option_list = OptionList(options["universal"])
            serialized = option_list.serialize()
            for option in serialized:
                cmake_args.append(option)
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