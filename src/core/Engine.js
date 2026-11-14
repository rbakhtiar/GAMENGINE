
import * as THREE from "https://cdn.jsdelivr.net/npm/three@0.180.0/build/three.module.js";

import { Scene } from "./Scene.js";
import { Input } from "./Input.js";
import { Renderer } from "../renderer/Renderer.js";
import { Editor } from "../editor/Editor.js";

export class Engine {
  constructor() {
    this.name = "GAMENGINE";
    this.version = "0.1.0";
    this.playing = false;

    this.scene = new Scene(THREE);
    this.input = new Input();

    this.renderer = new Renderer(
      THREE,
      document.getElementById("viewport"),
      this.scene
    );

    this.editor = new Editor(this);
  }

  start() {
    this.renderer.start();
    this.setStatus("GAMENGINE initialized successfully");

    console.log(
      `${this.name} ${this.version} initialized`
    );
  }

  setPlaying(value) {
    this.playing = value;

    this.setStatus(
      value ? "Game running" : "Game stopped"
    );
  }

  setStatus(message) {
    document.getElementById("status-message").textContent =
      message;
  }

  destroy() {
    this.renderer.destroy();
  }
}
