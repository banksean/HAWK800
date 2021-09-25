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
	// Name, CC#, Min, Max, Range
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

	// Array of name strings, indexed by CC#
	fmt.Printf("const char* const PARAM_NAMES[] =\n{\n")
	for i, row := range rows[1:] {
		name := strings.Trim(strings.ToTitle(row[0]), " ")
		//for _, c := range ".()/-*" {
		//	name = strings.Replace(name, string(c), "", -1)
		//}
		//name = strings.ReplaceAll(name, " ", "_")
		fmt.Printf("\t\"%s\"", name)
		if i < len(rows)-2 {
			fmt.Printf(",")
		}
		fmt.Printf("\n")
	}
	fmt.Printf("};\n")
}
