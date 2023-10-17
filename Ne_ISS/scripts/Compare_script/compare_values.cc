#include <iostream>
#include <fstream>
#include <string>
#include <map>


std::map<std::string, long double> map_value(std::string filename){
    
    //open the file  
    std::ifstream input(filename);

    // Create a map to store the counts for each value from the file
    std::map<std::string, long double> asic_value;

    std::string line;
    while (std::getline(input, line)) {
        // Split the line into a name and a value
        size_t colonPos = line.find(':');
        std::string name = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        //Store the value for a particular asic
        asic_value[name] = std::stold(value);
    }

    // Close the input file
    input.close();

    // std::ofstream outfile( "autocals_test.dat" );

    // // Print the value for each asic
    // for (const auto& element : asic_value) {
    //     outfile << element.first << ": " << element.second << std::endl;
    // }

    // outfile.close();
    
    return asic_value;
}


int Percentage_difference(std::string filename1, std::string filename2)
{
    //define the two maps that will be compared
    const std::map<std::string, long double> autocal1 = map_value(filename1);
    const std::map<std::string, long double> autocal2 = map_value(filename2);

    std::string directory = "";
    std::string filename = "Compare_thresh_NOT-PHD_to_PHD.dat";
    std::ofstream out_file;

    out_file.open(directory + filename, std::ios::out);
    if (!out_file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    //Iterativley compare asic values in the first map to the second
    for (const auto& element : autocal1){
        auto iter = autocal2.find(element.first);

        //if the same asic exists in both maps, compare the values via percentage difference
        if (iter != autocal2.end()) {
            out_file << element.first << "_pdiff" << ": " << (abs(element.second - iter->second)/( (element.second + iter->second)/2.0))*100 << std::endl;
        }
    }

    out_file.close();

    return 0;
}

int main()
{
    Percentage_difference("/media/ben/TOSHIBA_brj/Ne_ISS/scripts/Thresholds/thresholds_R1_all.dat", "/media/ben/TOSHIBA_brj/Ne_ISS/scripts/Thresholds/NOT_PHD_corrected/thresholds_R1_all.dat");
    return 0;
}

