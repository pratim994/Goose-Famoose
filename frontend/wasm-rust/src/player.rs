use crate::card::Card;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Seat {
    North,
    East,
    South,
    West,
}

impl Seat {
    pub fn index(&self) -> usize {
        match self {
            Seat::North => 0,
            Seat::East => 1,
            Seat::South => 2,
            Seat::West => 3,
        }
    }
}

pub struct Player {
    pub seat: Seat,
    pub hand: Vec<Card>,
}

impl Player {
    pub fn new(seat: Seat) -> Self {
        Self {
            seat,
            hand: Vec::new(),
        }
    }
}