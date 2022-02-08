use std::{fs, io, path::PathBuf};

use clap::StructOpt;
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize)]
struct Date {
    month: i32,
    day: i32,
    year: i32,
}

#[derive(Serialize, Deserialize)]
struct Cents(i64);

#[derive(Serialize, Deserialize)]
struct Account {
    name: String,
    id: i64,
    balance: Cents,
}

#[derive(Serialize, Deserialize)]
struct Transaction {
    amount: Cents,
    account_id: i64,
    date: Date,
}

fn interest(t: f64, amount: f64, rate: i32) -> f64 {
    (rate as f64 * t * amount) / 100.0
}

#[derive(clap::Parser, Debug)]
#[clap(author, version, about, long_about = None)]
struct Args {
    #[clap(short = 'L', long, value_name = "FILE")]
    load: Option<PathBuf>,
}

#[derive(Serialize, Deserialize)]
struct State {
    accounts: Vec<Account>,
    transaction_history: Vec<Transaction>,
}

impl State {
    fn new() -> Self {
        Self {
            accounts: Vec::new(),
            transaction_history: Vec::new(),
        }
    }
}

#[derive(Debug, thiserror::Error)]
enum LoadError {
    #[error(transparent)]
    Io(io::Error),
    #[error(transparent)]
    Ron(ron::Error),
}

impl From<io::Error> for LoadError {
    fn from(i: io::Error) -> Self {
        Self::Io(i)
    }
}

fn try_load_file(path: PathBuf) -> Result<State, LoadError> {
    let file = fs::read_to_string(path)?;
    ron::from_str(&file).map_err(LoadError::Ron)
}

fn main() -> Result<(), &'static str> {
    let args = Args::parse();
    let state = match args.load {
        Some(load) => match try_load_file(load) {
            Ok(state) => state,
            Err(e) => {
                eprintln!("loading from file: {}", e);
                return Err("could not load from file");
            }
        },
        None => State::new(),
    };
    Ok(())
}
