# ble

An extensible command-line tool written in Go for managing known bluetooth devices

## Adding Devices

Add the device & MAC address variables to the var list,
Implement the flag in func init().
Make a switch statement in main if you're feeling fancy.

```go
var (
    airpodsMAC string = "B0:3F:64:21:7E:D7"
    airpods    bool
)

func init() {
    flag.BoolVar(&airpods, "airpods", false, "Connect to AirPods")
}

func main() {
    flag.Parse()

    if airpods {
        connectToBluetoothDevice(airpodsMAC)
    } else {
        fmt.Println("No device specified. Use -airpods to connect to AirPods.")
    }
}
```
