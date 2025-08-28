# X2tConverter – Generate HTML API Documentation

This guide explains how to install Doxygen and generate the HTML documentation for the C++ sources in `X2tConverter/src/`.

## Prerequisites

- Doxygen

### Windows (PowerShell)
- Install Doxygen:
  ```powershell
  winget install doxygen
  ```

### Linux (Debian/Ubuntu)
```bash
sudo apt-get update && sudo apt-get install -y doxygen
```

## Build the HTML docs

From current directory, run:

```
doxygen ./Doxyfile
```

Output will be generated here:
- `html/index.html`

Open that file in a browser to view the documentation.

## Configuration notes

- The Doxygen config lives at `Doxyfile`.
