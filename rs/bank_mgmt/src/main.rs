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
    RmpDecode(rmp_serde::decode::Error),
}

impl From<io::Error> for LoadError {
    fn from(i: io::Error) -> Self {
        Self::Io(i)
    }
}

fn try_load_file(path: PathBuf) -> Result<State, LoadError> {
    let file = fs::read(path)?;
    rmp_serde::from_slice(&file).map_err(LoadError::RmpDecode)
}

fn main() -> Result<(), &'static str> {
    let args = Args::parse();
    let mut state = match args.load {
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
