# ble

An extensible command-line tool written in C++ for managing known bluetooth devices

## Usage

Change the MAC address

```cpp
constexpr auto airpodsMAC = "D0:4F:65:A1:6E:D6";
auto airpods = false;
```

```sh
g++ ble.cpp -o ble -lboost_iostreams
chmod +x ble
./ble -airpods
```
