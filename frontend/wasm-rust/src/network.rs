use wasm_bindgen::prelude::*;
use web_sys::WebSocket;

pub struct Network {
    socket: WebSocket,
}

impl Network {
    pub fn connect(url: &str) -> Result<Self, JsValue> {
        let socket = WebSocket::new(url)?;

        Ok(Self { socket })
    }

    pub fn send(&self, message: &str) -> Result<(), JsValue> {
        self.socket.send_with_str(message)
    }
}