# DOS-based graphical SR and D flip-flops simulators

These are a couple of school projects I developed back in the 2000's. They
simulate the workings of SR and D-type flip flops and were developed using
Borland Turbo C. _SRSIM_ simulates an SR-type flip-flop, while _DSIM_
simulates a D-type flip-flop.

They probably have no real use today, but since I have come across the code
while scanning old drives, I wanted to make them able to compile on
modern systems and publish them. The UI is in Italian only as well as
most variable names within the code.

![SRSIM GUI][srsim-gui]

![DSIM CLI][dsim-gui]


_LEGAL DISCLAIMER: this repository includes the Borland Turbo C 2.01 toolchain,
which is necessary to build the programs. Technically, it is a copyrighted
product, but it is pretty easy to find it as abandoware on the web, and
it dates back to 1987, so I find unlikely somebody cares about it
anymore.  However, if the copyright holder decides so, it may contact me
for removal from the source tree._

## License

Distributed under the MIT license.

## Build system

As with other DOS-based projects I maintain, the build system is simply
a container with some Linux distro wrapping DosBox, which provides the
DOS environment required to run the toolchain.

The `docker/build.sh` script automatically takes care of building the
container if it does not exist, then it launches it with the repository
as a bind mount. Artifacts are built inside the container and then
copied under the `build` top level directory in the repository.

    # Build the programs
    cd docker
    ./build.sh

    # Grab files from build

    # Clean everything
    ./build.sh clean

Note that, even when cleaning, some log files are left under `build` for
convenience.

## Outputs

This is what you find under `build` when compilation is done:

    .
    ├── BUILD.LOG
    ├── DSIMCFG.BGI
    ├── DSIM.EXE
    ├── EGAVGA.BGI
    ├── RESULT
    ├── SRSIMCFG.BGI
    └── SRSIM.EXE 

`BUILD.LOG` and `RESULT` are log files left there by the build system
and can be ignored. Other files are used as follows:

* `SRSIM.EXE`, `DSIM.EXE` are the actual executables;
* `SRSIMCFG.BGI`, `DSIMCFG.BGI` are the configuration files for the
  programs, which merely contain the path where the BGI driver
  (Borland's graphics library) is to be found at runtime;
* `EGAVGA.BGI` is the graphics support driver. The programs need to
  locate an load this at runtime or they won't work. This is part of
  Borland Turbo C and has been copied here for convenience, as it needs
  to  be distributed alongside the programs.

[srsim-gui]: docs/srsim-gui.png
[dsim-gui]: docs/dsim-gui.png

<!-- vi: set tw=72 sw=4 sts=-1 fo=tcroqna et autoindent spell spelllang=en :-->
