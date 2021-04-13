package main

func add(x int, y int, z ...int) int {
	return x + y
}

func main() {
	println(add(1, 2))
	a := []int{1, 3}
	println(add(1, 2, a...))
}
