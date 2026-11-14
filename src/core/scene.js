
export class Scene {
  constructor(THREE) {
    this.THREE = THREE;
    this.world = new THREE.Scene();
    this.world.background = new THREE.Color("#0a1710");

    this.objects = [];
    this.selected = null;

    this.world.add(
      new THREE.GridHelper(100, 100, 0x315a40, 0x1b3023)
    );

    const ambient = new THREE.AmbientLight(0xffffff, 1.5);
    this.world.add(ambient);

    const sunlight = new THREE.DirectionalLight(0xffffff, 3);
    sunlight.position.set(5, 10, 7);
    this.world.add(sunlight);

    this.addCube("Cube");
  }

  addCube(name = "Cube") {
    const geometry = new this.THREE.BoxGeometry(1, 1, 1);

    const material = new this.THREE.MeshStandardMaterial({
      color: 0x16865a,
      roughness: 0.45,
      metalness: 0.25
    });

    const cube = new this.THREE.Mesh(geometry, material);

    cube.name = name;
    cube.position.set(
      this.objects.length * 1.5,
      0.5,
      0
    );

    this.world.add(cube);
    this.objects.push(cube);

    this.select(cube);

    return cube;
  }

  select(object) {
    this.selected = object;
  }

  removeSelected() {
    if (!this.selected) return;

    this.world.remove(this.selected);

    this.selected.geometry.dispose();
    this.selected.material.dispose();

    this.objects = this.objects.filter(
      object => object !== this.selected
    );

    this.selected = null;
  }

  clear() {
    for (const object of [...this.objects]) {
      this.world.remove(object);
      object.geometry.dispose();
      object.material.dispose();
    }

    this.objects = [];
    this.selected = null;
  }
}
