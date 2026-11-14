
using System;
using System.Runtime.InteropServices;

namespace GAMENGINE
{
    public sealed class Engine : IDisposable
    {
        private const string Library = "gamengine_core";

        [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ge_engine_create();

        [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
        private static extern void ge_engine_destroy(IntPtr engine);

        [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
        private static extern ulong ge_object_create(
            IntPtr engine,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string name
        );

        [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
        private static extern UIntPtr ge_object_count(IntPtr engine);

        [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ge_object_remove(
            IntPtr engine,
            ulong id
        );

        [DllImport(Library, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ge_object_set_position(
            IntPtr engine,
            ulong id,
            float x,
            float y,
            float z
        );

        private IntPtr handle;

        public Engine()
        {
            handle = ge_engine_create();

            if (handle == IntPtr.Zero)
                throw new InvalidOperationException(
                    "GAMENGINE initialization failed"
                );
        }

        public ulong CreateObject(string name)
        {
            ulong id = ge_object_create(handle, name);

            if (id == 0)
                throw new InvalidOperationException(
                    "Object creation failed"
                );

            return id;
        }

        public ulong Count()
        {
            return ge_object_count(handle).ToUInt64();
        }

        public bool SetPosition(
            ulong id,
            float x,
            float y,
            float z
        )
        {
            return ge_object_set_position(
                handle, id, x, y, z
            ) == 1;
        }

        public bool Remove(ulong id)
        {
            return ge_object_remove(handle, id) == 1;
        }

        public void Dispose()
        {
            if (handle != IntPtr.Zero)
            {
                ge_engine_destroy(handle);
                handle = IntPtr.Zero;
            }

            GC.SuppressFinalize(this);
        }
    }
}
