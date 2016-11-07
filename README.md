
# Install

Currently only build-by-yourself is supported.

1. Run `bash bootstrap.sh` to see if required packages are available.
2. Run `cmake .` and fix any missing dependencies (i.e., perhaps you need SFML, `apt install libsfml-dev` on Ubuntu)
3. Run `make -j $(nproc)`

# Compiling with clang

On Ubuntu 15.10 SFML will give linking errors like the following.

  src/states/selecting.cpp:96: undefined reference to `sf::IpAddress::toString() const'

This is because of http://en.sfml-dev.org/forums/index.php?topic=18482.0

sfml was build for c++11 abi without  -D_GLIBCXX_USE_CXX11_ABI=0,
and current clang in Ubuntu does not yet support GCC's [abi:cxx11] tags.

that's why you need the latest clang, i.e., I followed instructions from the following answer
http://askubuntu.com/questions/735201/installing-clang-3-8-on-ubuntu-14-04-3

I just chose 3.9 instead of 3.8, also make sure the repository line you add is the correct one for your Ubuntu version
