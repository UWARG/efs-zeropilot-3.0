# Telemetry Manager

## CRC32 code

To generate the CRC32 table for a given polynomial, plug this GO code into the [go playground](https://go.dev/play/) and click run.
```go
package main

import (
	"fmt"
	"hash/crc32"
)

func main() {
	// generate function defs, includes, etc
	fmt.Print("#pragma once\n#include <cstdint>\n\n")
	fmt.Print("uint32_t calculateChecksum(uint8_t* buf, uint8_t size);\n\n")

	// generate crc32 table
	table := crc32.MakeTable(crc32.IEEE) // <- this is where the polynomial is defined

	fmt.Print("uint32_t crcTable[256] = {\n")
	for index, val := range table {

		if index%8 == 0 {
			fmt.Print("\t")
		}

		fmt.Printf("0x%08X", val)

		if index != 255 {
			fmt.Print(", ")
		}
		if (index+1)%8 == 0 {
			fmt.Print("\n")
		}
	}
	fmt.Print("};\n")
}
```
The output of this code should just be the contents of CRC32.hpp. To change the polynomial, edit the part that says ``crc32.IEEE``. You can add a polynomial in hex, as a uint32, or take one of the other predefined constants from the golang [stdlib](https://cs.opensource.google/go/go/+/refs/tags/go1.19.3:src/hash/crc32/crc32.go;l=26).