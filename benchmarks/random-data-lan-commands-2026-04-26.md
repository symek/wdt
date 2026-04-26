# WDT LAN Random-Data Benchmark Commands

Date: 2026-04-26

These commands reproduce the random-data tests from `10.20.1.250` to `10.20.1.150`.

## Common Receiver/Sender Pattern

Receiver host: `symek@10.20.1.150`

Sender host: `ailocal@10.20.1.250`

Receiver binary:

```bash
/tmp/wdt-bench/cmake-build-bench/_bin/wdt/wdt
```

Sender binary:

```bash
/tmp/wdt-bench-runtime/bin/wdt
```

Receiver `LD_LIBRARY_PATH`:

```bash
/tmp/wdt-bench/cmake-build-bench:/tmp/wdt-bench/cmake-build-bench/_deps/glog-build:/tmp/wdt-bench/cmake-build-bench/_deps/gflags-build:/tmp/wdt-bench/cmake-build-bench/_deps/boringssl-build
```

Sender `LD_LIBRARY_PATH`:

```bash
/tmp/wdt-bench-runtime/lib
```

Run shape, replacing paths and port range per scenario:

```bash
ssh -F /dev/null -o BatchMode=yes -o ConnectTimeout=8 -o StrictHostKeyChecking=no symek@10.20.1.150 \
'LD_LIBRARY_PATH=/tmp/wdt-bench/cmake-build-bench:/tmp/wdt-bench/cmake-build-bench/_deps/glog-build:/tmp/wdt-bench/cmake-build-bench/_deps/gflags-build:/tmp/wdt-bench/cmake-build-bench/_deps/boringssl-build /tmp/wdt-bench/cmake-build-bench/_bin/wdt/wdt -directory RECEIVER_DIR -hostname 10.20.1.150 -static_ports=true -start_port START_PORT -num_ports 8 -encryption_type none -progress_report_interval_millis 1000 -full_reporting=true 2>RECEIVER_LOG' \
| ssh -F /dev/null -o BatchMode=yes -o ConnectTimeout=8 -o StrictHostKeyChecking=no ailocal@10.20.1.250 \
'LD_LIBRARY_PATH=/tmp/wdt-bench-runtime/lib /usr/bin/time -p /tmp/wdt-bench-runtime/bin/wdt -directory SENDER_DIR - -encryption_type none -progress_report_interval_millis 1000 -full_reporting=true >SENDER_OUT 2>SENDER_LOG'
```

## Generate `10x1GiB-random`

```bash
ssh -F /dev/null -o BatchMode=yes -o ConnectTimeout=8 -o StrictHostKeyChecking=no ailocal@10.20.1.250 'rm -rf /tmp/wdt-bench-src-10x1g-random; mkdir -p /tmp/wdt-bench-src-10x1g-random; python3 - <<PY
import os
root = "/tmp/wdt-bench-src-10x1g-random"
chunk = 4 * 1024 * 1024
chunks_per_file = 256
for i in range(10):
    path = os.path.join(root, f"blob-{i:02d}.bin")
    with open(path, "wb", buffering=chunk) as f:
        for _ in range(chunks_per_file):
            f.write(os.urandom(chunk))
print(f"files=10 total_bytes={10*chunk*chunks_per_file} total_mib={10*chunk*chunks_per_file/1024/1024:.2f}")
PY
du -sh /tmp/wdt-bench-src-10x1g-random
find /tmp/wdt-bench-src-10x1g-random -type f | wc -l'
```

Run with:

- Source: `/tmp/wdt-bench-src-10x1g-random`
- Destination: `/tmp/wdt-bench-dst-10x1g-random`
- Start port: `30000`

## Generate `200x50MiB-random`

```bash
ssh -F /dev/null -o BatchMode=yes -o ConnectTimeout=8 -o StrictHostKeyChecking=no ailocal@10.20.1.250 'rm -rf /tmp/wdt-bench-src-200x50m-random; mkdir -p /tmp/wdt-bench-src-200x50m-random; python3 - <<PY
import os
root = "/tmp/wdt-bench-src-200x50m-random"
chunk = 1024 * 1024
chunks_per_file = 50
for i in range(200):
    d = os.path.join(root, f"dir-{i//50:02d}")
    os.makedirs(d, exist_ok=True)
    path = os.path.join(d, f"file-{i:03d}.bin")
    with open(path, "wb", buffering=chunk) as f:
        for _ in range(chunks_per_file):
            f.write(os.urandom(chunk))
print(f"files=200 total_bytes={200*chunks_per_file*chunk} total_mib={200*chunks_per_file:.2f}")
PY
du -sh /tmp/wdt-bench-src-200x50m-random
find /tmp/wdt-bench-src-200x50m-random -type f | wc -l'
```

Run with:

- Source: `/tmp/wdt-bench-src-200x50m-random`
- Destination: `/tmp/wdt-bench-dst-200x50m-random`
- Start port: `31000`

## Generate `1000-variable-random`

```bash
ssh -F /dev/null -o BatchMode=yes -o ConnectTimeout=8 -o StrictHostKeyChecking=no ailocal@10.20.1.250 'rm -rf /tmp/wdt-bench-src-var-random; python3 - <<PY
import os, random
root = "/tmp/wdt-bench-src-var-random"
os.makedirs(root, exist_ok=True)
rng = random.Random(20260426)
min_size = 100 * 1024
max_size = 100 * 1024 * 1024
chunk_size = 1024 * 1024
manifest = open(root + "/MANIFEST.tsv", "w")
total = 0
for i in range(1000):
    d = os.path.join(root, f"dir-{i//100:02d}")
    os.makedirs(d, exist_ok=True)
    size = rng.randint(min_size, max_size)
    path = os.path.join(d, f"file-{i:04d}.bin")
    with open(path, "wb", buffering=chunk_size) as f:
        remaining = size
        while remaining >= chunk_size:
            f.write(os.urandom(chunk_size))
            remaining -= chunk_size
        if remaining:
            f.write(os.urandom(remaining))
    manifest.write(f"{path}\t{size}\n")
    total += size
manifest.close()
print(f"files=1000 total_bytes={total} total_mib={total/1024/1024:.2f}")
PY
du -sh /tmp/wdt-bench-src-var-random
find /tmp/wdt-bench-src-var-random -type f ! -name MANIFEST.tsv | wc -l'
```

Run with:

- Source: `/tmp/wdt-bench-src-var-random`
- Destination: `/tmp/wdt-bench-dst-var-random`
- Start port: `32000`

## Generate `104858x100KiB-random`

```bash
ssh -F /dev/null -o BatchMode=yes -o ConnectTimeout=8 -o StrictHostKeyChecking=no ailocal@10.20.1.250 'rm -rf /tmp/wdt-bench-src-100k-random; python3 - <<PY
import os
root = "/tmp/wdt-bench-src-100k-random"
os.makedirs(root, exist_ok=True)
count = 104858
size = 100 * 1024
manifest = open(root + "/MANIFEST.tsv", "w")
for i in range(count):
    d = os.path.join(root, f"dir-{i//1000:03d}")
    os.makedirs(d, exist_ok=True)
    path = os.path.join(d, f"file-{i:06d}.bin")
    with open(path, "wb") as f:
        f.write(os.urandom(size))
    manifest.write(f"{path}\t{size}\n")
manifest.close()
print(f"files={count} total_bytes={count*size} total_mib={count*size/1024/1024:.2f}")
PY
du -sh /tmp/wdt-bench-src-100k-random
find /tmp/wdt-bench-src-100k-random -type f ! -name MANIFEST.tsv | wc -l'
```

Run with:

- Source: `/tmp/wdt-bench-src-100k-random`
- Destination: `/tmp/wdt-bench-dst-100k-random`
- Start port: `29000`

## Collect Results

Sender:

```bash
ssh -F /dev/null -o BatchMode=yes -o ConnectTimeout=8 -o StrictHostKeyChecking=no ailocal@10.20.1.250 \
'grep -E "Total sender time|Total sender throughput|Transfer status|real |user |sys " SENDER_LOG; du -sh SENDER_DIR; find SENDER_DIR -type f | wc -l'
```

Receiver:

```bash
ssh -F /dev/null -o BatchMode=yes -o ConnectTimeout=8 -o StrictHostKeyChecking=no symek@10.20.1.150 \
'grep -E "Transfer status|Average throughput|Returning with OK" RECEIVER_LOG | tail -20; du -sh RECEIVER_DIR; find RECEIVER_DIR -type f | wc -l'
```

