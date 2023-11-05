package main

import (
	"github.com/gin-gonic/gin"
)

type request struct {
	Operation string `json:"operation"`
	Mode      string `json:"mode"`
	FileName  string `json:"fileName"`
}

func main() {
	// 创建一个 Gin 路由器
	router := gin.Default()
	log_router := gin.Default()

	router.OPTIONS("/tftp", func(c *gin.Context) {
		c.Header("Access-Control-Allow-Origin", "*") // 允许任何域名访问
		c.Header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
		c.Header("Access-Control-Allow-Headers", "Content-Type, Authorization")
		c.Header("Access-Control-Allow-Credentials", "true")
		c.Status(200)
	})

	log_router.OPTIONS("/log", func(c *gin.Context) {
		c.Header("Access-Control-Allow-Origin", "*") // 允许任何域名访问
		c.Header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
		c.Header("Access-Control-Allow-Headers", "Content-Type, Authorization")
		c.Header("Access-Control-Allow-Credentials", "true")
		c.Status(200)
	})

	log_router.POST("/log", receiveLog)
	router.GET("/ws", handleWebSocket)
	router.POST("/tftp", tftp)

	go func() {
		if err := router.Run(":8080"); err != nil {
			panic(err)
		}
	}()

	go func() {
		if err := log_router.Run(":8081"); err != nil {
			panic(err)
		}
	}()

	select {}
}
