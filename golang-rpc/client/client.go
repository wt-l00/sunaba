package main

import (
	"fmt"
	"log"
	"net"
	"net/rpc/jsonrpc"
)

type Args struct {
	A, B int
}

func main() {
	client, err := net.Dial("tcp", "localhost:18888")
	if err != nil {
		log.Fatal("dialing:", err)
	}
	// Synchronous call
	args := &Args{7, 8}
	var reply int
	c := jsonrpc.NewClient(client)
	err = c.Call("Calculator.Multiply", args, &reply)
	if err != nil {
		log.Fatal("arith error:", err)
	}
	fmt.Printf("Result: %d*%d=%d\n", args.A, args.B, reply)
}
