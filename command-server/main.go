package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"

	"github.com/gorilla/websocket"
)

// We'll need to define an Upgrader
// this will require a Read and Write buffer size
var upgrader = websocket.Upgrader{
	ReadBufferSize:  108,
	WriteBufferSize: 108,
	CheckOrigin:     func(r *http.Request) bool { return true },
}

var data = ""

func setupRoutes() {
	http.HandleFunc("/hello-from-esp", func(w http.ResponseWriter, r *http.Request) {
		w.Write([]byte(data))
	})
	http.HandleFunc("/set-data", func(w http.ResponseWriter, r *http.Request) {
		body, _ := ioutil.ReadAll(r.Body)
		data = string(body)

		fmt.Printf("Set data to %s\n", data)
	})
}

func main() {
	fmt.Println("Hello World")
	setupRoutes()
	log.Fatal(http.ListenAndServe(":4567", nil))
}
