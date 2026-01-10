# Easy-Pinn

Small C++ library for experimenting with Physics-Informed Neural Networks (PINNs) using a single hidden layer and analytic input/parameter derivatives.

## Reference

- I.E. Lagaris, A. Likas, D.I. Fotiadis, "Artificial neural networks for solving ordinary and partial differential equations", IEEE Trans. Neural Networks, 1998.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Run tests:

```bash
ctest --test-dir build
```
