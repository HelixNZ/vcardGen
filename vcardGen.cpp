#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

std::string formatNumberString(const std::string& _s, int _i)
{
    std::string strNum = std::to_string(_i);
    std::string strOut = _s;

    //count wildcard characters
    auto n = std::count(_s.begin(), _s.end(), '*');

    //number won't fit
    if((int)strNum.length() > n) return("");

    //prepend zeros to number to fill all wildcards
    while((int)strNum.length() < n) strNum = "0" + strNum;

    //insert number into string
    for(int i = 0; i < n; ++i) strOut[strOut.rfind('*')] = strNum[(n - 1) - i];

    return(strOut);
}

bool writeNumbersToFile(std::fstream& _rFile, std::string _strfmt)
{
    //check file status
    if(!_rFile.is_open() || !_rFile.good())
    {
        std::cout << "\nFile error\n";
        return false;
    }

    //count wildcards
    auto n = std::count(_strfmt.begin(), _strfmt.end(), '*');

    //check if numbers to be generated exceeds 25k (max numbers for a vcard)
    int total = (int)pow(10, n);
    if(total > 25000)
    {
        std::cout << std::fixed << "\nError: Numbers to be generated based on format exceeds 25,000 numbers (" << total << ")\n";
        return false;
    }

    //write to file
    std::string strTel = "TEL;type=CELL:";
    for(int i = 0; i < total; ++i)
    {
        _rFile << strTel << formatNumberString(_strfmt, i) << "\n";
    }

    std::cout << "\nGenerated " << total << " numbers based on given format\n";
    return(true);
}

int main()
{
    std::cout << "VCARD Generator\n\n";

    //get name
    std::string strName;
    std::cout << "\nEnter contact name: ";
    std::getline(std::cin >> std::ws, strName);

    //request fmt/range
    std::string strFmt;
    std::cout << "\nPlease enter desired number with wildcards for generatiom (eg: (555) 5*5-55**):\n";
    std::getline(std::cin >> std::ws, strFmt);

    //open file
    std::fstream fileStream;
    fileStream.open(strName + ".vcf", std::ios_base::out | std::ios_base::trunc);

    //Check if open
    if(fileStream.is_open())
    {
        //write header
        fileStream << "BEGIN:VCARD\nVERSION:3.0\n";
        
        //write name,fn
        bool bFirstLast = (strName.find(' ') != strName.npos);
        fileStream << "N:" << (bFirstLast ? strName.substr(strName.find(' ') + 1) : "") << ";" << strName.substr(0, strName.find(' ')) << ";;;\n";
        fileStream << "FN:" << strName << "\n";

        //build number listings
        bool bSuccess = writeNumbersToFile(fileStream, strFmt);
        if(!bSuccess) std::cout << "\nFailed to generate the numbers for the vcard\n";

        //write eof
        fileStream << "END:VCARD\n\n";
    }
    else
    {
        std::cout << "\nFailed to create vcard\n";
    }

    //close regardless
    fileStream.close();

    return(0);
}