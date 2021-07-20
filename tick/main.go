package main

import (
	"os"
	"os/signal"
	"time"
)

func main() {
	go func() {
		for range time.Tick(1 * time.Second) {
			println("HI")
		}
	}()

	quit := make(chan os.Signal)
	signal.Notify(quit, os.Interrupt)

	<-quit
	println("Hello")
}
