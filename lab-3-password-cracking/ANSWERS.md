# Lab 3: Password Cracking - Answers

1. Kewuser Jemal

2. abccBcrPOxnLU -> pass

3. xyxMB6gxuiBZg -> ferry

4. Single thread, 5-letter exhaustive search (hash: nomatchingpass):
   real 0m34.458s

5. Timing by thread count (5-letter exhaustive search):
   - 1 thread:  34.458s
   - 2 threads: 17.277s
   - 4 threads: 9.340s
   - 8 threads: 6.671s
   - 13 threads: 5.136s
   - 26 threads: 5.367s

6. Hash rate (total hashes for lengths 1-5 = 12,356,630):
   - 1 thread:  ~358,600 hashes/sec
   - 2 threads: ~715,200 hashes/sec
   - 4 threads: ~1,323,000 hashes/sec
   - 8 threads: ~1,852,300 hashes/sec
   - 13 threads: ~2,405,900 hashes/sec
   - 26 threads: ~2,302,300 hashes/sec

7. 2 threads gives close to ideal 2x speedup (1.99x). But it falls off after
   that: 4 threads only gets 3.69x, 8 threads gets 5.17x, and 26 threads is
   actually slightly slower than 13 threads. This means the machine has
   somewhere around 8-13 logical cores. Once more threads are created than
   there are cores, they compete for CPU time instead of running truly in
   parallel, so speedup flattens out and eventually drops slightly due to
   scheduling/context-switch overhead.

8. Maximum hash rate recorded: ~2,405,900 hashes/sec (at 13 threads)

9. Total hashes to exhaustively search each keysize (26 lowercase chars):
   - 1: 26
   - 2: 676
   - 3: 17,576
   - 4: 456,976
   - 5: 11,881,376
   - 6: 308,915,776
   - 7: 8,031,810,176
   - 8: 208,827,064,576

10. Estimated time to exhaust each keyspace at max rate (~2,405,900/sec):
    - 1-4: under a fifth of a second
    - 5: ~4.9s
    - 6: ~128s (~2.1 min)
    - 7: ~3,338s (~55.6 min)
    - 8: ~86,790s (~24.1 hours, about a day)

    Benefit of increasing key length: each extra character multiplies the
    search space by 26, so cracking time grows exponentially. Going from
    length 7 to 8 turns under an hour into a full day, even at this
    program's fastest measured rate.

11. Estimated time for a 6-character keyspace with larger alphabets, at max
    rate (~2,405,900/sec):
    - 52 chars (upper+lower): 52^6 = 19,770,609,664 hashes -> ~8,218s (~2.3 hours)
    - 62 chars (+digits): 62^6 = 56,800,235,584 hashes -> ~23,609s (~6.6 hours)
    - 93 chars (+symbols): 93^6 = 646,990,183,449 hashes -> ~268,944s (~74.7 hours, ~3.1 days)

12. No known bugs. One note: when a match is found, the worker thread calls
    exit(0) directly to terminate the whole process immediately per spec,
    rather than signaling other threads to stop and joining them, which
    means any in-progress allocations aren't explicitly freed on that path
    (harmless since the OS reclaims all process memory on exit).

13. Extra credit attempted: all three.
    - Cracked the 9 five-letter hashes (see below)
    - Explained why salts are useful
    - Added a 4th argument for extended-alphabet search

## Extra Credit: 9 hashes

| Hash | Password |
|---|---|
| abA.g8pU2Iffo | salts |
| cdfnIXMyMCpPg | salts |
| efgC/gw8PDKhs | salts |
| ghLneTdBMxJP. | salts |
| ijSsXTgIC7QRU | salts |
| klr7dT7cAODsk | donot |
| mn4iWfK0m76t6 | skipp |
| opjPsgpXaahxM | anyyy |
| qr6ncfvfqecME | salts |

Six of these hash to the same password ("salts") under different salts,
producing completely different hash strings, which demonstrates the next
answer directly.

## Extra Credit: Why salts are useful

Without a salt, two users with the same password get the identical hash,
so an attacker can precompute a lookup table (rainbow table) of common
password-to-hash mappings once and crack every account using a hash in
that table instantly. A salt forces the attacker to redo the hashing work
separately for every account, since crypt(password, salt) produces a
different hash per salt even for the same password. It doesn't make any
individual password harder to brute-force, but it eliminates the "crack
once, break everyone" efficiency that unsalted hashes allow, and it also
prevents anyone from telling two accounts share a password just by
comparing hashes.

## Extra Credit: Extended alphabet

Added an optional 4th command-line argument. When present (any value),
the program searches the full printable ASCII range from '!' (33) through
'~' (126), 94 characters covering uppercase, lowercase, digits, and
common symbols, instead of just lowercase a-z. Verified against a
generated test hash for password "ab!" with salt "zz".
