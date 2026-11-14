
class GAMENGINE:
    VERSION = "0.1.0"

    def __init__(self):
        self.objects = []

    def create_object(self, name):
        obj = {
            "name": name,
            "position": [0.0, 0.0, 0.0]
        }
        self.objects.append(obj)
        return obj

    def remove_object(self, name):
        self.objects = [
            obj for obj in self.objects
            if obj["name"] != name
        ]

    def list_objects(self):
        return self.objects


if __name__ == "__main__":
    engine = GAMENGINE()

    engine.create_object("Player")
    engine.create_object("Environment")

    print("GAMENGINE Python Runtime")
    print("Version:", engine.VERSION)
    print("Objects:", engine.list_objects())
