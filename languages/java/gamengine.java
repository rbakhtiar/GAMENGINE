
package gamengine;

import java.util.ArrayList;
import java.util.List;

public class Gamengine {

    public static final String VERSION = "0.1.0";

    public static class GameObject {
        public String name;
        public float x, y, z;

        public GameObject(String name) {
            this.name = name;
            this.x = 0;
            this.y = 0;
            this.z = 0;
        }
    }

    private final List<GameObject> objects =
        new ArrayList<>();

    public GameObject createObject(String name) {
        GameObject object = new GameObject(name);
        objects.add(object);
        return object;
    }

    public List<GameObject> getObjects() {
        return List.copyOf(objects);
    }

    public boolean removeObject(GameObject object) {
        return objects.remove(object);
    }

    public static void main(String[] args) {
        Gamengine engine = new Gamengine();

        engine.createObject("Player");
        engine.createObject("Environment");

        System.out.println("GAMENGINE " + VERSION);
        System.out.println(
            "Objects: " + engine.getObjects().size()
        );
    }
}
