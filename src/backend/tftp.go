package main

import (
	"fmt"
	"github.com/gin-gonic/gin"
	"os/exec"
)

// 定义tftp_cpp.exe的路径
var tftp_path string = "D:\\codefield\\tftp-client\\src\\cpp\\cmake-build-debug\\tftp_cpp.exe"

func tftp(c *gin.Context) {

	// 创建一个请求对象
	var req request

	// 检查请求主体是否为空
	if c.Request.Body == nil {
		c.JSON(400, gin.H{"error": "Request body is empty"})
		return
	}

	// 读取请求主体中的 JSON 数据并绑定到请求对象中
	if err := c.BindJSON(&req); err != nil {
		// 记录错误信息
		fmt.Println("Error binding JSON:", err)

		// 如果绑定失败，说明请求主体中的 JSON 数据不符合要求
		// 使用 c.JSON 方法返回一个 JSON 格式的响应
		c.JSON(400, gin.H{"error": err.Error()})
		return
	}
	// 打印请求对象
	fmt.Println(req)

	//调用tftp_cpp.exe
	cmd := exec.Command(tftp_path, req.Operation, req.Mode, req.FileName)
	err := cmd.Run()
	if err != nil {
		fmt.Println("Error executing command:", err)
		c.JSON(400, gin.H{"error": err.Error()})
		return
	}
	// 返回一个 JSON 格式的响应
	c.JSON(200, gin.H{"status": "ok"})
}
