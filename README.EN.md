# KKM HTTPS Adapter

## Description

This is yet another HTTPS server in C++, but with ~~blackjack and hoo...~~ Asio, OpenSSL and coroutines.

An implementation of a service for Windows OS that provides access to a cash register (hereinafter CR) connected to
a computer and supported by the ATOL driver. The REST JSON API over HTTPS is used to access the CR. This is not an
independent product, it is a component of a distributed system. It enables software running on a remote server access
a CR locally connected to the operator's thin client. This service is designed to be used in applications with a web
interface and business logic implemented on a remote server. In this regard, full-fledged interactive interaction with
CR is could be challenging in some cases. Therefore, all stages of the operation (sale registration, shift closure,
etc.) are performed atomically.

## Building

For assembly, first of all, you will need:
- ATOL KKT Driver version not lower than 10.10.7 ([Official website](http://fs.atol.ru/))
- Asio C++ Library ([Git](https://github.com/chriskohlhoff/asio), [Official website](https://think-async.com/Asio/))
- OpenSSL ([Git](https://github.com/openssl/openssl), [Official website](https://www.openssl.org/))
- JSON for Modern C++ ([Git](https://github.com/nlohmann/json), [Official website](https://json.nlohmann.me/))
- Catch2 ([Git](https://github.com/catchorg/Catch2))
- vcpkg ([Git](https://github.com/microsoft/vcpkg), [Official website](https://learn.microsoft.com/en-us/vcpkg/))

**vcpkg** can be replaced with another dependency manager for C++ or ready-made assemblies of the above mentioned
libraries can be used. In this case, you may need to correct `CMakeLists.txt`.

The assembly was tested using the MSVC and Clang compilers. The scripts with the standard assembly for these compilers
are located in the `.\_msvc` and `.\_clang` directories, respectively.

| Script                      | Description                                                                                                                                            |
|-----------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------|
| `config_env.cmd`            | Sets environment variables and runs the necessary bootstrap scripts. It is called from the scripts listed below                                        |
| `install_dynamic_deps.cmd`  | Installs dynamic build dependencies and copies the necessary header files from `%programfiles%\ATOL\Drivers10\KKT\langs\cpp\fptr10` to `.\deps\fptr10` |
| `install_static_deps.cmd`   | Installs static build dependencies and copies the necessary header files from `%programfiles%\ATOL\Drivers10\KKT\langs\cpp\fptr10` to `.\deps\fptr10`  |
| `test_dynamic.cmd`          | Builds the dynamic debug version and runs unit tests                                                                                                   |
| `test_static.cmd`           | Builds the static debug version and runs unit tests                                                                                                    |
| `build_dynamic_memprof.cmd` | Builds the dynamic debug version with memory profiling enabled                                                                                         |
| `build_static_memprof.cmd`  | Builds the static debug version with memory profiling enabled                                                                                          |
| `build_dynamic_debug.cmd`   | Builds the dynamic debug version                                                                                                                       |
| `build_static_debug.cmd`    | Builds the static debug version                                                                                                                        |
| `build_dynamic_release.cmd` | Builds the dynamic release version                                                                                                                     |
| `build_static_release.cmd`  | Builds the static release version                                                                                                                      |

It is assumed that the required software is installed in the following directories:

| Software                                       | Path                                                    |
|------------------------------------------------|---------------------------------------------------------|
| ATOL Driver                                    | `%programfiles%\ATOL`                                   |
| Microsoft Visual Studio 2022 Community Edition | `%programfiles%\Microsoft Visual Studio\2022\Community` |
| Clang                                          | `C:\Devel\Platform\Clang\20.1.8-x86_64`                 |
| CMake                                          | `C:\Devel\Platform\CMake\3.31.7-x86_64`                 |
| Ninja                                          | `C:\Devel\Platform\Ninja\1.13.1`                        |

The paths can be modified in the files `config_env.cmd` and `install_*_deps.cmd`. CMake and Ninja can be used from MSVS.

After the build, one of the scripts `build_*.cmd` will install the file `kkmha.exe` in the directory `.\_build` and
files `libcrypto-?-x64.dll `, `libssl-?-x64.dll` for dynamic build. Also, this directory already contains configuration
files, etc., forming a ready-made environment.

In order to run the service on a PC other than the one on which the build was made, you may need to install the latest
package [Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170).
