from scripts import SCRIPTS
from scripts.util import strip_args
import sys
if len(sys.argv) < 2:
    print("No command specified!")
    exit(1)
args = strip_args(2)
try:
    exit(SCRIPTS[sys.argv[1]](args))
except KeyError:
    print("The specified command was not found! Tried to call: \"{}\"".format(sys.argv[1]))
    exit(2)