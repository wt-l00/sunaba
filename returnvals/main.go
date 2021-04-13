package main

// 名前付き変数を宣言したくないときは，_を使う
func add_sub(m int, n int) (x int, _ int) {
	x = m + n
	y := m - n
	return x, y
}

func main() {
	x, y := add_sub(5, 3)
	println(x, y)
}
