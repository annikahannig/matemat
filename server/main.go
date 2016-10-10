package main

import (
	"github.com/tarm/serial"
	"log"
)

func main() {
	conn, err := serial.OpenPort(&serial.Config{
		Name: "/dev/cu.usbmodem1421",
		Baud: 19200,
	})

	if err != nil {
		log.Fatal("Could not open serial device:", err)
	}

	buffer := make([]byte, 128)

	for {
		n, err := conn.Read(buffer)
		if err != nil {
			log.Fatal("Could not read from serial device:", err)
		}
		log.Println(string(buffer[:n]))
	}

}
