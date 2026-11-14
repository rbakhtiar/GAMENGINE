
export class Renderer {
  constructor(THREE, container, scene) {
    this.THREE = THREE;
    this.scene = scene;
    this.container = container;

    this.camera = new THREE.PerspectiveCamera(
      60,
      1,
      0.1,
      1000
    );

    this.camera.position.set(5, 5, 8);
    this.camera.lookAt(0, 0, 0);

    this.renderer = new THREE.WebGLRenderer({
      antialias: true,
      powerPreference: "high-performance"
    });

    this.renderer.setPixelRatio(
      Math.min(window.devicePixelRatio || 1, 1.5)
    );

    this.renderer.shadowMap.enabled = false;

    container.appendChild(this.renderer.domElement);

    this.resize();

    this.resizeObserver = new ResizeObserver(
      () => this.resize()
    );

    this.resizeObserver.observe(container);

    this.raycaster = new THREE.Raycaster();
    this.pointer = new THREE.Vector2();

    this.renderer.domElement.addEventListener(
      "pointerdown",
      event => this.handleSelection(event)
    );

    this.running = false;
    this.animate = this.animate.bind(this);
  }

  resize() {
    const width = this.container.clientWidth;
    const height = this.container.clientHeight;

    if (!width || !height) return;

    this.camera.aspect = width / height;
    this.camera.updateProjectionMatrix();

    this.renderer.setSize(width, height, false);
  }

  handleSelection(event) {
    const rect = this.renderer.domElement.getBoundingClientRect();

    this.pointer.x =
      ((event.clientX - rect.left) / rect.width) * 2 - 1;

    this.pointer.y =
      -((event.clientY - rect.top) / rect.height) * 2 + 1;

    this.raycaster.setFromCamera(
      this.pointer,
      this.camera
    );

    const hits = this.raycaster.intersectObjects(
      this.scene.objects,
      false
    );

    if (hits.length > 0) {
      this.scene.select(hits[0].object);

      window.dispatchEvent(
        new CustomEvent("engine:selection")
      );
    }
  }

  start() {
    if (this.running) return;

    this.running = true;
    this.animate();
  }

  stop() {
    this.running = false;
  }

  animate() {
    if (!this.running) return;

    requestAnimationFrame(this.animate);

    this.renderer.render(
      this.scene.world,
      this.camera
    );
  }

  destroy() {
    this.stop();
    this.resizeObserver.disconnect();
    this.renderer.dispose();
    this.renderer.domElement.remove();
  }
}
