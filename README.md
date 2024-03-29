# mod1

Small terrain renderer

## Clone instructions
This repo contains submodules, so clone with `--recursive`.

```shell
git clone --recursive git@github.com:pmerku/mod1
```

Otherwise clone the repo like normal and run this commands at the root of the repo:
```shell
git submodule update --init
```

## Run the program
You can use your favorite IDE with the provided `Cmake` file. Just build and run `mod1`.
Or use `cmake` in the terminal.

```shell
mkdir cmake
cd cmake
cmake .. && make
```

Both ways will build the executable in the `build` folder at the root of the repo.

## Screenshots
![alt tex](https://github.com/pmerku/mod1/raw/main/images/mod1.png "mod1 png")
