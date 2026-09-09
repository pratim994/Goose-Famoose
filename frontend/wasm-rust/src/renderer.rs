use wasm_bindgen::JsCast;
use web_sys::{
    CanvasRenderingContext2d,
    HtmlCanvasElement,
    HtmlImageElement,
};

use crate::game::Game;
use crate::player::Seat;

pub struct Renderer {
    canvas: HtmlCanvasElement,
    context: CanvasRenderingContext2d,
}

impl Renderer {
    pub fn new(canvas: HtmlCanvasElement) -> Self {
        let context = canvas
            .get_context("2d")
            .unwrap()
            .unwrap()
            .dyn_into::<CanvasRenderingContext2d>()
            .unwrap();

        Self {
            canvas,
            context,
        }
    }

    pub fn clear(&self) {
        let width = self.canvas.width() as f64;
        let height = self.canvas.height() as f64;

        self.context.clear_rect(0.0, 0.0, width, height);
    }

    pub fn render(&self, game: &Game) {
        self.clear();

        self.draw_table();
        self.draw_hand(game, Seat::South);
        self.draw_hand(game, Seat::North);
        self.draw_hand(game, Seat::East);
        self.draw_hand(game, Seat::West);
    }

    fn draw_table(&self) {
        let width = self.canvas.width() as f64;
        let height = self.canvas.height() as f64;

        self.context.begin_path();

        self.context
            .ellipse(
                width / 2.0,
                height / 2.0,
                width * 0.35,
                height * 0.35,
                0.0,
                0.0,
                std::f64::consts::PI * 2.0,
            )
            .unwrap();

        self.context.fill();
    }

    fn draw_hand(&self, game: &Game, seat: Seat) {
        let player = &game.players[seat.index()];

        for (index, card) in player.hand.iter().enumerate() {
            let x = self.card_x(seat, index, player.hand.len());
            let y = self.card_y(seat);

            self.draw_card(card.asset_name(), x, y, seat);
        }
    }

    fn card_x(
        &self,
        seat: Seat,
        index: usize,
        count: usize,
    ) -> f64 {
        let width = self.canvas.width() as f64;

        match seat {
            Seat::South | Seat::North => {
                let spacing = 45.0;

                width / 2.0
                    - ((count as f64 - 1.0) * spacing / 2.0)
                    + index as f64 * spacing
            }

            Seat::East | Seat::West => {
                0.0
            }
        }
    }

    fn card_y(&self, seat: Seat) -> f64 {
        let height = self.canvas.height() as f64;

        match seat {
            Seat::South => height - 180.0,
            Seat::North => 40.0,
            Seat::East | Seat::West => height / 2.0,
        }
    }

    fn draw_card(
        &self,
        src: String,
        x: f64,
        y: f64,
        seat: Seat,
    ) {
        let image = HtmlImageElement::new().unwrap();

        image.set_src(&src);

        let context = self.context.clone();

        let rotation = match seat {
            Seat::East => std::f64::consts::PI / 2.0,
            Seat::West => -std::f64::consts::PI / 2.0,
            _ => 0.0,
        };

        let closure = wasm_bindgen::closure::Closure::once(move || {
            context.save();

            context
                .translate(x, y)
                .unwrap();

            context
                .rotate(rotation)
                .unwrap();

            context
                .draw_image_with_html_image_element(
                    &image,
                    0.0,
                    0.0,
                )
                .unwrap();

            context.restore();
        });

        image.set_onload(Some(closure.as_ref().unchecked_ref()));

        closure.forget();
    }
}