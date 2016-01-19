
Overview
========
This project is using `cbuid.scripts` to configure the all neccessary files path and yet provide friendly command to execute specified task.

Commands
========
All the hard work behind the scene is handled by `cbuild` and now we only need to type in the command.
To view all available commands issue:
```
rake -T
```

Configuration
=============

By default rake loads `project.yml` configuration file for Ceedling. You can override that by issuing `project=<yml_filename>`. E.g.,
```
rake project=./myproject.yml test:all
```
will load `myproject.yml` configuration file in the current directory and then run all tests.

Release
=======
To build hardware `release` version, type:
```
rake probe:release
```
The script for building it can be found in `scripts/probe.rb`. Edit the script if you need different compilation/linking outcome.

The command above only works if there is **exactly** one `.coproj` file in the current directory. If it has none, you need to specify the path and filename of the coproj. E.g.,
```
rake probe:release[project/Blinky.coproj]
```
Similarly, if there are many `coproj` files in the current directory, you need to specify which one `coproj` file to be used for building.

To release and flash `FlashProgrammer` need to specified the `.coproj` type:
```
rake target:release[FlashProgrammer/FlashProgrammer.coproj]
rake target:flash[FlashProgrammer/FlashProgrammer.coproj]
```

Flash
=====
To flash hardware `release` version, type:
```
rake hw:flash
```
This command is to build the source code from the path stated in `.coproj` and flash it into hardware using ST-LINK. This command relies on `ST-LINK_CLI.exe` program by default. The program can be obtained from [here](http://www.st.com/web/en/catalog/tools/PF258168). You need to include the path to that program into the system path, so that the build system is able to invoke it.

There is an another way of running this command by specified the `.coproj` name similar with 'probe:release'.
```
rake hw:flash[Blinky02.coproj]
```

To specify a different name of or specific path to flash-programmer, you can pass it in through rake command:
```
rake hw:flash flasher=/c/STM/stlink/ST_FLASHER
```

GCC compiles and links C/C++ files to ELF executable. Unfortunately, `ST-LINK_CLI` lacks the ability to read such file, but only HEX or BIN file. The good news is, the translation can be done by `arm-none-eabi-objcopy`. The `rake hw:flash` command automatically performs this and assumes that the program is in the Bash path. If the name or the path needs to be specified, you can issue:
```
rake hw:flash elf_to_hex="/e/ProgramFiles/GNU Tools ARM Embedded/4.9 2015q1/bin/arm-none-eabi-objcopy"
```

Tests
=====
There is 2 different platform dependent test in this project which is specified in `host` and `probe`.

To run all `host` test in command :
```
rake host:test:all
```
There is some different from running `probe` test because there are 2 different type of test one is running on ceedling using mock and the another hand is running on the actual hardware called hardware-in-the-loop test in `test/Hardware`. Therefore we need to load a specified `.yml` in order to run the `probe:test:all`.

To run all `probe` test running on ceedling:
```
rake p=probe.yml probe:test:all
```

To run all `probe` hardware-in-the-loop test:
```
rake p=probe_hil.yml probe:hw:test:all
```

CLI
========
To compile and release Command Line Interface for this project simply use the following command:
```
rake host:release
```
This command will automatic generate the Main.exe located in file build/release
