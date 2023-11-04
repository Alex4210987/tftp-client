package main

import (
	"github.com/gin-gonic/gin"
)

/*
#include "main.h"
*/
import "C"

type request struct {
	operation int   `json:"operation"`
	mode      int   `json:"mode"`
	fileName  string `json:"fileName"`
}
//var requestData = {
//                operation: operation,
//                mode: mode,
//                fileName: fileName
//            };

func processer(c *gin.Context) {
	// 创建一个请求对象
	var req request
	// 将请求的主体（Body）中的 JSON 数据绑定到请求对象中
	if err := c.BindJSON(&req); err != nil {
		// 如果绑定失败，说明请求主体中的 JSON 数据不符合要求
		// 使用 c.JSON 方法返回一个 JSON 格式的响应
		c.JSON(400, gin.H{"error": err.Error()})
		return
	}
	// 调用 C++ 的处理函数
	if(req.operation == 1){
		C.mainProgram::runRead(req.mode, req.fileName)
	}else if(req.operation == 2){
		C.mainProgram::runWrite(req.mode, req.fileName)
	}
	// 返回一个空的响应
	c.JSON(200, gin.H{})
}
