package main

import (
	"github.com/gin-gonic/gin"
)

func main() {
	// 创建一个 Gin 路由器
	router := gin.Default()

	// 为路由器注册一个处理 GET 请求的处理器,转到processer函数
	router.GET("/", processer)；
	// 启动 Gin 服务器，监听在80端口
	// 注意：需要以管理员权限或在合适的环境下运行，因为80端口通常需要特权访问
	router.Run(":80")
}
