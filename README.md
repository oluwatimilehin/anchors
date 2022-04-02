# Anchors

Anchors is a C++ library for [self-adjusting computations](https://lord.io/spreadsheets/), based
on [lord/anchors](https://github.com/lord/anchors) for rust
and [janestreet/incremental](https://github.com/janestreet/incremental) for OCaml.

It allows you represent complex computations as a DAG that will update efficiently in response to an input change. That
is, when a node value changes, it will only recompute the parts of the graph that depend on the node. [reword this]

## Usage

Anchors

## Installation

## Roadmap

- Implement [lord/anchors](https://lord.io/spreadsheets/) optimization - scroll to "anchors, a hybrid solution".
- Implement a `setUpdater()` function that allows you change the updater function for an `Anchor`.
- Add support for
  an [Incremental.bind](https://ocaml.janestreet.com/ocaml-core/latest/doc/incremental/Incremental__/Incremental_intf/#bind)
  equivalent.
- Support caching input parameters.
- Support for `map3`, `map4`, etc.
- More tests.