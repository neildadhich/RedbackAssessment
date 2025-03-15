#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
// Question 3: This is an extension task that requires you to decode sensor data from a CAN log file.
// CAN (Controller Area Network) is a communication standard used in automotive applications (including Redback cars)
// to allow communication between sensors and controllers.
//
// Your Task: Using the definition in the Sensors.dbc file, extract the "WheelSpeedRR" values
// from the candump.log file. Parse these values correctly and store them in an output.txt file with the following format:
// (<UNIX_TIME>): <DECODED_VALUE>
// eg:
// (1705638753.913408): 1234.5
// (1705638754.915609): 6789.0
// ...
// The above values are not real numbers; they are only there to show the expected data output format.
// You do not need to use any external libraries. Use the resources below to understand how to extract sensor data.
// Hint: Think about manual bit masking and shifting, data types required,
// what formats are used to represent values, etc.
// Resources:
// https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial
// https://www.csselectronics.com/pages/can-dbc-file-database-intro


// Solution: 

// After reading the first resource I understand CAN bus and its uses and how the data should be used.
// Second resource end was helpful in decoding. 

//BO_ 1797 ECU_WheelSpeed: 8 Vector__XXX
// SG_ WheelSpeedRR : 32|16@0+ (0.1,0) [0|0] "km/h" Vector__XXX

// We care about the above output as we need the rear right wheel only. 
// The first line decoded is: 
// ID: 1797 - Standard frame as it is given in the 11 bit range of 0-2047
// Name: ECU_WheelSpeed
// Length #Data Bytes: 8
// Sender name: no name available hence Vector_XXX

// Then we have signal syntax explained 
// Name: WheelSpeedRR
// The bit start: (when the message starts) = 32nd bit
// The message lenght is: 16 bits
// @0 = big-endian/Motorola (didnt remember much on big endian from 1521 so had to research)
// + means it is unsigned
// (0.1,0): scaling factor offset which is physicalVal = (RawVal) * 0.1 + 0
// the min/max is optional and not used here
// unit is km/h 


// Now i tried to understand the log outputs 
//(1705638753.913408) vcan0 705#39C2A37B95B17C57
// the brackets is the timestamp, vcan0 is jsut virtual can bus which is used in simulation so
// contextually makes sense for what we are doing and then I chose 705 log since 1797 translates 
// to 705 in hex. Now it makes a lot of sense since each of the other SG_ ones had split the 64 bits into 
// 16 bits to represent the different wheel speeds in the data payload. In our case we have to use from the 32nd bit
// till the 47th bit. Since counting starts at 4, we read from the 33rd bit to 48th bit inclusive. This is 16 bits. 
// The big endian means the MSB is the 33rd bit and we go in a sequential order. So it is in order of MSB to LSB and the order
// is maintained acoress the byte boundaries. 

// So we care about 95B1 which represents out wheel speed in km/h but we have to multiply this value by 0.1.
// 95B1 = 38321 = 38321*0.1 = 3832.1 km/h

// Now i need to program this. A lot of this is 1521 knowledge but I forgot. So i had to reseach how to 
// open files, create new files, recurse through each line and find t

int main() {
    std::ifstream infile("../Question-3/candump.log");
    if (!infile) {
        std::cerr << "Error opening candump.log" << std::endl;
        return 1;
    }
    // Open the output file.
    std::ofstream outfile("../Question-3/output.txt");
    if (!outfile) {
        std::cerr << "Error opening output.txt" << std::endl;
        return 1;
    }

    std::string line;
    // Process the file line by line.
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string timestamp, iface, canMsg;

        //The iss is the complete string, each >> reads the next bit of it from the whitespace. 
        //Had to research this too.
        iss >> timestamp >> iface >> canMsg;
        size_t hashPos = canMsg.find('#');

        //This condition is true if the '#' was not found in the string.
        //I had to google this, but I didn't add much error checking since
        //the output in the log file seems correct and it wasn't asked.
        //If I had to I can add more error checking. s
        if (hashPos == std::string::npos){
            continue;
        }

        //find substring
        std::string msgID = canMsg.substr(0, hashPos);
        //skip if not relevant
        if (msgID != "705") {
            continue;
        }
        std::string dataHex = canMsg.substr(hashPos + 1);

        // This is basically using the 8, 9 character for byte4Str
        // This is basically using 10, 11 character for byte5Str
        std::string byte4Str = dataHex.substr(8, 2);  // 5th byte
        std::string byte5Str = dataHex.substr(10, 2); // 6th byte

        // Convert these two hex strings to integer values.
        unsigned int byte4 = std::stoul(byte4Str, nullptr, 16);
        unsigned int byte5 = std::stoul(byte5Str, nullptr, 16);

        // Combine the two bytes into one 16-bit value.
        // Since it is big-endian, byte4 is the high byte.
        unsigned int rawValue = (byte4 << 8) | byte5;

        // Apply the scaling factor (0.1) and offset (0)
        double wheelSpeed = rawValue * 0.1;

        outfile << timestamp << ": " << std::fixed << std::setprecision(1) << wheelSpeed << std::endl;
    }
    infile.close();
    outfile.close();
    return 0;
}


//To validate my solution and do a rough check, I did contrl F to find how many "705#" exists in the
//file and it is 2364 which is my output number of lines. I also checked the manual calculation I did initially
//with the output for that line and it matched. Further testing can be done. 