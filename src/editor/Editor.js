
export class Editor {
  constructor(engine) {
    this.engine = engine;

    this.objectList = document.getElementById("object-list");
    this.inspector = document.getElementById("inspector-content");

    document.getElementById("add-object").onclick = () => {
      this.engine.scene.addCube(
        `Cube ${this.engine.scene.objects.length + 1}`
      );

      this.refresh();
    };

    document.getElementById("play-button").onclick = event => {
      this.engine.playing = !this.engine.playing;

      event.target.textContent = this.engine.playing
        ? "■ Stop"
        : "▶ Play";

      this.engine.setPlaying(this.engine.playing);
    };

    document.getElementById("new-scene").onclick = () => {
      this.engine.scene.clear();
      this.refresh();
      this.engine.setStatus("New scene created");
    };

    window.addEventListener(
      "engine:selection",
      () => this.refresh()
    );

    this.refresh();
  }

  refresh() {
    this.renderHierarchy();
    this.renderInspector();
  }

  renderHierarchy() {
    this.objectList.innerHTML = "";

    for (const object of this.engine.scene.objects) {
      const button = document.createElement("button");

      button.className = "object-item";

      if (object === this.engine.scene.selected) {
        button.classList.add("selected");
      }

      button.textContent = "◇  " + object.name;

      button.onclick = () => {
        this.engine.scene.select(object);
        this.refresh();
      };

      this.objectList.appendChild(button);
    }
  }

  renderInspector() {
    const object = this.engine.scene.selected;

    if (!object) {
      this.inspector.innerHTML =
        '<p class="muted">Select an object to inspect it.</p>';
      return;
    }

    this.inspector.innerHTML = "";

    const heading = document.createElement("h3");
    heading.textContent = object.name;
    this.inspector.appendChild(heading);

    this.addField("Name", object.name, value => {
      object.name = value;
      this.refresh();
    }, "text");

    for (const axis of ["x", "y", "z"]) {
      this.addField(
        `Position ${axis.toUpperCase()}`,
        object.position[axis],
        value => {
          const number = Number(value);

          if (Number.isFinite(number)) {
            object.position[axis] = number;
          }
        },
        "number"
      );
    }

    const remove = document.createElement("button");
    remove.textContent = "Delete Object";
    remove.style.marginTop = "15px";

    remove.onclick = () => {
      this.engine.scene.removeSelected();
      this.refresh();
    };

    this.inspector.appendChild(remove);
  }

  addField(label, value, callback, type) {
    const wrapper = document.createElement("div");
    wrapper.className = "inspector-field";

    const title = document.createElement("label");
    title.textContent = label;

    const input = document.createElement("input");
    input.type = type;
    input.value = value;

    input.addEventListener("change", () => {
      callback(input.value);
    });

    wrapper.append(title, input);
    this.inspector.appendChild(wrapper);
  }
}
