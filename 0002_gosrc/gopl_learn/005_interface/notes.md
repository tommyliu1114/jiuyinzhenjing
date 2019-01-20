interface被赋予了很多不同的含义，每个类型都有借口，意味着对
那个类型定义了方法集合，这段代码定义了具有一个字段和两个方法的结果类型
type S struct{i int}
func(p *S)Get() int{return p.i}
func(p *S)Put(v int){p.i = v}
定义接口类型，仅仅是方法的集合，定义了一个有两个方法的接口
type I interface{
	Get() int
	Put(int)
}
对于接口I，S是合法的实现，因为它定义了I所需的两个方法，即使
没有明确定义S实现了I 也是正确的
GO可以利用这个特点来实现接口的另一个含义，接口值
func f(p I){//定义一个函数接受一个接口类型作为参数
	fmt.Println(p.Get)//p实现了接口I，必须有get()方法
	p.Put(1)
}
这里的变量p保存了接口类型的值，因为S实现了I可以调用f向其
传递S类型的值的指针
var s S;f(&s)
呼气s的地址，而不是S的值，是因为在s的指针上定义了方法