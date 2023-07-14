# `controller-firmware`

controller-firmware

---

## `Usage`

### ***Building***

#### ***Windows***

1. Download ARM GCC toolchain [9-2019-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads/9-2019-q4-major). [10.3-2021.10](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) You should get **gcc-arm-none-eabi-9-2019-q4-major-win32.zip** file or equivalent.

2. Create a folder on C drive called **TOOLCHAIN** -> "C:/TOOLCHAIN/". After the folder is created, extract toolchain here. Toolchain path should look like **C:/TOOLCHAIN/9-2019-q4-mayor** same as makefile shown on line 7.

3. Download and Install [Cygwin](https://cygwin.com/install.html).

4. Add **C:/cygwin64/bin** folder to the **PATH**. To test this step works, open a CMD console and type "grep" or "mv".

5. Install GIT on windows computer. Download the installer from [here](https://git-scm.com/download/win).

6. Open a new CMD terminal and configure git with your user name and password. For example: `git config --global user.name "Santiago Goldaraz"` and also `git congig --user.mail "santiagogoldaraz@gmail.com"`

7. Download **GNU MCU Eclipse Windows Build Tools** from [here](https://github.com/gnu-mcu-eclipse/windows-build-tools/releases). In the case you didn't get the installer file, copy the folder "GNU MCU Eclipse" into C driver, and put it into the program folder.

8. Add **GNU MCU Eclipse Windows Build Tools** folder to **PATH**: Example: "C:\Program Files\GNU ARM Eclipse\Build Tools\2.6-201507152002\bin". To test this step works, open a CMD console and type "make".

9. Congratulation!!! You should be able to clone and **compile** this project by typing `make -j8` *or* `make all` and **clean** by typing `make clean` from the project root folder.

#### ***Linux***

##### *Ubuntu / apt*

1. `$ sudo apt-get update && apt-get upgrade`

2. `$ sudo apt-get install build-essential && apt-get install git`

##### *Fedora / dnf*

1. `$ sudo dnf check-update`

2. `$ sudo dnf install make automake gcc gcc-c++ kernel-devel && sudo dnf install git`

##### *Linux*

3. Download ARM GCC toolchain from [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads). You should have this file or equivalent in the system ready to be installed or extracted **gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2**. \
`$ wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2`

4. Uncompressed file. \
`$ tar -xvjf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2`

5. Make new dir where toolchain will live. \
`$ sudo mkdir /opt/arm-toolchain/`

6. Copy toolchain to default project folder. \
`$ sudo mv ./gcc-arm-none-eabi-9-2019-q4-major /opt/arm-toolchain/gcc-arm-none-eabi-9-2019-q4-major`

7. Congratulation!!!  You should be able to clone and **compile** this project by typing `$ make -j8` *or* `$ make all` and **clean** by typing `$ make clean` on project root folder.

### ***Flashing***

#### *Windows*

1. ...

#### *Linux with STLink*

1. Install STLink package [here](https://github.com/stlink-org/stlink).

2. Verify connection with: \
`$ st-info --probe`

3. Flash firmware with: \
`$ st-flash write <example>.bin 0x8000000`

## `Hardware Manager`

### ***Usage***

Example task for blinking LEDs.

```cpp
#include <hardware/hardware-manager.hpp>

static HardwareManager s_manager;
static Led *s_led;

static bool s_led_on = false;

void task_blink_thread(void*)
{
    s_led = s_manager.get_led();
    s_manager.init();

    for ( ;; )
    {
        s_led->set_led_1(s_led_on);
        s_led->set_led_2(!s_led_on);
        s_led->set_led_3(s_led_on);

        s_led_on = !s_led_on;
        s_manager.update();

        vTaskDelay(250);
    }
}
```

Hardware Manager Config.

```cpp
struct Hardware_Config
{
    uint16_t neoled_update_interval;
    uint16_t led_update_interval;
    uint16_t imu_update_interval;
};

Hardware_Config example_config
{
    .neoled_update_interval = 40,
    .led_update_interval = 250,
    .imu_update_interval = 5,
}
```

### ***Actuators***

Actuator interface.

```cpp
class Actuator
{
    public:

        virtual void init(void) = 0;

        virtual void update(void) = 0;
};
```

Lego Motor interface.

```cpp
class LegoMotor
{
    public:

        virtual void forward(int32_t rotation) = 0;

        virtual void backward(int32_t rotation) = 0;

        virtual void stop(void) = 0;

        virtual void start_sync(LegoMotor *motor) = 0;

        virtual void end_sync(void) = 0;

        virtual bool is_stalled(void) = 0;

        virtual int32_t get_tacho_count(void) = 0;

        virtual void set_motor_pwm(uint8_t motor_pwm) = 0;

        virtual uint8_t get_motor_pwm(void) = 0;

        virtual void set_motor_encoder(uint8_t motor_encoder) = 0;

        virtual uint8_t get_motor_encoder(void) = 0;

        virtual void set_motor_speed(uint32_t motor_speed) = 0;

        virtual uint32_t get_motor_speed(void) = 0;

    private:

        virtual void drive_motor(float speed, int32_t rotation, bool immediate_return) = 0;
};
```

### ***Devices***

Device interface.

```cpp
class Device
{
    public:

        virtual void init(void) = 0;

        virtual void update(void) = 0;
};
```

unknown if needed.

### ***Sensors***

Sensor interface.

```cpp
class Sensor
{
    public:

        virtual void init(void) = 0;

        virtual void update(void) = 0;

        virtual float fetch_sample(void) = 0;
};
```

#### *Inertial Measurement Unit (IMU)*

#### *EV3 Colour Sensor*

#### *EV3 Sound Sensor*
