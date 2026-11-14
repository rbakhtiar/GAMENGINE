
import ctypes
import os
import platform
from pathlib import Path


def find_library():
    root = Path(__file__).resolve().parents[2]

    system = platform.system()

    names = {
        "Windows": "gamengine_core.dll",
        "Darwin": "libgamengine_core.dylib",
        "Linux": "libgamengine_core.so"
    }

    name = names.get(system)

    if not name:
        raise RuntimeError("Unsupported platform")

    candidates = [
        root / "native" / "build" / name,
        root / "native" / "build" / "Debug" / name,
        root / "native" / "build" / "Release" / name
    ]

    for path in candidates:
        if path.exists():
            return str(path)

    raise FileNotFoundError(
        f"Could not find {name}. Build GAMENGINE first."
    )


_lib = ctypes.CDLL(find_library())

_lib.ge_engine_create.restype = ctypes.c_void_p

_lib.ge_engine_destroy.argtypes = [
    ctypes.c_void_p
]

_lib.ge_object_create.argtypes = [
    ctypes.c_void_p,
    ctypes.c_char_p
]
_lib.ge_object_create.restype = ctypes.c_uint64

_lib.ge_object_count.argtypes = [
    ctypes.c_void_p
]
_lib.ge_object_count.restype = ctypes.c_size_t

_lib.ge_object_id_at.argtypes = [
    ctypes.c_void_p,
    ctypes.c_size_t
]
_lib.ge_object_id_at.restype = ctypes.c_uint64

_lib.ge_object_set_position.argtypes = [
    ctypes.c_void_p,
    ctypes.c_uint64,
    ctypes.c_float,
    ctypes.c_float,
    ctypes.c_float
]
_lib.ge_object_set_position.restype = ctypes.c_int

_lib.ge_object_get_name.argtypes = [
    ctypes.c_void_p,
    ctypes.c_uint64,
    ctypes.c_char_p,
    ctypes.c_size_t
]
_lib.ge_object_get_name.restype = ctypes.c_int

_lib.ge_object_remove.argtypes = [
    ctypes.c_void_p,
    ctypes.c_uint64
]
_lib.ge_object_remove.restype = ctypes.c_int


class Engine:
    def __init__(self):
        self.handle = _lib.ge_engine_create()

        if not self.handle:
            raise RuntimeError("Engine initialization failed")

    def create_object(self, name):
        object_id = _lib.ge_object_create(
            self.handle,
            name.encode("utf-8")
        )

        if not object_id:
            raise RuntimeError("Object creation failed")

        return object_id

    def count(self):
        return _lib.ge_object_count(self.handle)

    def get_name(self, object_id):
        buffer = ctypes.create_string_buffer(256)

        success = _lib.ge_object_get_name(
            self.handle,
            object_id,
            buffer,
            len(buffer)
        )

        if not success:
            return None

        return buffer.value.decode("utf-8")

    def set_position(self, object_id, x, y, z):
        return bool(_lib.ge_object_set_position(
            self.handle,
            object_id,
            x,
            y,
            z
        ))

    def remove(self, object_id):
        return bool(
            _lib.ge_object_remove(self.handle, object_id)
        )

    def close(self):
        if self.handle:
            _lib.ge_engine_destroy(self.handle)
            self.handle = None

    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.close()


if __name__ == "__main__":
    with Engine() as engine:
        player = engine.create_object("Player")

        engine.set_position(player, 10, 0, 5)

        print("GAMENGINE Python Bridge")
        print("Player ID:", player)
        print("Player name:", engine.get_name(player))
        print("Object count:", engine.count())
