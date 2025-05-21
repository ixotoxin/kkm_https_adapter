# KKM HTTPS Adapter

## Description

This is yet another HTTPS server on C++, but with ~~blackjack and hoo...~~ Asio, OpenSSL and coroutines.

The implementation of a service for Windows OS that provides access to a cash register (hereinafter CR) connected
to a computer supported by the ATOL driver. The REST JSON API over HTTPS is used to access the CR. This is not an
independent product, it is a component of a distributed system. With it, you can implement access by software running
on a remote server to a CR locally connected to the operator's thin client. The role of this server is to be used
in applications with a web interface and business logic implemented on a remote server. In this regard, full-fledged
interactive interaction with CR is difficult in some cases. Therefore, all stages of the operation (sell registration,
shift closure, etc.) are performed atomically.

## Requirements

The following libraries are used:
- Library from the ATOL KKT Driver V10.10.6+ ([Official website](http://fs.atol.ru/))
- Asio C++ Library ([Git](https://github.com/chriskohlhoff/asio), [Official website](https://think-async.com/Asio/))
- OpenSSL ([Git](https://github.com/openssl/openssl), [Official website](https://www.openssl.org/))
- JSON for Modern C++ ([Git](https://github.com/nlohmann/json), [Official website](https://json.nlohmann.me/))
- Catch2 ([Git](https://github.com/catchorg/Catch2))

## Building

For assembly, first of all, you will need:
- ATOL KKT Driver version not lower than 10.10.6 ([Official website](http://fs.atol.ru/))
- vcpkg ([Git](https://github.com/microsoft/vcpkg), [Official website](https://learn.microsoft.com/en-us/vcpkg/))

**vcpkg** can be replaced with another dependency manager for C++ or use ready-made assemblies of the above libraries.
In this case, you may need to correct `CMakeLists.txt`.

The assembly was tested using the MSVC and Clang compilers. The scripts with the standard assembly for these compilers
are located in the `.\_msvc` and `.\_clang` directories, respectively.

| Script              | Description                                                                                                                           |
|---------------------|---------------------------------------------------------------------------------------------------------------------------------------|
| `config_env.cmd`    | Set environment variables and running the necessary bootstrap scripts. It is called from the scripts listed below                     |
| `install_deps.cmd`  | Install dependencies and copy the necessary header files from `%programfiles%\ATOL\Drivers10\KKT\langs\cpp\fptr10` to `.\deps\fptr10` |
| `test.cmd`          | Build the debug version and running unit tests                                                                                        |
| `build_mprof.cmd`   | Build the debug version with memory profiling enabled                                                                                 |
| `build_debug.cmd`   | Build the debug version                                                                                                               |
| `build_release.cmd` | Building the release version                                                                                                          |

It is assumed that the necessary software is installed in the following directories:

| Software                                       | Path                                                    |
|------------------------------------------------|---------------------------------------------------------|
| ATOL Driver                                    | `%programfiles%\ATOL`                                   |
| Microsoft Visual Studio 2022 Community Edition | `%programfiles%\Microsoft Visual Studio\2022\Community` |
| Clang                                          | `C:\Devel\Platform\Clang\20.1.5-x86_64`                 |
| CMake                                          | `C:\Devel\Platform\CMake\3.31.7-x86_64`                 |
| Ninja                                          | `C:\Devel\Platform\Ninja\1.12.1`                        |

The paths can be changed in the files `config_env.cmd` and `install_deps.cmd`. CMake and Ninja can be used from MSVS.

After the build, one of the scripts `build_*.cmd` will install the files `kkmha.exe`, `libcrypto-X-x64.dll `,
`libssl-X-x64.dll ` in the directory `.\_build`. Also, this directory already contains configuration files, etc.,
forming a ready-made environment for launching.

To run on a PC other than the one on which the build was made, you may need to install the latest package
[Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170).
