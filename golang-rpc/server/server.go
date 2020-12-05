package main

import (
	"log"
	"net"
	"net/http"
	"net/rpc"
	"net/rpc/jsonrpc"
)

type Calculator int

type Args struct {
	A, B int
}

func (c *Calculator) Multiply(args Args, result *int) error {
	*result = args.A * args.B
	return nil
}

func main() {
	calculator := new(Calculator)
	server := rpc.NewServer()
	server.Register(calculator)
	http.Handle(rpc.DefaultDebugPath, server)
	log.Println("start listening :18888")
	listener, err := net.Listen("tcp", ":18888")
	if err != nil {
		panic(err)
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			panic(err)
		}
		go server.ServeCodec(jsonrpc.NewServerCodec(conn))
	}
}
