# WDT LAN Random-Data Benchmark Results

Date: 2026-04-26

Direction: `10.20.1.250 -> 10.20.1.150`

Transport:

- WDT static ports, 8 streams
- WDT encryption disabled: `-encryption_type none`
- SSH used only to pass the WDT receiver URL and collect logs
- Data payloads were generated with random bytes to avoid compression effects

Hosts:

- Sender: `ailocal@10.20.1.250`
- Receiver: `symek@10.20.1.150`

## Summary

| Scenario | Data files | Payload | Ports | Status | Sender time | Wall time | Sender throughput | Receiver throughput |
| --- | ---: | ---: | --- | --- | ---: | ---: | ---: | ---: |
| `10x1GiB-random` | 10 | 10240.00 MiB | `30000-30007` | OK | 9.35 s | 9.37 s | 1095.22 MB/s | 1095.30 MB/s |
| `200x50MiB-random` | 200 | 10000.00 MiB | `31000-31007` | OK | 9.12 s | 9.13 s | 1097.01 MB/s | 1097.15 MB/s |
| `1000-variable-random` | 1000 | 48833.11 MiB | `32000-32007` | OK | 44.65 s | 44.68 s | 1093.67 MB/s | 1093.69 MB/s |
| `104858x100KiB-random` | 104858 | 10246.24 MiB | `29000-29007` | OK | 37.90 s | 38.06 s | 270.42 MB/s | 270.34 MB/s |

## Observations

The large-file and mixed-size random-data tests are effectively line-rate for 10 GbE after protocol overhead. The `10x1GiB`, `200x50MiB`, and `1000-variable` cases all clustered around `1094-1097 MB/s`.

The `104858x100KiB` case is substantially slower at about `270 MB/s`. This is the first test where file-count and metadata operations dominate enough to move the result far below interface throughput.

The previous zero-filled tests were not affected by WDT compression because these runs used no WDT compression and `-encryption_type none`, but random payloads are the better baseline for later SSH, tunnel, IPsec, storage, or filesystem tests.

## Verification

`10x1GiB-random`:

- Source: `11G`, `10` files
- Destination: `11G`, `10` files
- WDT status: `OK`

`200x50MiB-random`:

- Source: `9.8G`, `200` files
- Destination: `9.8G`, `200` files
- WDT status: `OK`

`1000-variable-random`:

- Source: `48G`, `1000` files
- Destination: `48G`, `1000` files
- WDT status: `OK`

`104858x100KiB-random`:

- Source: `11G`, `104858` files
- Destination: `11G`, `104858` files
- WDT status: `OK`

## Notes

The reverse direction, `10.20.1.150 -> 10.20.1.250`, previously failed on WDT data ports with `No route to host` while ICMP worked. The successful benchmark direction was therefore `10.20.1.250 -> 10.20.1.150`.

