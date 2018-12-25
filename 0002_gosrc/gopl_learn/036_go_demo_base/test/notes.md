# test的使用
1. 一般情况下使用 `go test . `进行测试
2. `go test -v` 可以用于打印每个测试函数的名字和运行时间
3. 测试未通过的项不影响测试的继续进行，会在所有测试结束后给出测试结果
4. `go test -v -run="List"` 只测试正则表达式匹配的用例run= 后为正则表达式
5. 修复了失败的测试用例，在提交代码前，应该以不带参数的go test进行全部测试用例的执行，以确保修复失败测试的同时没有引入新的问题。
6. `go test -coverprofile=c.out` 代码覆盖率测试 使用 `less c.out` 查看
7. `go tool cover -html=c.out` 将覆盖率文件c.out转换为html格式
# 测试一个命令
1. 参见echo例子