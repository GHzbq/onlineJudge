# onlineJudge 在线判题系统

## 一、项目简介

基于B/S模式，简单实现一个在线判题系统

## 二、实现功能

### 1. 浏览器端实现功能

- 登录/注册功能

- 题目列表

- 题目详情

- 代码编辑框

- 提交按钮

- 判题结果展示

- 下一题

- 支持留言评论

- 官方题解

### 2. 服务端实现功能

- 支持登录/注册

- 数据持久化

- 消息队列

- 判题功能
    
    生产源文件
    编译
    运行
    比对结果
    查重


- 针对请求的响应

    登录/注册 请求
    all_questions 请求
    question 请求
    判题结果


- 支持模拟笔试

    拉题
    公司信息（logo、本场笔试简介...）
    回收判题结果
    检查考生的诚信问题（摄像头，录音、切出界面）

## 三、关于服务端的一些问题

### 1. 运行别人代码的安全性问题

- 运行环境安全

- 是否会泄漏测试数据

- 内存/时间限制
