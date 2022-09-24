"""
auto generate cc_binary for each cpp file
assuming all cpp files here is a stand-alone
"""

[
    cc_binary(
        name = src.split(".")[0],
        srcs = [src],
    )
    for src in glob(["*.cpp"])
]
