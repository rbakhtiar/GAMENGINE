
package gamengine;

public final class GAMENGINE implements AutoCloseable {

    static {
        System.loadLibrary("gamengine_java");
    }

    private long handle;

    private native long nativeCreate();
    private native void nativeDestroy(long handle);
    private native long nativeCreateObject(long handle, String name);
    private native long nativeCount(long handle);
    private native boolean nativeSetPosition(
        long handle,
        long id,
        float x,
        float y,
        float z
    );
    private native boolean nativeRemove(long handle, long id);

    public GAMENGINE() {
        handle = nativeCreate();

        if (handle == 0) {
            throw new IllegalStateException(
                "GAMENGINE initialization failed"
            );
        }
    }

    public long createObject(String name) {
        long id = nativeCreateObject(handle, name);

        if (id == 0) {
            throw new IllegalStateException(
                "Object creation failed"
            );
        }

        return id;
    }

    public long count() {
        return nativeCount(handle);
    }

    public boolean setPosition(
        long id, float x, float y, float z
    ) {
        return nativeSetPosition(handle, id, x, y, z);
    }

    public boolean remove(long id) {
        return nativeRemove(handle, id);
    }

    @Override
    public void close() {
        if (handle != 0) {
            nativeDestroy(handle);
            handle = 0;
        }
    }

    public static void main(String[] args) {
        try (GAMENGINE engine = new GAMENGINE()) {
            long player = engine.createObject("Player");

            engine.setPosition(player, 10, 0, 5);

            System.out.println("GAMENGINE Java Bridge");
            System.out.println("Player ID: " + player);
            System.out.println("Objects: " + engine.count());
        }
    }
}
