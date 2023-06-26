# `controller-firmware`

controller-firmware

---

## `Usage`

### ***Windows***

1. Download ARM GCC toolchain [9-2019-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads/9-2019-q4-major). [10.3-2021.10](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) You should get **gcc-arm-none-eabi-9-2019-q4-major-win32.zip** file or equivalent.

2. Create a folder on C driver called **TOOLCHAIN** -> "C:/TOOLCHAIN/". After folder is created, extract toolchain here. Toolchain path should look **C:/TOOLCHAIN/9-2019-q4-mayor** same as makefile showed in line 7.

3. Download and Install [Cygwin](https://cygwin.com/install.html).

4. Add **C:/cygwin64/bin** folder to the **PATH**. To test this step works, open a CMD console and type "grep" or "mv".

5. Install GIT on windows computer. Download the installer from [here](https://git-scm.com/download/win).

6. Open a new CMD terminal and configure git with your user name and password. For example: `git config --global user.name "Santiago Goldaraz"` and also `git congig --user.mail "santiagogoldaraz@gmail.com"`

7. Download **GNU MCU Eclipse Windows Build Tools** from [here](https://github.com/gnu-mcu-eclipse/windows-build-tools/releases). In the case you didn't get the installer file, copy the folder "GNU MCU Eclipse" into C driver, and put it into the program folder.

8. Add **GNU MCU Eclipse Windows Build Tools** folder to **PATH**: Example: "C:\Program Files\GNU ARM Eclipse\Build Tools\2.6-201507152002\bin". To test this step works, open a CMD console and type "make".

9. Congratulation!!! You should be able to clone an compile this project by typing **make -j8** and clean by typing **make clean** from the project root folder.

### ***Linux***

#### *Ubuntu / apt*

1. `$ sudo apt-get update && apt-get upgrade`

2. `$ sudo apt-get install build-essential && apt-get install git`

#### *Fedora / dnf*

1. `$ sudo dnf check-update`

2. `$ sudo dnf groupinstall "Development Tools" "Development Libraries" && dnf install git`

#### *Linux*

3. Download ARM GCC toolchain from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads). You should have this file or equivalent in the system ready to be installed or extracted **gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2**. \
`$ wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2`

4. Uncompressed file. \
`$ tar -xvjf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2`

5. Make new dir where toolchain will live. \
`$ sudo mkdir /opt/arm-toolchain/`

6. Copy toolchain to default project folder. \
`$ sudo mv ./gcc-arm-none-eabi-9-2019-q4-major /opt/arm-toolchain/gcc-arm-none-eabi-9-2019-q4-major`

7. Congratulation!!!  You should be able to clone an compile this project by typing **make -j8** and clean by typing **make clean** on project root folder.
