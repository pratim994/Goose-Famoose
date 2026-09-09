use crate::card::{Card, Rank, Suit};
use crate::player::{Player, Seat};

pub struct Game {
    pub players: Vec<Player>,
    pub current_turn: Seat,
    pub trick: Vec<(Seat, Card)>,
}

impl Game {
    pub fn new() -> Self {
        let players = vec![
            Player::new(Seat::North),
            Player::new(Seat::East),
            Player::new(Seat::South),
            Player::new(Seat::West),
        ];

        Self {
            players,
            current_turn: Seat::South,
            trick: Vec::new(),
        }
    }

    pub fn deal_test_cards(&mut self) {
        let suits = [
            Suit::Clubs,
            Suit::Diamonds,
            Suit::Hearts,
            Suit::Spades,
        ];

        let ranks = [
            Rank::Two,
            Rank::Three,
            Rank::Four,
            Rank::Five,
            Rank::Six,
            Rank::Seven,
            Rank::Eight,
            Rank::Nine,
            Rank::Ten,
            Rank::Jack,
            Rank::Queen,
            Rank::King,
            Rank::Ace,
        ];

        let mut deck = Vec::new();

        for suit in suits {
            for rank in ranks {
                deck.push(Card::new(suit, rank));
            }
        }

        for (index, card) in deck.into_iter().enumerate() {
            self.players[index % 4].hand.push(card);
        }
    }

    pub fn play_card(&mut self, player: Seat, card_index: usize) -> bool {
        if player != self.current_turn {
            return false;
        }

        let player_index = player.index();

        if card_index >= self.players[player_index].hand.len() {
            return false;
        }

        let card = self.players[player_index].hand.remove(card_index);

        self.trick.push((player, card));

        self.current_turn = match player {
            Seat::North => Seat::East,
            Seat::East => Seat::South,
            Seat::South => Seat::West,
            Seat::West => Seat::North,
        };

        true
    }
}