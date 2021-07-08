import subprocess
import yaml
class PackageSpec:
    def __init__(self, packages: list[str]):
        self.packages = packages
    def install(self, command: list[str]):
        args = command.copy()
        for package in self.packages:
            args.append(package)
        subprocess.call(args)
class PackageSpecList:
    def __init__(self, path: str):
        file = open(path, "r")
        self.data = yaml.safe_load(file)
        file.close()
    def get(self, platform: str):
        try:
            package_list = list[str](self.data[platform])
        except KeyError:
            package_list = list[str]()
        return PackageSpec(package_list)
spec_list = PackageSpecList("dependencies.yml")
ARCH_DEPENDENCIES = spec_list.get("Arch-Linux")
UBUNTU_DEPENDENCIES = spec_list.get("Ubuntu")
# todo: add more keys