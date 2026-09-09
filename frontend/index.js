import init, { BridgeGame } from "./pkg/bridge_game.js";

async function main() {
    await init();

    const game = new BridgeGame("game-canvas");

    document.getElementById("loading").remove();

    window.bridgeGame = game;

    console.log("Bridge game initialized");
}

main().catch((error) => {
    console.error("Failed to start game:", error);
});