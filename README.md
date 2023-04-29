# tinyrenderer
A rendering demo made by following [ssloy's tutorial](https://github.com/ssloy/tinyrenderer).

SFML is the only dependency, it is only used to save the software-rendered image.

## Building
1. Download SFML using your package manager. Example:
```bash
sudo pacman -S sfml
```
2. Build the project by running
```bash
cmake . && make
```

## Running
```bash
./raycaster.out
```

This will create a `result.png` image with the rendered scene.
