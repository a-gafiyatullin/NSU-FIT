#include <iostream>
#include <string>
#include <libusb.h>
#include <iomanip>
using namespace std;

class Device{
private:
    libusb_device_descriptor desc;      //дескриптор устройства
    libusb_config_descriptor *config;   //дескриптор конфигурации объекта
public:
    Device(libusb_device *dev);
    const string get_device_class() const;
    friend ostream& operator<<(ostream &s, const Device &dev);
    ~Device();
};

Device::Device(libusb_device *dev){
    int error_code = libusb_get_device_descriptor(dev, &desc);
    if (error_code < 0)
        throw domain_error("Error: haven't got device descriptor, code: " +
            to_string(error_code));
    //получить конфигурацию устройства
    libusb_get_config_descriptor(dev, 0, &config);
}

const string Device::get_device_class() const{
    switch(desc.bDeviceClass){
        case 0x00 : return string(" ");
        case 0x01 : return string("Audio device");
        case 0x02 : return string("Network adapter");
        case 0x03 : return string("User interface device");
        case 0x05 : return string("Physical device");
        case 0x06 : return string("Images");
        case 0x07 : return string("Printer");
        case 0x08 : return string("Data storage device");
        case 0x09 : return string("Concentrator");
        case 0x0A : return string("CDC-Data");
        case 0x0B : return string("Smart Card");
        case 0x0D : return string("Content Security");
        case 0x0E : return string("Video device");
        case 0x0F : return string("Personal medical device");
        case 0x10 : return string("Audio and Video devices");
        case 0xdc : return string("Diagnostic device");
        case 0xe0 : return string("Wireless controller");
        case 0xef : return string("Various devices");
        case 0xfe : return string("Special devices");
        default   : return string(" ");
    }
}

ostream& operator << (ostream &s, const Device &dev){
    s << left << setw(14) << setfill(' ') << hex
        << "Device class: " << dev.get_device_class() << endl << left
                            << setfill(' ') << setw(14) 
        << "Vendor ID: " << right << setfill('0') << setw(4) << dev.desc.idVendor
                            << endl << left << setfill(' ') << setw(14)
        << "Product ID: " << right << setw(4) << setfill('0')
                            << dev.desc.idProduct << endl;
    return s;
}

Device::~Device(){
    libusb_free_config_descriptor(config);
}

int main(){
    //указатель на указатель на устройство, используется для получения списка устройств
    libusb_device **devs;
    //контекст сессии libusb, позволяет использовать библиотеку
    //нескольким пользователям одновременно без нанесения друг другу ущерба
    libusb_context *ctx = NULL;
    int error_code;      //для возвращаемых значений
    ssize_t cnt;         //число найденных USB-устройств
    ssize_t i;           //индексная переменная цикла перебора всех устройств
    //инициализировать библиотеку libusb, открыть сессию работы с libusb
    error_code = libusb_init(&ctx);
    if(error_code < 0)
        throw domain_error("Error: haven't got initialization, code: " +
            to_string(error_code));
    //инициализировать библиотеку libusb, открыть сессию работы с libusb
    libusb_set_debug(ctx, 3);
    //получить список всех найденных USB-устройств
    cnt = libusb_get_device_list(ctx, &devs);
    if(cnt < 0)
        throw domain_error("Error: haven't got device list, code: " +
            to_string(error_code));
    for(i = 0; i < cnt; i++){              //цикл перебора всех устройств
        cout << Device(devs[i]) << endl;   //печать параметров устройства
    }
    //printf("===========================================================\n");
    //освободить память, выделенную функцией получения списка устройств
    libusb_free_device_list(devs, 1);
    //завершить работу с библиотекой libusb, закрыть сессию работы с libusb
    libusb_exit(ctx);
    return 0;
}
