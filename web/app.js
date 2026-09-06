
import createBridgeModule from "./bridge_client.js";

const canvas = document.getElementById("bridge-canvas");

const Module = await createBRidgeModule({
		canvas
});

Module.initialize()'


let previousTime = performance.now();

function frame(time ) {
	const delta = (time-previousTime)/1000;

	previousTime = 	time;

	Module.update(delta);
	Module.render();

	requestAnimationFrame(frame);
}


requestAnimationFrame(frame);


canvas.addEventListener(
    "click",
    event => {

        const rect =
            canvas.getBoundingClientRect();

        const x =
            event.clientX - rect.left;

        const y =
            event.clientY - rect.top;

        Module.mouseClick(x, y);
    }
);
