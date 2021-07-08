import sys
import platform
import subprocess
def strip_args(offset_index: int, args: list[str] = None):
    if args == None:
        args = sys.argv
    new_args = list[str]()
    if len(args) > offset_index:
        for index in range(offset_index, len(args)):
            new_args.append(args[index])
    return new_args
def get_platform_name():
    verbose_platform_name = platform.system()
    if verbose_platform_name == "Linux":
        process = subprocess.Popen(["uname", "-a"], stdout=subprocess.PIPE)
        output = process.communicate()
        verbose_platform_name = output[0].decode("utf-8")
    platform_names = {
        "-arch": "Arch-Linux",
        "-Ubuntu": "Ubuntu",
        "Darwin": "MacOS",
        "Windows": "Windows"
        # todo: add more later
    }
    for keyword in platform_names:
        if keyword in verbose_platform_name:
            return platform_names[keyword]
    return "Unknown"