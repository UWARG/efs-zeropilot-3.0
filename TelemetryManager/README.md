# Telemetry Manager

## CRC32 code

To generate the CRC32 header for a given polynomial, plug this GO code into the [go playground](https://go.dev/play/) and click run.
```go
package main

import (
	"fmt"
	"hash/crc32"
)

func main() {
	// generate function defs, includes, etc
	fmt.Print("#pragma once\n\n#include <cstdint>\n\n")
	fmt.Print("const uint32_t calculateChecksum(uint8_t* buf, uint8_t size);\n\n")

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

## LCM Messages 

To build the LCM messages, call ``Tools/build_messages.sh cpp``, it will build the C++ headers under ``TelemetryManager/Inc/messages``. You can clean the headers with the ``clean`` argument.

To build the python messages, do the same as above, but the first argument must be python, and pass a location for the second argument. It will build the messages in ``<location>/messages``.

#### IMPORTANT NOTE
All LCM messages must have a `Header header;` as the first variable and a `byte crc[4];` as the last variable.


## Testing

There is a file in src called TM_test.cpp, meant only for testing TM functions. you can compile and run it by running the following command from the TelemetryManager folder.

```
g++ -o TM_test Src/* -I Inc ; echo "running now"; ./TM_test; rm TM_test
``` 
This will compile all cpp files and use Inc as the includes path, run the program, then delete the compiled program.
