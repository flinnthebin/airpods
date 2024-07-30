package main

import (
    "flag"
    "fmt"
    "io"
    "os"
    "os/exec"
    "strings"
)

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

func connectToBluetoothDevice(deviceMAC string) {
    cmd := exec.Command("bluetoothctl")
    stdin, err := cmd.StdinPipe()
    if err != nil {
        fmt.Fprintln(os.Stderr, "Error obtaining stdin pipe:", err)
        return
    }
    defer stdin.Close()

    stdout, err := cmd.StdoutPipe()
    if err != nil {
        fmt.Fprintln(os.Stderr, "Error obtaining stdout pipe:", err)
        return
    }
    defer stdout.Close()

    if err := cmd.Start(); err != nil {
        fmt.Fprintln(os.Stderr, "Error starting bluetoothctl:", err)
        return
    }

    waitForPrompt(stdout)

    _, err = io.WriteString(stdin, "scan on\n")
    if err != nil {
        fmt.Fprintln(os.Stderr, "Error writing command:", err)
        return
    }

    _, err = io.WriteString(stdin, "connect "+deviceMAC+"\n")
    if err != nil {
        fmt.Fprintln(os.Stderr, "Error writing command:", err)
        return
    }

    waitForPrompt(stdout)

    _, err = io.WriteString(stdin, "scan off\n")
    if err != nil {
        fmt.Fprintln(os.Stderr, "Error writing command:", err)
        return
    }

    _, err = io.WriteString(stdin, "quit\n")
    if err != nil {
        fmt.Fprintln(os.Stderr, "Error writing command:", err)
        return
    }

    if err := cmd.Wait(); err != nil {
        fmt.Fprintln(os.Stderr, "Error waiting for command:", err)
        return
    }
}

func waitForPrompt(stdout io.Reader) {
    buf := make([]byte, 1024)
    for {
        n, err := stdout.Read(buf)
        if err != nil {
            fmt.Fprintln(os.Stderr, "Error reading from stdout:", err)
            return
        }
        if strings.Contains(string(buf[:n]), "#") {
            break
        }
    }
}
