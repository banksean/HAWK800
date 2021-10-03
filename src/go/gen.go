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

	fmt.Printf(`
typedef struct {
  byte cc;
  byte min;
  byte max;
} Param;
`)

	// Name, CC#, Min, Max, Range
	vars := []string{}
	names := []string{}
	for _, row := range rows[1:] {
		name := strings.TrimSpace(strings.ToTitle(row[0]))
		for _, c := range ".()/-*" {
			name = strings.Replace(name, string(c), "", -1)
		}
		name = strings.ReplaceAll(name, " ", "_")
		if name == "NOT_USED" {
			continue
		}
		display := row[5]
		if len(display) == 0 {
			continue
		}
		vars = append(vars, "&"+name)
		names = append(names, name+"_S")
		fmt.Printf("const char %s_S[] PROGMEM = \"%s\";\n", name, display)
		fmt.Printf("Param %s{%s, %s, %s};\n", name, row[1], row[2], row[3])
	}

	fmt.Printf("const int NUM_PARAMS = %d;\n", len(vars))
	fmt.Printf("Param *ALL_PARAMS[]{%s};\n", strings.Join(vars, ", "))
	fmt.Printf("const char* const PARAM_NAMES[NUM_PARAMS] PROGMEM = {%s};\n", strings.Join(names, ", "))
}
