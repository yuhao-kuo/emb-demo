# emb-demo

## toolchain

### Windows toolchain

1. download tools
    * [gcc-arm-none-eabi](https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-win32.zip?revision=ab9cb8f8-6a9d-4a6e-818a-295f5d1ca982?product=GNU%20Arm%20Embedded%20Toolchain,ZIP,,Windows,8-2018-q4-major)
    * [MinGW](https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/)
2. install arm-gcc
    * view [document](https://docs.labs.mediatek.com/resource/mt7687-mt7697/en/get-started-linkit-7697-hdk/gcc-arm-embedded-linkit-7697/get-linkit-sdk-linkit-7697)
3. install MinGW
    * view [document](https://docs.labs.mediatek.com/resource/mt7687-mt7697/en/get-started-linkit-7697-hdk/gcc-arm-embedded-linkit-7697)

### Linux toolchain

1. install tools
    ```
    sudo apt-get install build-essential
    sudo dpkg --add-architecture i386
    sudo apt-get update
    sudo apt-get install libc6-i386
    ```

---

## Build

專案位置於 `{sdk_root}/project/link7697_sdk/` 切入此位置後下達 `make all` 即可進行編譯，編譯成功後將會在 `{sdk_root}/project/link7697_sdk/{project_name}/build` 中看到 **.bin** 檔案。

如需要將編譯資料刪除請下達 `make clean`

---

## Uploader

### Windows

##### 燒錄 bootloader

```
upload.exe -c COMx -t ldr -p mt7697 -f mt7697_bootloader.bin
````

##### 燒錄 n9 firmware

```
upload.exe -c COMx -t n9 -p mt7697 -f WIFI_RAM_CODE_MT76X7_in_flash.bin
```

##### 燒錄 application / firmware

```
upload.exe -c COMx -t cm4 -p mt7697 -f user_app.bin
```

### Linux

##### 燒錄 bootloader

```
python upload.py -c /dev/ttyUSB1 -t ldr -f mt7697_bootloader.bin
````

##### 燒錄 n9 firmware

```
python upload.py -c /dev/ttyUSB1 -t n9 -f WIFI_RAM_CODE_MT76X7_in_flash.bin
```

##### 燒錄 application / firmware

```
python upload.py -c /dev/ttyUSB1 -t cm4 -f user_app.bin
```
