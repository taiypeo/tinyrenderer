# tinyrenderer
A rendering demo made by following [ssloy's tutorial](https://github.com/ssloy/tinyrenderer).

SFML is the only dependency, it is only used to save the software-rendered image.


## Features
- [X] Line rendering
- [X] Triangle rendering
- [X] Shaders
- [X] Phong reflection
- [X] Normal mapping
- [X] Linear transformations
- [ ] Darboux frame normal mapping
- [ ] Shadows
- [ ] Ambient occlusion

## Examples
![diablo](https://user-images.githubusercontent.com/4065977/235376499-f4b84c6d-d17d-41b5-b274-2831503b36ca.png)
![head](https://user-images.githubusercontent.com/4065977/235376546-d43d0b66-192c-432c-87b5-2c38f0cbc044.png)

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
