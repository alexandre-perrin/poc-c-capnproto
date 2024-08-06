# Cap'n Proto C POC

This is a simple example on how to use
the C version of Cap'n proto,
an alternative derived from Google Protobuf.

## Prerequisites

Install `capnproto`:

```
sudo apt-install capnproto
```

Build `c-capnproto`:
Requires `meson`.

```
pip install meson
```

Then:

```
cd c-capnproto
meson build .
meson compile -C build
```

And install binaries in the system:

```
cd c-capnproto/build
cp capnp-c /usr/bin
cp libcapnp.so /usr/lib
```

## Build

```
make
```

## Run the example

```
make run
```

or 

```
./demo
```
