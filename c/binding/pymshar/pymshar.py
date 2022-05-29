#!/usr/bin/env python3
import ctypes, os, platform

def str_array_to_c_style_str_array(yourstringarray):
    """
    Convert a string array to a char array
    """
    # get bytes
    strbyte = []
    for s in range(len(yourstringarray)):
        strbyte.append(bytes(yourstringarray[s], 'utf-8'))

    cstrarr = (ctypes.c_char_p * (len(yourstringarray)+1))()

    cstrarr[:-1] = strbyte

    return cstrarr

libfile=""
libmshar = None

# check if platform is windows
if platform.system() == "Windows":
    # check if 64 bit
    if platform.architecture()[0] == "64bit":
        libfile = "libmshar64.dll"
    else:
        libfile = "libmshar.dll"
    libmshar = ctypes.WinDLL(os.path.join(os.path.dirname(__file__), libfile))
else:
    if platform.architecture()[0] == "64bit":
        libfile = "libmshar64.so"
    else:
        libfile = "libmshar.so"
    libmshar=ctypes.CDLL(os.path.join(os.path.dirname(__file__), libfile))

libmshar.mkmshar.restype = ctypes.c_char_p
libmshar.mkmshar_x.restype = ctypes.c_char_p
libmshar.mkmshar_s.restype = ctypes.c_char_p

def mkmshar(prescript, postscript, files, errors):
    """
    Make a mshar file
    """
    # convert to ctypes
    cfiles = str_array_to_c_style_str_array(files)

    # call libmshar
    return libmshar.mkmshar(prescript, postscript, cfiles, len(files), errors)

def mkmshar_x(prescript, postscript, files):
    """
    Make a mshar file, always return on error
    """
    # convert to ctypes
    cfiles = str_array_to_c_style_str_array(files)

    # call libmshar
    return libmshar.mkmshar_x(prescript, postscript, cfiles, len(files))

def mkmshar_s(prescript, postscript, files):
    """
    Make a mshar file, ignore errors.
    """
    # convert to ctypes
    cfiles = str_array_to_c_style_str_array(files)

    # call libmshar
    return libmshar.mkmshar_s(prescript, postscript, cfiles, len(files))