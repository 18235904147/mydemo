
#include <iostream>
#include "tlv_box.h"

using namespace tlv;


int main(int argc, char const *argv[])
{
    DiscoveryDeviceInfo box;

    box.PutUuidValue(DEVICE_UUID, (char *)"dfiu23yr8u2342390ds90fsdfa9s8f90");
    box.PutDevTypeValue(DEVICE_TYPE, (int)3);
    box.PutDevNameValue(DEVICE_NAME, (char *)"hello world !");

    if (!box.Serialize()) {
        std::cout << "box Serialize Failed !\n";
        return -1;
    }

    std::cout << "box Serialize Success, " << box.GetSerializedBytes() << " bytes \n";

    DiscoveryDeviceInfo boxes;
    boxes.PutObjectValue(DEVICE_ALL, box);
    
    if (!boxes.Serialize()) {
        std::cout << "boxes Serialize Failed !\n"; 
        return -1;
    }

    std::cout << "boxes Serialize Success, " << boxes.GetSerializedBytes() << " bytes \n";
    
    DiscoveryDeviceInfo parsedBoxes;
    if (!parsedBoxes.Parse(boxes.GetSerializedBuffer(), boxes.GetSerializedBytes())) {
        std::cout << "boxes Parse Failed !\n";
        return -1;
    }

    std::cout << "boxes Parse Success, " << parsedBoxes.GetSerializedBytes() << " bytes \n";

    DiscoveryDeviceInfo parsedBox;
    if (!parsedBoxes.GetObjectValue(DEVICE_ALL, parsedBox)) {
        std::cout << "GetObjectValue Failed !\n";
        return -1;
    }

    std::cout << "box Parse Success, " << parsedBox.GetSerializedBytes() << " bytes \n";

    std::vector<int> tlvList;
    int numTlvs = parsedBox.GetTLVList(tlvList);
    std::cout <<  "box contains " << numTlvs << " TLVs: \n";
    for (int i=0;i<numTlvs; i++)
        std::cout << "Tlv " << std::hex << tlvList[i] << "\n";


    {
        int value;
        if (!parsedBox.GetDevTypeValue(DEVICE_TYPE, value)) {
            std::cout << "GetIntValue Failed !\n";
            return -1;
        }
        std::cout << "GetIntValue Success " << value << std::endl;
    }

    {
        char value[128];
        int length = 128;
        if (!parsedBox.GetUuidValue(DEVICE_UUID, value, length)) {
            std::cout << "GetStringValue Failed !\n";
            return -1;
        }
        std::cout << "GetStringValue Success " << value << std::endl;
    }

    {
        char value[128];
        int length = 128;
        if (!parsedBox.GetDevNameValue(DEVICE_NAME, value, length)) {
            std::cout << "GetStringValue Failed !\n";
            return -1;
        }
        std::cout << "GetStringValue Success " << value << std::endl;
    }


    return 0;
}
