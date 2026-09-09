mod card;
mod gamestate;
mod player;
mod renderer;

use std::cell::RefCell;
use std::rc::Rc;

use wasm_bindgen::prelude::*;
use wasm_bindgen::JsCast;

use web_sys::{
    HtmlCanvasElement,
    Window,
};

use gamestate::Game;
use renderer::Renderer;

#[wasm_bindgen]
pub struct BridgeGame {
    game: Rc<RefCell<Game>>,
    renderer: Rc<Renderer>,
}

#[wasm_bindgen]
impl BridgeGame {
    #[wasm_bindgen(constructor)]
    pub fn new(canvas_id: &str) -> Result<BridgeGame, JsValue> {
        let window: Window =
            web_sys::window().unwrap();

        let document =
            window.document().unwrap();

        let canvas = document
            .get_element_by_id(canvas_id)
            .ok_or_else(|| JsValue::from_str("Canvas not found"))?
            .dyn_into::<HtmlCanvasElement>()?;

        let renderer = Renderer::new(canvas);

        let mut game = Game::new();

        game.deal_test_cards();

        let game = Rc::new(RefCell::new(game));
        let renderer = Rc::new(renderer);

        renderer.render(&game.borrow());

        Ok(Self {
            game,
            renderer,
        })
    }

    pub fn render(&self) {
        self.renderer.render(&self.game.borrow());
    }

    pub fn play_card(
        &self,
        seat: usize,
        card_index: usize,
    ) -> bool {
        let seat = match seat {
            0 => player::Seat::North,
            1 => player::Seat::East,
            2 => player::Seat::South,
            3 => player::Seat::West,
            _ => return false,
        };

        let result = self
            .game
            .borrow_mut()
            .play_card(seat, card_index);

        self.render();

        result
    }
}