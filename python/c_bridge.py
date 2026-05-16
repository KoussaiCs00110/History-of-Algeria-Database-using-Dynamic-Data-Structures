"""
This file is the small bridge between Python and the shared C library.
It loads the exported C functions once and wraps them in Python helpers
so the Flask routes can use normal Python values.
"""

import ctypes
import json
import os
import sys