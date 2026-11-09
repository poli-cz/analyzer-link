# analyzer-link

Decodes the text frames a bench chemistry analyzer sends over its serial link and turns
instrument errors into records for the laboratory information system (LIS).

```
HB|AN-07|2.6                      heartbeat with the protocol version
RES|AN-07|S-1001|GLU|5.4|mmol/L   result
ERR|AN-07|9|S-1003                instrument error (code, sample or -)
```

`link-cli` reads frames from standard input and prints one line per frame:

```
$ build/debug/link-cli < tests/data/recorded/an07-2026-10-14.frames
heartbeat AN-07 protocol 2.6 ok
result AN-07 S-1001 GLU 5.4 mmol/L
error AN-07 code=9 "Reagent empty" lis=E|AN-07|S-1003|R|Reagent empty
...
```

## Build and test

CMake 3.22+, Ninja and a C++20 compiler (GCC, Clang or MSVC). GoogleTest is used from the
system when installed, otherwise from `third_party/`.

```
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Other presets: `release`, `asan` (AddressSanitizer and UBSan), `tidy` (clang-tidy on
first-party code).

## Layout

| path | content |
|---|---|
| `include/alink/`, `src/` | library: frame decoder, instrument errors, LIS export |
| `apps/link-cli/` | command line tool |
| `tests/` | GoogleTest suites and recorded analyzer captures in `tests/data/recorded/` |
| `third_party/googletest/` | copy of GoogleTest |
