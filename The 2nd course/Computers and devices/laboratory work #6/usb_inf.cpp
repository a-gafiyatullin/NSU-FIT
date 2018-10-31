#include <iostream>
#include <string>
#include <libusb.h>
#include <iomanip>
using namespace std;

class Device{
private:
    libusb_device *dev;
    libusb_device_descriptor desc;      //дескриптор устройства
    libusb_config_descriptor *config;   //дескриптор конфигурации объекта
public:
    Device(libusb_device *dev);
    static const string get_device_class(int bDeviceClass);
    const string get_interfaces_info() const;
    friend ostream& operator<<(ostream &s, const Device &dev);
    const string get_serial_number() const;
    ~Device();
};

Device::Device(libusb_device *dev){
    this->dev = dev;
    int error_code = libusb_get_device_descriptor(dev, &desc);
    if (error_code < 0)
        throw domain_error("Error: haven't got device descriptor, code: " +
            to_string(error_code));
    //получить конфигурацию устройства
    libusb_get_config_descriptor(dev, 0, &config);
}

const string Device::get_device_class(int bDeviceClass){
    switch(bDeviceClass){
        case LIBUSB_CLASS_PER_INTERFACE :
            return string("Each interface specifies its own class information");
        case LIBUSB_CLASS_AUDIO : return string("Audio class");
        case LIBUSB_CLASS_COMM : return string("Communications class");
        case LIBUSB_CLASS_HID  : return string("Human Interface Device class");
        case LIBUSB_CLASS_PHYSICAL  : return string("Physical");
        case LIBUSB_CLASS_PTP : return string("Image class");
        case LIBUSB_CLASS_PRINTER : return string("Printer class");
        case LIBUSB_CLASS_MASS_STORAGE : return string("Mass storage class");
        case LIBUSB_CLASS_HUB : return string("Hub class");
        case LIBUSB_CLASS_DATA : return string("Data class");
        case LIBUSB_CLASS_SMART_CARD : return string("Smart Card");
        case LIBUSB_CLASS_CONTENT_SECURITY : return string("Content Security");
        case LIBUSB_CLASS_VIDEO : return string("Video");
        case LIBUSB_CLASS_PERSONAL_HEALTHCARE : return string("Personal Healtcare");
        case LIBUSB_CLASS_DIAGNOSTIC_DEVICE : return string("Diagnostic Device");
        case LIBUSB_CLASS_WIRELESS  : return string("Wireless class");
        case LIBUSB_CLASS_APPLICATION : return string("Application class");
        case LIBUSB_CLASS_VENDOR_SPEC : return string("Class is vendor-specific");
        default : return string(" ");
    }
}

const string Device::get_interfaces_info() const{
    const libusb_interface *inter;
    const libusb_interface_descriptor *interdesc;
    stringstream interfaces_info;
    interfaces_info << right << setw(30) << "Number of interfaces: "
        << to_string(config->bNumInterfaces) << endl;
    for(int i = 0; i < (int)config->bNumInterfaces; i++){
        inter = &config->interface[i];
        interfaces_info << right << setw(15) << i + 1 << ") "
            << "Number of alternate settings:  " << inter->num_altsetting << endl;
        for(int j = 0; j < inter->num_altsetting; j++) {
            interdesc = &inter->altsetting[j];
            interfaces_info << right << setw(20) << j + 1 << ") "
                << "Interface class: "
                << get_device_class(interdesc->bInterfaceClass) << endl;
        }
    }
    return interfaces_info.str();
}

const string Device::get_serial_number() const {
    libusb_device_handle *handle;
    int error_code = libusb_open(dev, &handle);
    unsigned char serial_number[255];
    error_code = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber,
         serial_number, 255);
    libusb_close(handle);
    return string(reinterpret_cast<char*>(serial_number));
}

ostream& operator << (ostream &s, const Device &dev){
    s << left << setw(15) << setfill(' ') << hex
        << "Device class: " << Device::get_device_class(dev.desc.bDeviceClass)
                            << endl << left << setfill(' ') << setw(15)
        << "Serial number: " << dev.get_serial_number() << endl << left
                            << setfill(' ') << setw(15)
        << "Vendor ID: " << right << setfill('0') << setw(4) << dev.desc.idVendor
                            << endl << left << setfill(' ') << setw(15)
        << "Product ID: " << right << setw(4) << setfill('0')
                            << dev.desc.idProduct << endl << right
                            << setfill(' ') << setw(28)
        << "Interfaces information: " << endl << dev.get_interfaces_info() << endl;
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
        cout << i + 1 << ") " << endl << Device(devs[i]) << endl;//печать параметров устройства
    }
    //printf("===========================================================\n");
    //освободить память, выделенную функцией получения списка устройств
    libusb_free_device_list(devs, 1);
    //завершить работу с библиотекой libusb, закрыть сессию работы с libusb
    libusb_exit(ctx);
    return 0;
}
