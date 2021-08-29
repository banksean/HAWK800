package main

import (
	"encoding/csv"
	"fmt"
	"os"
	"strings"
)

func main() {
	f, err := os.Open("./ccmsgs.csv")
	if err != nil {
		panic(err)
	}

	r := csv.NewReader(f)

	rows, err := r.ReadAll()
	if err != nil {
		panic(err)
	}

	// First row is column titles.
	for _, row := range rows[1:] {
		name := strings.Trim(strings.ToTitle(row[0]), " ")
		for _, c := range ".()/-*" {
			name = strings.Replace(name, string(c), "", -1)
		}
		name = strings.ReplaceAll(name, " ", "_")
		if name == "NOT_USED" {
			continue
		}
		fmt.Printf("byte %s = %s;\n", name, row[1])
	}
	//fmt.Printf("%+v\n", rows)
}
