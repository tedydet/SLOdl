# HashBeans (HABS)

HashBeans is a simplified successor to HOdlcoin: it keeps on-chain interest (~10.26% APR), but removes term deposits/lockups and bonus-era mechanics, to stay usable long-term.

HOdlcoin’s core idea was fun: balances grow via interest, and HODLing is rewarded on-chain.  
In practice, the extreme long-term inflation and “numbers get too big” problem made it hard to sustain and hard to operate (exchanges, explorers, wallets, humans).

**HashBeans keeps the vibe, keeps the fairness, but tries to stay operationally sane.**

> Status: experimental / community project. Expect rough edges.  
> Not investment advice. No guarantees. Use at your own risk.

---

## What is HashBeans?

HashBeans is a Bitcoin-derived cryptocurrency with:

- **Proof-of-Work mining**
- **UTXO model**
- **A fixed block schedule (halvings)**
- **An on-chain interest concept** derived from HOdlcoin.

The goal is to be a lightweight, understandable, CPU-friendly-ish altcoin that’s fun to mine, run, and explore.

---

## Why a successor?

HOdlcoin proved two things at once:

1. **Interest-on-UTXOs is fun and creates a HODL narrative**
2. **If you overdo it, the protocol and economy get weird**
   - exploding supply over time
   - huge values in explorers/wallets
   - exchange integration becomes harder since it is difficult to make an explorer
   - “what is supply?” becomes a confusing question because interest depends on UTXO age distribution

HashBeans is an attempt to keep the idea *without* letting it grow into an unmaintainable monster.

---

## Network parameters (mainnet)

- **Ticker:** HASHB
- **Block time:** 154 seconds
- **P2P port:** 3377
- **RPC port:** 3376
- **Halving interval:** 819,678 blocks (≈ 4 years at 154s)
- **Initial subsidy:** 50 HABS per block
- **Early blocks:** 1 HABS subsidy for the first 100 blocks

### Block reward schedule (simplified)

| Height range | Block reward |
|------------:|-------------:|
| 0 – 99      | 1 HASHB      |
| 100 – 819,677 | 50 HASHB   |
| 819,678 – 1,639,355 | 25 HASHB |
| 1,639,356 – 2,459,033 | 12.5 HASHB |
| …          | halves every interval |

> Exact logic is in `GetBlockSubsidy()` and `consensus.nSubsidyHalvingInterval`.

### Max supply

HashBeans has **no simple “guaranteed max supply”** once interest is considered.

You can compute a **baseline mined supply cap** (the classic halving series) — but **total circulating supply can exceed that** depending on how many UTXOs sit unspent long enough to accumulate interest.

So for websites/exchanges we recommend listing:

- **Mined supply cap (baseline, PoW only):** finite (halving series)
- **Effective supply including interest:** variable / depends on UTXO age distribution

---

## Interest: what it is (and what it is not)

HashBeans includes an on-chain interest concept that was first introduced by HOdlcoin (to the best of our knowledge).

**Important reality check:**
- Interest means supply is **path-dependent** (depends on user behavior / UTXO age).

### How it’s implemented

Interest is calculated on **unspent outputs** (UTXOs) as a function of:
- output amount
- output creation height
- current chain height
- The APR of an UTXO is about 10.26%
Interest is capped by design so outputs don’t grow forever. UTXOs that are older than 1 year don't accumulate interestes anymore meaning that abandoned wallets stop accumulating HABS at some point.

### Practical consequences

- Holding coins in your own wallet can increase your balance over time.
- Leaving coins on an exchange may or may not benefit you depending on how the exchange accounts internally.
- Total supply is not a single clean number unless you define the accounting rules precisely.
- If you do not move your coins at least once a year, you will no longer receive interest.
  
---

## Proof of Work

HashBeans uses a memory-heavy Proof-of-Work design (Pattern Search / 1GB scratchpad family).  
The goal is to keep mining more “participatory” than pure ASIC races.

---
