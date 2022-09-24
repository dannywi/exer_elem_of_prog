# exer_elem_of_prog

Training exercises. Come and check regularly to sharpen your knife!

The bazel BUILD file assumes all `*.cpp` files are single stand-alone file that can be compiled into an executable. If this ceases to be true, segregate files like these to a folder and have the BUILD file there.

```
# BUILD ALL
bazel build //:all

# RUN ONE FILE (e.g. LinkedList.cpp)
bazel build //:LinkedList
```