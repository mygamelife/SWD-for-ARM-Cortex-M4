
Overview
========
This project is using 'cbuid.scripts' to configure the all neccessary files path and yet provide friendly command to execute specified task.

Commands
========
All the hard work behind the scene is handled by 'cbuild' and now we only need to type in the command.
To view all available commands issue:
```
rake -T
```
The new commands added by `scripts/hw.rb` are:
```
rake hw:flash
rake hw:release
```

By default rake loads `project.yml` configuration file for Ceedling. You can override that by issuing `project=<yml_filename>`. E.g.,
```
rake project=./myproject.yml test:all
```
will load `myproject.yml` configuration file in the current directory and then run all tests.

Flash
=====
To flash hardware `release` version, type:
```
rake hw:flash
```
This command is to build the source code from the path stated in '.coproj' and flash it into hardware using ST-LINK. This command relies on `ST-LINK_CLI.exe` program by default. The program can be obtained from [here](http://www.st.com/web/en/catalog/tools/PF258168). You need to include the path to that program into the system path, so that the build system is able to invoke it.

There is an another way of running this command by specified the '.coproj' name. Let's say if there is two coproj inside same directory you can specified which coproj to be compile and flash.
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