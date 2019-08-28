package main

import (
	"fmt"

	"github.com/MutexUnlocked/robotgo/clipboard"
)

func main() {
	text, err := clipboard.ReadAll()
	if err != nil {
		panic(err)
	}

	fmt.Print(text)
}
