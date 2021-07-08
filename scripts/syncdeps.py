import scripts.util as util
from scripts.dependencies import *
import subprocess
class PlatformPkgConfig:
    def __init__(self, install_package_command: str, spec: PackageSpec):
        self.command = install_package_command.split(" ")
        self.spec = spec
    def install(self):
        self.spec.install(self.command)
pkg_config = {
    "Arch-Linux": PlatformPkgConfig("sudo pacman -Syq", ARCH_DEPENDENCIES),
    "Ubuntu": PlatformPkgConfig("sudo apt-get install", UBUNTU_DEPENDENCIES)
}
def run(args: list[str]):
    print("This script may ask for root permissions at times. This script is simply an automation, so feel free to install these packages on your own.")
    platform_name = util.get_platform_name()
    print("Syncing submodules...")
    subprocess.call(["git", "submodule", "update", "--init", "--recursive"])
    print("Finished syncing submodules!")
    print("Syncing installable dependencies for platform: {}".format(platform_name))
    if platform_name == "Ubuntu":
        subprocess.call(["sudo", "apt-get", "update"])
    try:
        config = pkg_config[platform_name]
        if len(config.spec.packages) == 0:
            raise KeyError
        config.install()
    except KeyError:
        print("No installable packages for this platform - skipping")
    print("Finished syncing packages!")
    return 0
