package main

import (
	"github.com/gin-gonic/gin"
	"github.com/gorilla/websocket"
	"io/ioutil"
	"log"
	"net/http"
	"os"
)

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool {
		return true // 允许所有来源连接
	},
}

// 客户端连接的集合
var clients = make(map[*websocket.Conn]bool)

func receiveLog(c *gin.Context) {
	//把logdata写道文件里
	file, err := os.OpenFile("D:/codefield/tftp-client/log/log.txt", os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		log.Println(err)
		return
	}
	defer file.Close()
	// 读取HTTP请求的主体内容
	data, err := ioutil.ReadAll(c.Request.Body)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to read request body"})
		return
	}
	logData := string(data)
	//plus a \n
	logData += "\n"
	_, err = file.WriteString(logData)
	if err != nil {
		log.Println(err)
		return
	}
	log.Printf("Received log: %v", logData)
	forwardLog(logData)
	c.JSON(http.StatusOK, gin.H{"message": "Log received and forwarded"})

}

func forwardLog(logData string) {
	for client := range clients {
		err := client.WriteMessage(websocket.TextMessage, []byte(logData))
		if err != nil {
			log.Printf("Error: %v", err)
			client.Close()
			delete(clients, client)
		}
	}
}

func handleWebSocket(c *gin.Context) {
	conn, err := upgrader.Upgrade(c.Writer, c.Request, nil)
	if err != nil {
		log.Println(err)
		return
	}
	defer conn.Close()

	clients[conn] = true

	for {
		_, _, err := conn.ReadMessage()
		if err != nil {
			delete(clients, conn)
			return
		}
	}
}
