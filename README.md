
h1. Install 

Currently only build-by-yourself is supported.

1. Run `bash bootstrap.sh` to see if required packages are available.
2. Run `cmake .` and fix any missing dependencies (i.e., perhaps you need SFML, `apt install libsfml-dev` on Ubuntu)
3. Run `make -j $(nproc)`
