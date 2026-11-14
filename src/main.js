
import { Engine } from "./core/Engine.js";

const engine = new Engine();

engine.start();

document.getElementById("engine-status").textContent =
  "ENGINE ONLINE";

window.GAMENGINE = engine;
