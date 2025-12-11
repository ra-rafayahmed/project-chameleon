# project-chameleon
A university project that simulates how OSINT tools gather and organize publicly available data. Uses synthetic info only. Includes modules for fake data collection, basic analysis, compression, and DSA-based storage.

This project is a university assignment that focuses on simulating an OSINT-style data collection system in a safe and controlled way. The idea is to show how attackers might gather public information from online sources and combine it with automated data from an app to create a digital profile — all using synthetic (fake) data for safety.

The OSINT module is the main part of the project. It mimics how public information could be collected through:

basic web-scraping techniques

reading publicly available details

generating fake profiles, posts, locations, and timestamps

simulating how data might be collected over time

This OSINT-like data is not real — the system creates it using random generators to keep everything safe and ethical.

The Unity app adds a second layer by producing more synthetic events (like mock device activity or fake location points). All this data is then processed through:

simple compression

mock encryption

storage and organization using data structures and algorithms

The goal is to help students understand how OSINT-based data gathering and threat systems work, without using real personal information or performing any harmful actions.

```
COLLABORATORS:
-> Rafay Ahmed
-> Muhammad Abdurrehman
-> Hadia Noor
-> Umme Aimen
```
```
/project-root
│
├── osint_module/
│
├── malware_simulation/
│
├── unity_simulator/
│
├── dsa_pipeline/
│   ├── compression/        # Huffman, encoding tests
│   ├── encryption/         # mock encryption
│   ├── indexing/           # trees, hashing, sorting
│   └── database/           # SQLite / JSON storage
│
├── analysis/
│   ├── notebooks/
│   ├── plots/
│   └── reports/
│   ├── README_osint.md
│   ├── README_simulation.md
│   └── architecture_diagram.png
│
└── README.md
```
